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
        m_resources.template call<stream_t>(m_stream, m_parser.parse(req_raw, size));

        m_stream.flush();
    }

    stream_t       m_stream    {};
    resources_t    m_resources {};
    request_parser m_parser    {};
};

} // namespace web
    
} // namespace ecl