#ifndef ECL_WEB_RESOURCE_HPP
#define ECL_WEB_RESOURCE_HPP

#include <cstdint>
#include <cstring>

#include <algorithm>

#include <ecl/web/request.hpp>
#include <ecl/web/constants.hpp>

namespace ecl
{

namespace web
{

template<typename            RES_DATA,
         content_type_header TYPE,
         status_code         CODE,
         typename...         NAME
>
class resource
{
public:
    template<typename T>
    void exec(T& st, const request* req)                                   const
    {
        if(nullptr != req)
        {
            constants::write_status_line(st, req->ver, CODE);
        }
        else
        {
            constants::write_status_line(st, HTTP11, CODE);
        }

        st << constants::get_header_name(CONTENT_TYPE)
           << ":"
           << constants::get_content_type(TYPE) << "\r\n";
        st << "\r\n";
        st << RES_DATA::data << "\r\n";
    }

    static bool check_resource(const request* req)
    {
        return check_resource_internal<0, NAME...>(req);
    }

    template<std::size_t COUNT, typename N1, typename... TAIL>
    static bool check_resource_internal(const request* req)
    {
        if((0 == strncmp(req->uri, N1::name(), N1::size())) &&
           (strlen(req->uri) == N1::size()))
        {
            return true;
        }

        return check_resource_internal<COUNT + 1, TAIL...>(req);
    }

    template<std::size_t COUNT>
    static bool check_resource_internal(const request* req)
    {
        (void)(req);
        return false;
    }
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_RESOURCE_HPP
