#pragma once

#include <ecl/fsm.hpp>

#include "request.hpp"

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
    body_parsed,
    complete
};

struct event_line
{
    explicit event_line(char* l) : line(l)
    {}

    char* line { nullptr }; 
};
struct empty_line {};
struct rst        {};

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

        if(0 == strncmp(method_str, constants::get_method(method::GET), strlen(method_str)))
        {
            m_request.met = method::GET;
        }
        else if(0 == strncmp(method_str, constants::get_method(method::POST), strlen(method_str)))
        {
            m_request.met = method::POST;
        }
        else
        {
            return false;
        }

        m_request.uri = uri_str;

        if(0 == strncmp(version_str, constants::get_version(version::HTTP11), strlen(version_str)))
        {
            m_request.ver = version::HTTP11;
        }
        else if(0 == strncmp(version_str, constants::get_version(version::HTTP20), strlen(version_str)))
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

        m_request.body = (uint8_t*)e.line;

        return true;
    }

    typedef parser_state s;
    typedef parser_fsm   p;

    typedef transition_table<
        row< s::init,                event_line, s::request_line_parsed, nullptr, &p::g_request_line_parse >,
        row< s::request_line_parsed, event_line, s::header_parse,        nullptr, &p::g_header_parse       >,
        row< s::header_parse,        event_line, s::header_parse,        nullptr, &p::g_header_parse       >,
        row< s::header_parse,        empty_line, s::headers_parsed,      nullptr, nullptr                  >,
        row< s::headers_parsed,      event_line, s::body_parsed,         nullptr, &p::g_body_parsed        >,
        row< s::body_parsed,         empty_line, s::complete,            nullptr, nullptr                  >,

        row< s::request_line_parsed, rst,        s::init,                nullptr, nullptr                  >,
        row< s::header_parse,        rst,        s::init,                nullptr, nullptr                  >,
        row< s::headers_parsed,      rst,        s::init,                nullptr, nullptr                  >,
        row< s::body_parsed,         rst,        s::init,                nullptr, nullptr                  >,
        row< s::complete,            rst,        s::init,                nullptr, nullptr                  >
    > transition_table_t;

    typedef callback_table<
        scb< s::init, &p::on_init_enter, nullptr >
    > callback_table_t;

public:
    template<typename event_t>
    parser_state process_event(const event_t& e)
    {
        return transition<event_t, transition_table_t, callback_table_t>(e);
    }

    request* get_request_ptr()
    {
        return &m_request;
    }

private:
    request m_request {};
};

} // namespace web

} // namespace ecl
