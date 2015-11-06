#ifndef ECL_WEB_URI_PARAM
#define ECL_WEB_URI_PARAM

namespace ecl
{

namespace web
{

struct uri_param
{
    const char* name  { "" };
    const char* value { "" };

    void clear()                                                        noexcept
    {
        name  = "";
        value = "";
    }
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_URI_PARAM
