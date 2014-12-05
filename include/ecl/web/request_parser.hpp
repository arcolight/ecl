#pragma once

#include <cstring>

#include "i_request_parser.hpp"
#include "parser_fsm.hpp"

namespace ecl
{

namespace web 
{

class request_parser : public i_request_parser
{
public:
    virtual ~request_parser() {}

    virtual request* parse(char* raw, size_t size)                    override
    {
        (void)size;
        parser_state st = m_parser.process_event(rst());

        char* current = raw;
        char* next = nullptr;

        do
        {
            next = strstr(current, m_crlf);
            bool is_empty_line = (next == current);

            if(nullptr == next)
            {
                break;
            }

            *next = 0x00; ++next;
            *next = 0x00; ++next;

            if(is_empty_line)
            {
                st = m_parser.process_event(empty_line());
            }
            else
            {
                event_line e;
                e.line = current;
                st = m_parser.process_event(e);
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
    parser_fsm        m_parser;

    const char* const m_crlf = "\r\n";
};

} // namespace web

} // namespace ecl
