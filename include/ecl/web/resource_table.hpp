#ifndef ECL_WEB_RESOURCE_TABLE
#define ECL_WEB_RESOURCE_TABLE

#include <cstring>
#include <ecl/web/request.hpp>

namespace ecl
{

namespace web
{

template<
    typename    PAGE_400,
    typename    PAGE_404,
    typename    PAGE_500,
    typename... RESOURCES
>
class resource_table : public PAGE_400,
                       public PAGE_404,
                       public PAGE_500,
                       public RESOURCES...
{
public:
    template<typename T>
    status_code call(T& st, const request* req)
    {
        if(nullptr == req)
        {
            return this->PAGE_400::template exec<T>(st, nullptr);
        }

        if(nullptr == req->uri)
        {
            return this->PAGE_400::template exec<T>(st, nullptr);
        }

        return call_internal<T, RESOURCES...>(st, req);
    }

private:
    template<typename T, typename RES, typename... TAIL>
    status_code call_internal(T& st, const request* req)
    {
        if(RES::check_resource(req))
        {
            return this->RES::template exec<T>(st, req);
        }

        return call_internal<T, TAIL...>(st, req);
    }

    template<typename T>
    status_code call_internal(T& st, const request* req)
    {
        return this->PAGE_404::template exec<T>(st, req);
    }
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_RESOURCE_TABLE
