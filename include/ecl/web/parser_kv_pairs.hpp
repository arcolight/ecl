#ifndef ECL_WEB_PARSER_KEY_VALUE_PAIRS_FSM_HPP
#define ECL_WEB_PARSER_KEY_VALUE_PAIRS_FSM_HPP

#include <ecl/fsm.hpp>
#include <ecl/str_const.hpp>

#include <ecl/web/uri_param.hpp>

namespace ecl
{

namespace web
{

enum class kv_parser_state
{
    init,
    name_parsing,
    value_parsing,
    equal_found,
    delimiter_found,
    done,
    error
};

struct rst                  {};
struct token_char           {};
struct value_add_token_char {};
struct equal_sign
{
    explicit equal_sign(char* p_) : p(p_) {}
    char* p;
};
struct delimiter
{
    explicit delimiter(char* p_) : p(p_) {}
    char* p;
};
struct end                  {};

class kv_parser : public state_machine
                         <
                            kv_parser,
                            kv_parser_state,
                            kv_parser_state::init
                         >
{
public:
    kv_parser(const str_const& token_chars,
              const str_const& value_additional_chars,
              const str_const& equal_chars,
              const str_const& delimiter_chars) :
        m_token_chars(token_chars),
        m_value_additional_token_chars(value_additional_chars),
        m_equal_chars(equal_chars),
        m_delimiter_chars(delimiter_chars)
    {}

private:

    void on_init_enter()
    {
        m_params_count = 0;
    }

    void on_equal_found(const equal_sign& e)
    {
        m_params[m_params_count].name = m_token;
        *e.p = 0;
        m_token = e.p + 1;
    }

    void on_delimiter_found_name(const delimiter& e)
    {
        if(m_params_count == m_max_params_count)
        {
            return;
        }

        m_params[m_params_count].name = m_token;
        *e.p = 0;
        m_token = e.p + 1;

        ++m_params_count;
    }

    void on_end_name(const end&)
    {
        if(m_params_count == m_max_params_count)
        {
            return;
        }

        m_params[m_params_count].name = m_token;
        ++m_params_count;
    }

    void on_delimiter_found_value(const delimiter& e)
    {
        if(m_params_count == m_max_params_count)
        {
            return;
        }

        m_params[m_params_count].value = m_token;
        *e.p = 0;
        m_token = e.p + 1;

        ++m_params_count;
    }

    void on_end_value(const end&)
    {
        m_params[m_params_count].value = m_token;
        ++m_params_count;
    }

    using s = kv_parser_state;
    using p = kv_parser;

    using transition_table_t = transition_table
    <
        // Normal ways
        row< s::init            , token_char           , s::name_parsing                                   >,
        row< s::init            , end                  , s::done                                           >,

        row< s::name_parsing    , token_char           , s::name_parsing                                   >,
        row< s::name_parsing    , equal_sign           , s::equal_found     , &p::on_equal_found           >,
        row< s::name_parsing    , delimiter            , s::delimiter_found , &p::on_delimiter_found_name  >,
        row< s::name_parsing    , end                  , s::done            , &p::on_end_name              >,

        row< s::equal_found     , token_char           , s::value_parsing                                  >,
        row< s::equal_found     , delimiter            , s::name_parsing                                   >,

        row< s::value_parsing   , token_char           , s::value_parsing                                  >,
        row< s::value_parsing   , value_add_token_char , s::value_parsing                                  >,
        row< s::value_parsing   , delimiter            , s::delimiter_found , &p::on_delimiter_found_value >,
        row< s::value_parsing   , end                  , s::done            , &p::on_end_value             >,

        row< s::delimiter_found , token_char           , s::name_parsing                                   >,

        // Errors
        row< s::init            , delimiter            , s::error                                          >,
        row< s::init            , equal_sign           , s::error                                          >,
        row< s::equal_found     , delimiter            , s::error                                          >,
        row< s::delimiter_found , equal_sign           , s::error                                          >,
        row< s::delimiter_found , end                  , s::error                                          >,
        row< s::equal_found     , end                  , s::error                                          >,
        row< s::name_parsing    , value_add_token_char , s::error                                          >,
        row< s::delimiter_found , value_add_token_char , s::error                                          >,

        // Reset
        row< s::init            , rst                  , s::init                                           >,
        row< s::name_parsing    , rst                  , s::init                                           >,
        row< s::equal_found     , rst                  , s::init                                           >,
        row< s::value_parsing   , rst                  , s::init                                           >,
        row< s::delimiter_found , rst                  , s::init                                           >,
        row< s::error           , rst                  , s::init                                           >,
        row< s::done            , rst                  , s::init                                           >
    >;

    using callback_table_t = callback_table
    <
        scb< s::init, &p::on_init_enter >
    >;

    template<typename event_t>
    kv_parser_state process_event(const event_t& e)
    {
        return transition<event_t, transition_table_t, callback_table_t>(e);
    }

    bool is_char_in_set(const char c, const str_const& set)
    {
        for(auto& sc: set)
        {
            if(sc == c)
            {
                return true;
            }
        }

        return false;
    }

public:
    kv_parser_state start_parse(char*       uri_p_str,
                                uri_param*  params,
                                std::size_t max_params)
    {
        m_token            = uri_p_str;
        m_params           = params;
        m_params_count     = 0;
        m_max_params_count = max_params;

        process_event(rst());

        while(*uri_p_str != 0)
        {
            if(is_char_in_set(*uri_p_str, m_token_chars))
            {
                process_event(token_char());
            }
            else if(is_char_in_set(*uri_p_str, m_value_additional_token_chars))
            {
                process_event(value_add_token_char());
            }
            else if(is_char_in_set(*uri_p_str, m_equal_chars))
            {
                process_event(equal_sign(uri_p_str));
            }
            else if(is_char_in_set(*uri_p_str, m_delimiter_chars))
            {
                process_event(delimiter(uri_p_str));
            }
            else
            {
                // WTF? We don't know this character.
                process_event(rst());
                return state();
            }

            if(state() == kv_parser_state::error)
            {
                return state();
            }

            ++uri_p_str;
        }

        process_event(end());

        return state();
    }

private:
    // TODO: First key name symbol not digit!
    str_const m_token_chars                  { "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_" };
    str_const m_value_additional_token_chars { "%+.-" };
    str_const m_equal_chars                  { "="     };
    str_const m_delimiter_chars              { "&;"    };

    char*       m_token            { nullptr };
    uri_param*  m_params           { nullptr };
    std::size_t m_params_count     { 0       };
    std::size_t m_max_params_count { 0       };
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_PARSER_KEY_VALUE_PAIRS_FSM_HPP
