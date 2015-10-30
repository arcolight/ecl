#ifndef ECL_WEB_REQUEST_PARSER_HPP
#define ECL_WEB_REQUEST_PARSER_HPP

#include <cstring>

#include <algorithm>

#include <ecl/web/parser_fsm.hpp>

#include <ecl/str_const.hpp>

namespace ecl
{

namespace web
{

template<std::size_t MAX_REQUEST_SIZE = 1024>
class request_parser
{
public:
    request* parse(request_raw_t raw)
    {
        memset(m_request_raw, 0, MAX_REQUEST_SIZE);

        parser_state st = m_parser.process_event(rst());

        std::size_t r_size = strlen(raw);

        if(r_size > MAX_REQUEST_SIZE)
        {
            return nullptr;
        }

        strncpy(m_request_raw, raw, MAX_REQUEST_SIZE - 1);

        char* current = m_request_raw;
        char* next    = nullptr;
        char* next_cr = nullptr;
        char* next_lf = nullptr;

        do
        {
            next    = strstr(current, m_crlf);
            next_cr = strstr(current, m_cr);
            next_lf = strstr(current, m_lf);

            if(nullptr == next_cr || nullptr == next_lf)
            {
                next = std::max(next_cr, next_lf);
            }

            if(nullptr == next)
            {
                st = m_parser.process_event(end_of_req());
                break;
            }

            bool is_empty_line = (next == current);

            *next = 0x00; ++next;
            *next = 0x00; ++next;

            if(is_empty_line)
            {
                st = m_parser.process_event(empty_line());
            }
            else
            {
                st = m_parser.process_event(event_line { current });
            }

            current = next;
        } while(st != parser_state::complete);

        if(st == parser_state::complete)
        {
            return m_parser.get_request_ptr();
        }

        return nullptr;
    }

private:
    char                       m_request_raw [MAX_REQUEST_SIZE];

    parser_fsm                 m_parser {};

    str_const m_crlf { "\r\n" };
    str_const m_cr   { "\r"   };
    str_const m_lf   { "\n"   };
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_REQUEST_PARSER_HPP
