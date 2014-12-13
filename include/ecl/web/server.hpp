#pragma once

#include <array>

#include "resource_table.hpp"
#include "request_parser.hpp"
#include "response_composer.hpp"

#include "stream"

namespace ecl
{

namespace web
{

template<size_t IN_BUFFER_SIZE,
         size_t OUT_BUFFER_SIZE,
         typename RESOURCES>
class server
{
public:
    typedef std::array<uint8_t, IN_BUFFER_SIZE>  in_buffer_t;
    typedef std::array<uint8_t, OUT_BUFFER_SIZE> out_buffer_t;
    typedef RESOURCES                            resources_t;

    status_code process_request(size_t size)
    {
        m_out_buf.fill(0x00);

        request* req_ptr = m_parser.parse((char*)m_in_buf.data(), size);

        status_code code = status_code::BAD_REQUEST;
        version ver = version::HTTP11;

        if(nullptr != req_ptr)
        {
            status_code code = status_code::NOT_FOUND;
            ver = req_ptr->ver;

            const i_resource* res = resources_t::lookup(req_ptr->uri);
            if(nullptr != res)
            {
                code = res->exec(m_out_buf.data(), m_out_buf.size(), 0, nullptr);
            }
        }

        m_composer.compose(ver, code, m_out_buf.data(), m_out_buf.size(), m_out_buf.data(), m_out_buf.size());

        return code;
    }

    in_buffer_t& get_in_buffer()
    {
        return m_in_buf;
    }

    out_buffer_t& get_out_buffer()
    {
        return m_out_buf;
    }

private:
    in_buffer_t       m_in_buf;
    out_buffer_t      m_out_buf;

    resources_t       m_resources;
    request_parser    m_parser;
    response_composer m_composer;
};

} // namespace web
    
} // namespace ecl