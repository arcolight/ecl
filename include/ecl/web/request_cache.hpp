#ifndef ECL_WEB_REQUEST_CACHE_HPP
#define ECL_WEB_REQUEST_CACHE_HPP

#include <algorithm>

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

    virtual version get_ver()                                  noexcept override
    {
        return m_ver;
    }

    virtual url_t get_url()                                    noexcept override
    {
        return m_url;
    }

    virtual method get_met()                                   noexcept override
    {
        return m_met;
    }

    virtual header_value_t get_hdr(header_name_t&& num)        noexcept override
    {
        return m_headers[std::forward<header_name_t>(num)];
    }

    virtual void set_ver(version v)                            noexcept override
    {
        m_ver = v;
    }

    virtual void set_url(url_t u)                              noexcept override
    {
        m_url = u;
    }

    virtual void set_met(method m)                             noexcept override
    {
        m_met = m;
    }

    virtual void set_hdr(header_t h)                           noexcept override
    {
        m_headers.insert(h);
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
    char         m_rq_raw[CACHE_SIZE] {   };
    std::size_t  m_rq_raw_size        { 0 };

    version      m_ver                { version::HTTP11 };
    url_t        m_url                { "/"             };
    method       m_met                { method::GET     };
    header_map_t m_headers            {                 };
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_REQUEST_CACHE_HPP
