#ifndef ECL_WEB_REQUEST_PARSER_HPP
#define ECL_WEB_REQUEST_PARSER_HPP

#include <cstring>

#include <algorithm>

#include <ecl/web/parser_fsm.hpp>

namespace ecl
{

namespace web
{

class request_parser
{
public:
    request* parse(char* raw, std::size_t size)
    {
        (void)size;
        parser_state st = m_parser.process_event(rst());

        char* current = raw;
        char* next = nullptr;
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
    parser_fsm        m_parser {};

    const char* const m_crlf = "\r\n";
    const char* const m_cr   = "\r";
    const char* const m_lf   = "\n";
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_REQUEST_PARSER_HPP
