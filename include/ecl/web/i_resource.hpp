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
    virtual                ~i_resource() {}
    virtual bool           on_request(ST&, i_request_cache&)                = 0;
    virtual const uint8_t* data()                                           = 0;
    virtual std::size_t    size()                                           = 0;
    virtual bool           compressed()                                     = 0;
    virtual content_type   type()                                           = 0;
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_I_RESOURCE_HPP
