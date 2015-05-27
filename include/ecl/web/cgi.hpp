#ifndef ECL_WEB_CGI_HPP
#define ECL_WEB_CGI_HPP

#include <ecl/web/request.hpp>

namespace ecl
{

namespace web
{

template<typename... NAME>
class cgi
{
public:
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

#endif // ECL_WEB_CGI_HPP
