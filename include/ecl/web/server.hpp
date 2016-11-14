#ifndef ECL_WEB_SERVER_HPP
#define ECL_WEB_SERVER_HPP

#include "http_parser.h"

#include <ecl/web/request_cache.hpp>
#include <ecl/web/resource.hpp>
#include <ecl/web/types.hpp>

#include <ecl/map.hpp>
#include <ecl/stream.hpp>

namespace ecl
{

namespace web
{

template
<
      std::size_t CACHE_SIZE      = 1536
    , std::size_t OUT_STREAM_SIZE = 1024
    , std::size_t HEADERS_COUNT   = 32
    , std::size_t RESOURCES_COUNT = 16
>
class server
{
    using request_cache_t = request_cache<CACHE_SIZE, HEADERS_COUNT>;
    using stream_t        = ecl::stream<OUT_STREAM_SIZE>;
    using i_resource_t    = i_resource<stream_t>;
    using resources_map_t = ecl::map<url_t, i_resource_t*, RESOURCES_COUNT>;

    int on_message_begin()
    {
        return 0;
    }

    int on_url(const char* at, std::size_t length)
    {
        m_cache.set_url(at);
        const_cast<char*>(at)[length] = 0;

        http_parser_url u;
        http_parser_parse_url(at, length, 1, &u);
        // for(int f = UF_SCHEMA; f < UF_MAX; ++f)
        // {
        //     if(u.field_set & (1 << f))
        //     {
        //         std::cout << "field " << f << " is set: " << std::string(at + u.field_data[f].off).substr(0, u.field_data[f].len) << std::endl;
        //     }
        // }

        return 0;
    }

    int on_status(const char*, std::size_t)
    {
        return 0;
    }

    int on_header_field(const char* at, std::size_t length)
    {
        m_hdr.first         = at;
        const_cast<char*>(at)[length] = 0;

        return 0;
    }

    int on_header_value(const char* at, std::size_t length)
    {
        m_hdr.second = at;
        const_cast<char*>(at)[length] = 0;

        m_cache.set_hdr(m_hdr);

        return 0;
    }

    int on_headers_complete()
    {
        return 0;
    }

    int on_body(const char*, std::size_t)
    {
        return 0;
    }

    int on_message_complete()
    {
        call_resource();
        return 0;
    }

    int on_chunk_header()
    {
        return 0;
    }

    int on_chunk_complete()
    {
        return 0;
    }

    static int on_message_begin_static    (http_parser* p)
    {
        return static_cast<server*>(p->data)->on_message_begin();
    }

    static int on_url_static              (http_parser* p, const char* at, std::size_t length)
    {
        return static_cast<server*>(p->data)->on_url(at, length);
    }

    static int on_status_static           (http_parser* p, const char* at, std::size_t length)
    {
        return static_cast<server*>(p->data)->on_status(at, length);
    }

    static int on_header_field_static     (http_parser* p, const char* at, std::size_t length)
    {
        return static_cast<server*>(p->data)->on_header_field(at, length);
    }

    static int on_header_value_static     (http_parser* p, const char* at, std::size_t length)
    {
        return static_cast<server*>(p->data)->on_header_value(at, length);
    }

    static int on_headers_complete_static (http_parser* p)
    {
        return static_cast<server*>(p->data)->on_headers_complete();
    }

    static int on_body_static             (http_parser* p, const char* at, std::size_t length)
    {
        return static_cast<server*>(p->data)->on_body(at, length);
    }

    static int on_message_complete_static (http_parser* p)
    {
        return static_cast<server*>(p->data)->on_message_complete();
    }

    static int on_chunk_header_static     (http_parser* p)
    {
        return static_cast<server*>(p->data)->on_chunk_header();
    }

    static int on_chunk_complete_static   (http_parser* p)
    {
        return static_cast<server*>(p->data)->on_chunk_complete();
    }

public:
    template<typename T>
    using resource_t = static_resource<T, stream_t>;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    server(send_callback_t cb)
        : m_stream { cb }
    {
#pragma GCC diagnostic pop
        http_parser_init(&m_parser, HTTP_REQUEST);

        m_parser.data = this;

        m_parser_settings.on_message_begin    = on_message_begin_static;
        m_parser_settings.on_url              = on_url_static;
        m_parser_settings.on_status           = on_status_static;
        m_parser_settings.on_header_field     = on_header_field_static;
        m_parser_settings.on_header_value     = on_header_value_static;
        m_parser_settings.on_headers_complete = on_headers_complete_static;
        m_parser_settings.on_body             = on_body_static;
        m_parser_settings.on_message_complete = on_message_complete_static;
        m_parser_settings.on_chunk_header     = on_chunk_header_static;
        m_parser_settings.on_chunk_complete   = on_chunk_complete_static;
    }

    void process_request(const char* buf, std::size_t buf_size)
    {
        m_cache.cache(buf, buf_size);

        http_parser_execute(&m_parser,
                            &m_parser_settings,
                            m_cache.get_raw_rq(),
                            m_cache.get_raw_rq_size());
    }

    bool attach_resource(url_t url, i_resource_t& res)
    {
        auto ret = m_resources.insert(std::make_pair(url, &res));
        return ret.second;
    }

private:
    void call_resource()
    {
        i_resource_t* res = m_resources[m_cache.get_url()];
        if(nullptr != res)
        {
            std::cout << ">>> size: " << res->size() << std::endl;
            std::cout << ">>> type: " << to_string(res->type()) << std::endl;
            res->on_request(m_stream, m_cache);
        }
        else
        {
            std::cout << m_cache.get_url() << " not found!" << std::endl;
        }
    }

    stream_t             m_stream          {};

    http_parser          m_parser          {};
    http_parser_settings m_parser_settings {};

    header_t             m_hdr             {};

    request_cache_t      m_cache           {};

    resources_map_t      m_resources       {};
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_SERVER_HPP
