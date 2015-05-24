#ifndef ECL_WEB_RESOURCE_TABLE
#define ECL_WEB_RESOURCE_TABLE

#include <cstring>
#include "request.hpp"

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
    bool call(T& st, const request* req)
    {
        if(nullptr == req)
        {
            this->PAGE_400::template exec<T>(st, nullptr);
            return false;
        }
        if(nullptr == req->uri)
        {
            this->PAGE_400::template exec<T>(st, nullptr);
            return false;
        }

        return call_internal<T, RESOURCES...>(st, req);
    }

private:
    template<typename T, typename RES, typename... TAIL>
    bool call_internal(T& st, const request* req)
    {
        if(RES::check_resource(req))
        {
            this->RES::template exec<T>(st, req);
            return true;
        }

        return call_internal<T, TAIL...>(st, req);
    }

    template<typename T>
    bool call_internal(T& st, const request* req)
    {
        this->PAGE_404::template exec<T>(st, req);

        return false;
    }
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_RESOURCE_TABLE
