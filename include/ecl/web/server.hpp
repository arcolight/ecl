#pragma once

#include "request_parser.hpp"

#include <ecl/stream.hpp>

namespace ecl
{

namespace web
{

template<size_t STREAM_SIZE,
         typename RESOURCES,
         flush_function_t F>
class server
{
public:
    typedef RESOURCES              resources_t;
    typedef stream<STREAM_SIZE, F> stream_t;

    void process_request(char* req_raw, size_t size)
    {
        request* req_ptr = m_parser.parse(req_raw, size);

        if(nullptr != req_ptr)
        {
            write_version(req_ptr->ver);
            if( ! m_resources.template call<stream_t>(m_stream, req_ptr->uri, 0, nullptr))
            {
                write_status(status_code::NOT_FOUND);
            }
        }
        else
        {
            write_version(HTTP11);
            write_status(status_code::BAD_REQUEST);
        }


        m_stream.flush();
    }

private:
    void write_status(status_code code)
    {
        m_stream << (uint16_t)code << " " << constants::get_status_code(code);
    }

    void write_version(version ver)
    {
        m_stream << constants::get_version(ver) << " ";
    }

    stream_t       m_stream    {};
    resources_t    m_resources {};
    request_parser m_parser    {};
};

} // namespace web
    
} // namespace ecl