#ifndef ECL_WEB_PARSER_URI_PARAM_FSM_HPP
#define ECL_WEB_PARSER_URI_PARAM_FSM_HPP

#include <ecl/fsm.hpp>
#include <ecl/str_const.hpp>

#include <ecl/web/uri_param.hpp>

namespace ecl
{

namespace web
{

enum class uri_parameters_parser_state
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
struct equal_sign           {};
struct delimiter            {};
struct end                  {};

class uri_parameter_parser_fsm : public state_machine
                                        <
                                            uri_parameter_parser_fsm,
                                            uri_parameters_parser_state,
                                            uri_parameters_parser_state::init
                                        >
{
    void on_init_enter()
    {

    }

    using s = uri_parameters_parser_state;
    using p = uri_parameter_parser_fsm;

    using transition_table_t = transition_table<
        // Normal ways
        row< s::init            , token_char           , s::name_parsing    >,
        row< s::init            , end                  , s::done            >,

        row< s::name_parsing    , token_char           , s::name_parsing    >,
        row< s::name_parsing    , equal_sign           , s::equal_found     >,
        row< s::name_parsing    , delimiter            , s::delimiter_found >,

        row< s::equal_found     , token_char           , s::value_parsing    >,

        row< s::value_parsing   , token_char           , s::value_parsing   >,
        row< s::value_parsing   , value_add_token_char , s::value_parsing   >,
        row< s::value_parsing   , delimiter            , s::delimiter_found >,
        row< s::value_parsing   , end                  , s::done            >,

        row< s::delimiter_found , token_char           , s::name_parsing    >,

        // Errors
        row< s::init            , delimiter            , s::error           >,
        row< s::init            , equal_sign           , s::error           >,
        row< s::equal_found     , delimiter            , s::error           >,
        row< s::delimiter_found , equal_sign           , s::error           >,
        row< s::delimiter_found , end                  , s::error           >,
        row< s::name_parsing    , value_add_token_char , s::error           >,
        row< s::delimiter_found , value_add_token_char , s::error           >,

        // Reset
        row< s::init            , rst                  , s::init            >,
        row< s::name_parsing    , rst                  , s::init            >,
        row< s::equal_found     , rst                  , s::init            >,
        row< s::value_parsing   , rst                  , s::init            >,
        row< s::delimiter_found , rst                  , s::init            >
    >;

    using callback_table_t = callback_table<
        scb< s::init, &p::on_init_enter >
    >;

    template<typename event_t>
    uri_parameters_parser_state process_event(const event_t& e)
    {
        return transition<event_t, transition_table_t, callback_table_t>(e);
    }

    bool included_in_set(const char c, const str_const& set)
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

    uri_parameters_parser_state start_parse(char*       uri_p_str,
                                            uri_param*  params,
                                            std::size_t max_params)
    {
        m_token            = uri_p_str;
        m_params           = params;
        m_params_count     = 0;
        m_max_params_count = max_params;

        while(*uri_p_str != 0)
        {
            if(included_in_set(*uri_p_str, m_token_chars))
            {
                process_event(token_char());
            }
            else if(included_in_set(*uri_p_str, m_value_additioanl_token_chars))
            {
                process_event(value_add_token_char());
            }
            else if(included_in_set(*uri_p_str, m_equal_chars))
            {
                process_event(equal_sign());
            }
            else if(included_in_set(*uri_p_str, m_delimiter_chars))
            {
                process_event(delimiter());
            }
            else
            {
                // WTF? We don't know this character.
                process_event(rst());
                return state();
            }

            ++uri_p_str;
        }

        process_event(end());
        return state();
    }

    // TODO: First key name symbol not digit!
    str_const m_token_chars                  { "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" };
    str_const m_value_additioanl_token_chars { "%+._-" };
    str_const m_equal_chars                  { "=" };
    str_const m_delimiter_chars              { "&;" };

    char*       m_token            { nullptr };
    uri_param*  m_params           { nullptr };
    std::size_t m_params_count     { 0       };
    std::size_t m_max_params_count { 0       };
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_PARSER_URI_PARAM_FSM_HPP
