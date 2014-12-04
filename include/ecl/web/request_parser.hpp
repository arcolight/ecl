#pragma once

#include <cstring>

#include "request.hpp"
#include "i_request_parser.hpp"

namespace ecl
{

template<typename REQUEST>
class request_parser : public i_request_parser<REQUEST>
{
public:
    typedef REQUEST request_t;

    virtual ~request_parser() {}

    virtual request_t* parse(const char* raw_req, size_t size)          override
    {
        m_request.clear();

        uintptr_t offset = 0;

        if(!parse_request_line(raw_req + offset, size - offset, offset))
        {
            return nullptr;
        }

        if(!parse_headers(raw_req + offset, size - offset, offset))
        {
            return nullptr;
        }

        if(!parse_body(raw_req + offset, size - offset, offset))
        {
            return nullptr;
        }

        return &m_request;
    }

private:
    bool parse_request_line(const char* raw, size_t size, size_t& offset)
    {
        size_t rolled = roll_symbols(raw, " ", strlen(" "));
        size -= rolled;
        offset += rolled;

        if(!parse_method(raw + offset, size - offset, offset))
        {
            return false;
        }

        if(!parse_resource(raw + offset, size - offset, offset))
        {
            return false;
        }

        if(!parse_http_version(raw + offset, size - offset, offset))
        {
            return false;
        }

        return true;
    }

    bool parse_method(const char* raw, size_t size, size_t& offset)
    {
        size_t rolled = roll_symbols(raw, " ", strlen(" "));
        size -= rolled;
        offset += rolled;

        if(size < 3)
        {
            return false;
        }

        if(0 == strncmp("GET", raw, 3))
        {
            m_request.met = method::GET;
            offset += 3;
            return true;
        }

        if(0 == strncmp("POST", raw, 4))
        {
            m_request.met = method::POST;
            offset += 4;
            return true;
        }

        return false;
    }

    bool parse_resource(const char* raw, size_t size, size_t& offset)
    {
        size_t rolled = roll_symbols(raw, " ", strlen(" "));
        size -= rolled;
        offset += rolled;

        if(size == 0)
        {
            return false;
        }

        const char* const end_ptr = strchr(raw, ' ');

        if(nullptr == end_ptr)
        {
            return false;
        }

        size_t len = end_ptr - raw;

        strncpy(m_request.resource, raw, std::min(request_t::max_uri_len(), len));

        offset += len + 1;

        return true;
    }

    bool parse_http_version(const char* raw, size_t size, size_t& offset)
    {
        size_t rolled = roll_symbols(raw, " ", strlen(" "));
        size -= rolled;
        offset += rolled;

        if(size < std::min(strlen("HTTP/1.1"), strlen("HTTP/2.0")))
        {
            return false;
        }

        if(0 == strncmp("HTTP/1.1", raw, strlen("HTTP/1.1")))
        {
            m_request.ver = version::HTTP11;
            offset += strlen("HTTP/1.1");
            return true;
        }

        if(0 == strncmp("HTTP/2.0", raw, strlen("HTTP/2.0")))
        {
            m_request.ver = version::HTTP20;
            offset += strlen("HTTP/2.0");
            return true;
        }

        return false;
    }

    bool parse_headers(const char* raw, size_t size, size_t& offset)
    {
        size_t rolled = roll_symbols(raw, " \r\n", strlen(" \r\n"));
        size -= rolled;
        offset += rolled;

        const char* end_ptr_prev = raw;
        const char* end_ptr = nullptr;
        do
        {
            std::cout << "Before roll: " << end_ptr_prev << std::endl;
            offset += roll_symbols(end_ptr_prev, " \r\n", strlen(" \r\n"));
            std::cout << "After roll:  " << end_ptr_prev << std::endl;
            end_ptr = strchr(end_ptr_prev, '\r');
            std::cout << "Cur: " << end_ptr << std::endl;
            if(nullptr == end_ptr)
            {
                // We have finished
                return true;
            }

            size_t len = end_ptr - end_ptr_prev - 1;

            if(!m_request.hdrs[m_request.hdrs_count].parse(end_ptr_prev, len))
            {
                return false;
            }

            ++m_request.hdrs_count;
            offset += len;

            end_ptr_prev = end_ptr;
        } while(m_request.hdrs_count < request_t::max_headers_count());

        return true;
    }

    bool parse_body(const char* raw, size_t size, size_t& offset)
    {
        size_t rolled = roll_symbols(raw, " \r\n", strlen(" \r\n"));
        size -= rolled;
        offset += rolled;

        (void)(offset);
        return true;
    }

    size_t roll_symbols(const char*& ptr, 
                        const char* const symbols,
                        size_t len)                                        const
    {
        size_t count = 0;

        bool condition = false;

        do
        {
            for(size_t i = 0; i < len; ++i)
            {
                if(*ptr == symbols[i])
                {
                    condition = true;
                }
            }

            if(condition)
            {
                ++ptr;
                ++count;
                condition = false;
            }
        } while(condition);

        return count;
    }

    request_t m_request;
};

} // namespace ecl
