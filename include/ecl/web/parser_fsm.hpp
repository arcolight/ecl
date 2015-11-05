#ifndef ECL_WEB_PARSER_FSM_HPP
#define ECL_WEB_PARSER_FSM_HPP

#include <cstring>

#include <ecl/fsm.hpp>

#include <ecl/web/request.hpp>

#include <ecl/web/parser_kv_pairs.hpp>

namespace ecl
{

namespace web
{

enum class parser_state
{
    init,
    request_line_parsed,
    header_parse,
    headers_parsed,
    wait_for_body,
    body_parsed,
    complete
};

struct event_line
{
    explicit event_line(char* l) : line(l)
    {}

    char*  line { nullptr };
};
struct empty_line {};
struct end_of_req {};

class parser_fsm : public state_machine<parser_fsm,
                                        parser_state,
                                        parser_state::init>
{
    void on_init_enter()
    {
        m_request.clear();
    }

    bool g_request_line_parse(const event_line& e)
    {
        char* method_str = e.line;
        if(nullptr == method_str)
        {
            return false;
        }

        char* uri_str = strchr(method_str, ' ');
        if(nullptr == uri_str)
        {
            return false;
        }

        char* version_str = strchr(uri_str + 1, ' ');
        if(nullptr == version_str)
        {
            return false;
        }

        *uri_str = 0x00;     ++uri_str;
        *version_str = 0x00; ++version_str;

        char* uri_param_str = strchr(uri_str, '?');
        if(nullptr != uri_param_str)
        {
            *uri_param_str = 0x00; ++uri_param_str;
            m_request.uri_param_string = uri_param_str;

            if(kv_parser_state::done !=
                   m_uri_param_parser.start_parse(uri_param_str,
                                                  m_request.uri_parameters,
                                                  MAX_URI_PARAMETERS))
            {
                return false;
            }
        }

        size_t method_str_len = strlen(method_str);
        if(0 == strncmp(method_str,
                        to_string(method::GET),
                        method_str_len))
        {
            m_request.met = method::GET;
        }
        else if(0 == strncmp(method_str,
                             to_string(method::POST),
                             method_str_len))
        {
            m_request.met = method::POST;
        }
        else if(0 == strncmp(method_str,
                             to_string(method::PUT),
                             method_str_len))
        {
            m_request.met = method::PUT;
        }
        else
        {
            return false;
        }

        m_request.uri = uri_str;

        size_t version_str_len = strlen(version_str);
        if(0 == strncmp(version_str,
                        to_string(version::HTTP10),
                        version_str_len))
        {
            m_request.ver = version::HTTP10;
        }
        else if(0 == strncmp(version_str,
                             to_string(version::HTTP11),
                             version_str_len))
        {
            m_request.ver = version::HTTP11;
        }
        else if(0 == strncmp(version_str,
                             to_string(version::HTTP20),
                             version_str_len))
        {
            m_request.ver = version::HTTP20;
        }
        else
        {
            return false;
        }

        return true;
    }

    bool g_header_parse(const event_line& e)
    {
        char* name = e.line;
        if(nullptr == name)
        {
            return false;
        }

        char* value = strchr(name, ':');
        if(nullptr == value)
        {
            return false;
        }

        *value = 0x00; ++value;

        while(*name == ' ')
        {
            *name = 0;
            ++name;
        }
        while(*value == ' ')
        {
            *value = 0;
            ++value;
        }

        m_request.headers[m_request.headers_count].name  = name;
        m_request.headers[m_request.headers_count].value = value;
        ++m_request.headers_count;

        return true;
    }

    bool g_body_parsed(const event_line& e)
    {
        if(nullptr == e.line)
        {
            return false;
        }

        m_request.body = e.line;

        return true;
    }

    using s = parser_state;
    using p = parser_fsm;

    using transition_table_t = transition_table
    <
        row< s::init,                event_line, s::request_line_parsed, nullptr, &p::g_request_line_parse >,
        row< s::request_line_parsed, event_line, s::header_parse,        nullptr, &p::g_header_parse       >,
        row< s::header_parse,        event_line, s::header_parse,        nullptr, &p::g_header_parse       >,
        row< s::header_parse,        empty_line, s::headers_parsed,      nullptr, nullptr                  >,
        row< s::headers_parsed,      end_of_req, s::complete,            nullptr, nullptr                  >,
        row< s::headers_parsed,      event_line, s::body_parsed,         nullptr, &p::g_body_parsed        >,
        row< s::body_parsed,         end_of_req, s::complete,            nullptr, nullptr                  >,

        row< s::request_line_parsed, rst,        s::init,                nullptr, nullptr                  >,
        row< s::header_parse,        rst,        s::init,                nullptr, nullptr                  >,
        row< s::headers_parsed,      rst,        s::init,                nullptr, nullptr                  >,
        row< s::body_parsed,         rst,        s::init,                nullptr, nullptr                  >,
        row< s::complete,            rst,        s::init,                nullptr, nullptr                  >
    >;

    using callback_table_t = callback_table
    <
        scb< s::init, &p::on_init_enter >
    >;

public:
    template<typename event_t>
    parser_state process_event(const event_t& e)
    {
        return transition<event_t, transition_table_t, callback_table_t>(e);
    }

    const request* get_request_ptr()
    {
        return &m_request;
    }

private:
    kv_parser m_uri_param_parser
    {
        str_const("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"),
        str_const("%+.-"),
        str_const("="),
        str_const("&;")
    };

    request                  m_request          {};
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_PARSER_FSM_HPP
