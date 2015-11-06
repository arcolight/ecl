#include <cstddef>

#include <functional>

#include <ecl/fsm.hpp>

namespace ecl
{

namespace web
{

enum class http_request_parser_state
{
    init                  ,

    parsing_method        ,
    parsing_method_done   ,

    parsing_uri           ,
    parsing_uri_done      ,

    parsing_version       ,
    parsing_version_done  ,

    parsing_header        ,
    parsing_header_done   ,

    waiting_for_eol       ,

    receiving_body        ,

    error                 ,
    error_parsing_method  ,
    error_parsing_uri     ,
    error_parsing_version ,
    error_parsing_header
};

struct start      {};
struct end_of_req {};
struct rst        {};

class http_request_parser : public state_machine
                                   <
                                       http_request_parser,
                                       http_request_parser_state,
                                       http_request_parser_state::init
                                   >
{
    http_request_parser(request& rq) :
        m_rq(rq)
    {}

// Guards
    bool g_parsing_method(const symbol& s)
    {
        if(/* good symbol */)
        {
            return true;
        }
    }

    bool g_error_parsing_method(const symbol& s)
    {
        return ! g_parsing_method(s);
    }

    using s = http_request_parser_state;
    using p = http_request_parser;

    using transition_table_t = transition_table
    <
        row< s::init                   , start      , s::parsing_method                                       >,

        row< s::parsing_method         , char       , s::parsing_method_done                                  >,
        row< s::parsing_method_done    , char       , s::parsing_uri          , nullptr , &p::g_is_space      >,

        row< s::parsing_uri            , char       , s::parsing_uri_done                                     >,
        row< s::parsing_uri_done       , char       , s::parsing_version      , nullptr , &p::g_is_space      >,

        row< s::parsing_version        , char       , s::parsing_version_done , nullptr , &p::g_check_version >,
        row< s::parsing_version_done   , char       , s::parsing_header       , nullptr , &p::g_is_eol        >,

        row< s::parsing_header         , char       , s::parsing_header_done                                  >,
        row< s::parsing_header_done    , char       , s::waiting_for_eol      , nullptr , &p::g_is_eol        >,
        row< s::waiting_for_eol        , char       , s::receiving_body                                       >,

        row< s::receiving_body         , char       , s::receiving_body                                       >,
        row< s::receiving_body         , end_of_req , s::init                                                 >,

// Errors
        row < s::error                 , rst        , s::init                                                 >,
        row < s::error_parsing_method  , rst        , s::init                                                 >,
        row < s::error_parsing_uri     , rst        , s::init                                                 >,
        row < s::error_parsing_version , rst        , s::init                                                 >,
        row < s::error_parsing_header  , rst        , s::init                                                 >
    >;

    using callback_table_t = callback_table
    <
        scb < s::init , &p::on_init_enter            >,
        scb < s::init , &p::on_parsing_method_enter  >,
        scb < s::init , &p::on_parsing_uri_enter     >,
        scb < s::init , &p::on_parsing_version_enter >,
        scb < s::init , &p::on_parsing_header_enter  >
    >;

    template<typename event_t>
    http_request_parser_state process_event(const event_t& e)
    {
        return transition<event_t, transition_table_t, callback_table_t>(e);
    }

public:
    void process_data(const char* data, std::size_t size)
    {
        for(std::size_t i = 0; i < size; ++i)
        {

        }
    }

private:
    request& m_rq;
};

} // namespace web

} // namespace ecl
