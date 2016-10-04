#ifndef ECL_WEB_I_RESOURCE_HPP
#define ECL_WEB_I_RESOURCE_HPP

namespace ecl
{

namespace web
{

struct i_resource
{
    virtual ~i_resource() {}
    virtual bool on_request(i_request_cache&)                               = 0;
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_I_RESOURCE_HPP
