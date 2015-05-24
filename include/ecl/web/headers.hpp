#ifndef ECL_WEB_HEADERS
#define ECL_WEB_HEADERS

namespace ecl
{

namespace web
{

struct header
{
    char* name  { nullptr };
    char* value { nullptr };

    void clear()
    {
        name =  nullptr;
        value = nullptr;
    }
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_HEADERS
