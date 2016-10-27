#ifndef ECL_WEB_REQUEST_CACHE_HPP
#define ECL_WEB_REQUEST_CACHE_HPP

#include <algorithm>

#include "http_parser.h"

#include <ecl/web/types.hpp>
#include <ecl/web/constants.hpp>
#include <ecl/web/i_request_cache.hpp>

#include <ecl/map.hpp>

namespace ecl
{

namespace web
{

template
<
      std::size_t CACHE_SIZE    = 1536
    , std::size_t HEADERS_COUNT = 32
>
struct request_cache : public i_request_cache
{
private:
    using header_map_t = ecl::map<header_name_t, header_value_t, HEADERS_COUNT>;

public:
    using url_schema_map_t = ecl::map
                             <
                                   url_field
                                 , url_field_t
                                 , UF_MAX
                             >;

    ~request_cache()                                                    noexcept
    {
    }

    virtual std::size_t cache(const char* buf,
                              std::size_t length)              noexcept override
    {
        m_rq_raw_size = std::min(length, CACHE_SIZE);
        memcpy(m_rq_raw, buf, m_rq_raw_size);

        return m_rq_raw_size;
    }

    virtual void clear()                                       noexcept override
    {
        m_ver = version::HTTP11;
        m_url = "/";
        m_met = method::GET;
        m_headers.clear();
        m_body = { nullptr, 0 };
        m_url_fields.clear();
    }

    virtual version get_ver()                                  noexcept override
    {
        return m_ver;
    }

    virtual url_t get_url()                                    noexcept override
    {
        return m_url;
    }

    virtual url_field_t get_url(url_field f)                   noexcept override
    {
        return m_url_fields[f];
    }

    virtual method get_met()                                   noexcept override
    {
        return m_met;
    }

    virtual header_value_t get_hdr(header_name_t num)          noexcept override
    {
        return m_headers[num];
    }

    virtual body_t get_body()                                  noexcept override
    {
        return m_body;
    }

    virtual void set_ver(version v)                            noexcept override
    {
        m_ver = v;
    }

    virtual void set_url(url_t u)                              noexcept override
    {
        m_url = u;

        m_url_fields.clear();

        http_parser_url parser_url;
        http_parser_url_init(&parser_url);
        http_parser_parse_url(u, std::strlen(m_url), 0, &parser_url);

        for(int f = UF_SCHEMA; f < UF_MAX; ++f)
        {
            if(parser_url.field_set & (1 << f))
            {
                const char* p = m_url + parser_url.field_data[f].off;
                m_url_fields[to_url_field(f)] = p;
                const_cast<char*>(p)[parser_url.field_data[f].len] = 0;
            }
        }
    }

    virtual void set_met(method m)                             noexcept override
    {
        m_met = m;
    }

    virtual void set_hdr(header_t h)                           noexcept override
    {
        m_headers.insert(h);
    }

    virtual void set_body(body_t b)                            noexcept override
    {
        m_body = b;
    }

    char* get_raw_rq()                                                  noexcept
    {
        return m_rq_raw;
    }

    std::size_t get_raw_rq_size()                                       noexcept
    {
        return m_rq_raw_size;
    }

private:
    char             m_rq_raw[CACHE_SIZE] {};
    std::size_t      m_rq_raw_size        { 0 };

    version          m_ver                { version::HTTP11 };
    url_t            m_url                { "/" };
    method           m_met                { method::GET };
    header_map_t     m_headers            {};
    body_t           m_body               {};

    url_schema_map_t m_url_fields         {};
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_REQUEST_CACHE_HPP
