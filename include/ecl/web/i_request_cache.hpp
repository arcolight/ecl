#ifndef ECL_WEB_I_REQUEST_CACHE_HPP
#define ECL_WEB_I_REQUEST_CACHE_HPP

#include <ecl/web/types.hpp>
#include <ecl/web/constants.hpp>

#include <utility>

namespace ecl
{

namespace web
{

struct i_request_cache
{
    virtual ~i_request_cache() {}

    virtual std::size_t    cache( const char*, std::size_t )       noexcept = 0;
    virtual void           clear()                                 noexcept = 0;

    virtual version        get_ver ()                              noexcept = 0;
    virtual url_t          get_url ()                              noexcept = 0;
    virtual url_field_t    get_url ( url_field )                   noexcept = 0;
    virtual method         get_met ()                              noexcept = 0;
    virtual header_value_t get_hdr ( header_name_t )               noexcept = 0;
    virtual body_t         get_body()                              noexcept = 0;

    virtual void           set_ver ( version  )                    noexcept = 0;
    virtual void           set_url ( url_t    )                    noexcept = 0;
    virtual void           set_met ( method   )                    noexcept = 0;
    virtual void           set_hdr ( header_t )                    noexcept = 0;
    virtual void           set_body( body_t   )                    noexcept = 0;
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_I_REQUEST_CACHE_HPP
