#ifndef ECL_WEB_I_RESOURCE_HPP
#define ECL_WEB_I_RESOURCE_HPP

#include <ecl/web/types.hpp>
#include <ecl/web/constants.hpp>
#include <ecl/web/i_request_cache.hpp>

namespace ecl
{

namespace web
{

template<typename ST>
struct i_resource
{
    using stream_t = ST;

    virtual              ~i_resource()                                  noexcept
    {}

    virtual status_code  on_request(ST&, i_request_cache&)         noexcept = 0;
};

template<typename ST>
struct i_static_resource : public i_resource<ST>
{
    using stream_t = typename i_resource<ST>::stream_t;

    virtual              ~i_static_resource()                           noexcept
    {}

    virtual content_type get_content_type()                        noexcept = 0;
    virtual status_code  get_status_code()                         noexcept = 0;
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_I_RESOURCE_HPP
