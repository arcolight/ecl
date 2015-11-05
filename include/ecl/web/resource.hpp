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

template<typename     RES_DATA,
         content_type TYPE,
         bool         GZIP_COMPRESSED,
         status_code  CODE,
         typename...  NAME
>
class resource
{
public:
    template<typename T>
    status_code exec(T& st, const request* req)                            const
    {
        if(nullptr != req)
        {
            write_status_line(st, req->ver, CODE);
        }
        else
        {
            write_status_line(st, version::HTTP11, CODE);
        }

        set_content_type_header(st, TYPE);
        if(GZIP_COMPRESSED)
        {
            set_content_encoding_header(st, content_encoding::GZIP);
        }

        st << "\r\n";
        st << RES_DATA::data << "\r\n";

        return CODE;
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
