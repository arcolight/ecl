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
    const request* parse(request_raw_t raw, size_t size)
    {
        memset(m_request_raw, 0, MAX_REQUEST_SIZE);

        if(size > MAX_REQUEST_SIZE)
        {
            return nullptr;
        }

        memcpy(m_request_raw, raw, size);

        parser_state st = m_parser.process_event(rst());

        char* current = m_request_raw;
        char* next    = nullptr;

        do
        {
            next = strchr(current, m_lf);

            if(nullptr != next)
            {
                if(*(next - 1) == m_cr)
                {
                    *(next - 1) = 0;
                }

                *next = 0;
                ++next;
            }

            bool is_empty_line = (0 == strlen(current));

//            std::cout << "Current   " << current << std::endl;
//            std::cout << "Next      " << next    << std::endl;
//            std::cout << "Empy line " << is_empty_line << std::endl;
//            std::cout << "-----" << std::endl;

            if(is_empty_line)
            {
                st = m_parser.process_event(empty_line());
            }
            else
            {
                st = m_parser.process_event(event_line { current });
            }

            if(nullptr == next)
            {
                st = m_parser.process_event(end_of_req());
                break;
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

    const char m_cr   { '\r' };
    const char m_lf   { '\n' };
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_REQUEST_PARSER_HPP
