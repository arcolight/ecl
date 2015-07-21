#ifndef ECL_WEB_REQUEST_HPP
#define ECL_WEB_REQUEST_HPP

#include <ecl/web/constants.hpp>
#include <ecl/web/headers.hpp>

#ifndef MAX_HEADERS_COUNT
#define MAX_HEADERS_COUNT 64
// #pragma message "[ECL web/headers.hpp] Defaulting to Max headers coubtn = 64"
#endif

namespace ecl
{

namespace web
{

struct request
{
    request()
    {
        clear();
    }

    constexpr static std::size_t max_headers_count()
    {
        return MAX_HEADERS_COUNT;
    }

    method         met                         { method::GET  };
    version        ver                         { version::HTTP10 };
    const char*    uri                         { nullptr };
    const char*    uri_param                   { nullptr };
    const char*    body                        { nullptr };
    header         headers[MAX_HEADERS_COUNT];
    std::size_t    headers_count               { 0 };

    void clear()
    {
        met = method::GET;
        ver = version::HTTP10;

        uri = nullptr;
        body = nullptr;

        for(auto& h : headers)
        {
            h.clear();
        }

        headers_count = 0;
    }
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_REQUEST_HPP
