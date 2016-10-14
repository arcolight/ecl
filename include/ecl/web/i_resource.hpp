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

    virtual             ~i_resource() {}
    virtual status_code on_request(ST&, i_request_cache&)                   = 0;
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_I_RESOURCE_HPP
