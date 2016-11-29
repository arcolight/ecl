#ifndef ECL_WEB_SERVER_HPP
#define ECL_WEB_SERVER_HPP

#include <cstring>

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
public:
    using stream_t            = ecl::stream<OUT_STREAM_SIZE>;

    template<typename T>
    using resource_t          = static_resource<T, stream_t>;

    template<typename T>
    using static_resource_t   = static_resource<T, stream_t>;

    using i_resource_t        = i_resource<stream_t>;
    using i_static_resource_t = i_static_resource<stream_t>;

private:
    template<typename T>
    struct char_ptr_cmp
    {
        bool operator()(char const *a, char const *b)
        {
           return (std::strcmp(a, b) < 0);
        }
    };

    using request_cache_t = request_cache<CACHE_SIZE, HEADERS_COUNT>;
    using resources_map_t = ecl::map
                            <
                                  url_t
                                , i_resource_t*
                                , RESOURCES_COUNT
                                , char_ptr_cmp
                            >;

    using handlers_map_t = ecl::map<status_code, i_static_resource_t*, 40>;

    int on_message_begin()
    {
        m_cache.clear();
        return 0;
    }

    int on_url(const char* at, std::size_t length)
    {
        const_cast<char*>(at)[length] = 0;
        m_cache.set_url(at);

        return 0;
    }

    int on_status(const char*, std::size_t)
    {
        return 0;
    }

    int on_header_field(const char* at, std::size_t length)
    {
        const_cast<char*>(at)[length] = 0;
        m_hdr.first = at;

        return 0;
    }

    int on_header_value(const char* at, std::size_t length)
    {
        const_cast<char*>(at)[length] = 0;
        m_hdr.second = at;

        m_cache.set_hdr(m_hdr);

        return 0;
    }

    int on_headers_complete()
    {
        return 0;
    }

    int on_body(const char* at, std::size_t length)
    {
        const_cast<char*>(at)[length] = 0;
        m_cache.set_body(std::make_pair(at, length));

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

    void process_request(const char* buf, std::size_t buf_size)         noexcept
    {
        m_cache.cache(buf, buf_size);

        http_parser_execute(&m_parser,
                            &m_parser_settings,
                            m_cache.get_raw_rq(),
                            m_cache.get_raw_rq_size());
    }

    bool attach_resource(url_t url, i_resource_t& res)                  noexcept
    {
        return m_resources.insert(std::make_pair(url, &res)).second;
    }

    bool attach_handler(i_static_resource_t& handler)                   noexcept
    {
        return m_handlers.insert(std::make_pair
                                 (
                                       handler.get_status_code()
                                     , &handler)
                                 ).second;
    }

private:
    void call_resource()
    {
        i_resource_t* res = m_resources[m_cache.get_url(url_field::PATH)];

        status_code result = status_code::NOT_FOUND;
        if(nullptr != res)
        {
            result = res->on_request(m_stream, m_cache);
        }

        if(is_error(result))
        {
            if(nullptr != m_handlers[result])
            {
                m_handlers[result]->on_request(m_stream, m_cache);
            }
            else
            {
                write_status_line(m_stream, version::HTTP11, status_code::NOT_FOUND);
            }
        }
    }

    stream_t             m_stream          {};

    http_parser          m_parser          {};
    http_parser_settings m_parser_settings {};

    header_t             m_hdr             {};

    request_cache_t      m_cache           {};

    resources_map_t      m_resources       {};
    handlers_map_t       m_handlers        {};
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_SERVER_HPP
