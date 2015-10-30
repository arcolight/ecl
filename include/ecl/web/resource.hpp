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
    request_raw_t exec(T& st, request* req)                                const
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

        return nullptr;
    }

    template<typename N>
    static bool check_resource_internal(const request* req)
    {
        if((0 == strncmp(req->uri, N::name(), N::size())) &&
           (strlen(req->uri) == N::size())                &&
           (method::GET == req->met)
        )
        {
            return true;
        }

        return false;
    }

    template<typename N1, typename N2, typename... TAIL>
    static bool check_resource_internal(const request* req)
    {
        if((0 == strncmp(req->uri, N1::name(), N1::size())) &&
           (strlen(req->uri) == N1::size())                 &&
           (method::GET == req->met))
        {
            return true;
        }

        return check_resource_internal<N2, TAIL...>(req);
    }

    static bool check_resource(const request* req)
    {
        return check_resource_internal<NAME...>(req);
    }
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_RESOURCE_HPP
