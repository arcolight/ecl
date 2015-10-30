#ifndef ECL_WEB_URI_PARAM
#define ECL_WEB_URI_PARAM

namespace ecl
{

namespace web
{

struct uri_param
{
    char* name  { nullptr };
    char* value { nullptr };

    void clear()                                                        noexcept
    {
        name =  nullptr;
        value = nullptr;
    }
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_URI_PARAM
