#pragma once

#include <cstdint>
#include <cstring>

#include <algorithm>
#include <tuple>

#include <ecl/sized_data.h>
#include "request.hpp"
#include "constants.hpp"

namespace ecl
{

namespace web
{

template<size_t              SIZE,
         const uint8_t     (&DATA)[SIZE],
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
        st << const_sized_data { DATA, SIZE } << "\r\n";
    }

    static bool check_resource(const request* req)
    {
        return check_resource_internal<0, NAME...>(req);
    }

    template<size_t COUNT, typename N1, typename... TAIL>
    static bool check_resource_internal(const request* req)
    {
        if((0 == strncmp(req->uri, N1::name(), N1::size())) &&
           (strlen(req->uri) == N1::size()))
        {
            return true;
        }

        return check_resource_internal<COUNT + 1, TAIL...>(req);
    }

    template<size_t COUNT>
    static bool check_resource_internal(const request* req)
    {
        (void)(req);
        return false;        
    }
};

} // namespace web

} // namespace ecl
