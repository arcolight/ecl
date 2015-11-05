#include <ecl/fsm.hpp>

namespace ecl
{

namespace web
{

enum class http_request_parser_state
{
    init,
    parsing_method,
    parsing_uri,
    parsing_version,
    parsing_header,
    receiving_body
};

struct symbol {};

class http_request_parser : public state_machine
                                   <
                                       http_request_parser,
                                       http_request_parser_state,
                                       http_request_parser_state::init
                                   >
{
    using s = http_request_parser_state;
    using p = http_request_parser;

    using transition_table_t = transition_table
    <
        row< s::init            , symbol     , s::parsing_method  >,
        row< s::parsing_method  , symbol     , s::parsing_uri     >,
        row< s::parsing_uri     , symbol     , s::parsing_version >,
        row< s::parsing_version , symbol     , s::parsing_header  >,
        row< s::parsing_header  , symbol     , s::receiving_body  >,
        row< s::receiving_body  , end_of_req , s::init            >
    >

public:
    void process_data(const char* data, std::size_t size)
    {

    }
};

} // namespace web

} // namespace ecl
