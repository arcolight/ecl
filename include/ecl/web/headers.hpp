#ifndef ECL_WEB_HEADERS
#define ECL_WEB_HEADERS

namespace ecl
{

namespace web
{

struct header
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

#endif // ECL_WEB_HEADERS
