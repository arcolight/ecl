#ifndef ECL_WEB_REQUEST_HPP
#define ECL_WEB_REQUEST_HPP

#include <cstddef>

#include <ecl/web/constants.hpp>
#include <ecl/web/headers.hpp>
#include <ecl/web/uri_param.hpp>

namespace ecl
{

namespace web
{

using request_raw_t = const char*;

template
<
      std::size_t MAX_URI_LENGTH     = 128
    , std::size_t MAX_URI_PARAMETERS = 32
    , std::size_t MAX_HEADER_LENGTH  = 128
    , std::size_t MAX_HEADERS_COUNT  = 32
>
struct request
{
    void clear()
    {
        met = method::GET;
        ver = version::HTTP10;

        uri = nullptr;
        uri_param_string = nullptr;

        for(auto& h : headers)
        {
            h.clear();
        }

        for(auto& p : uri_parameters)
        {
            p.clear();
        }

        headers_count = 0;
    }

    request()
    {
        clear();
    }

    constexpr static std::size_t max_uri_length()                       noexcept
    {
        return MAX_URI_LENGTH;
    }

    constexpr static std::size_t max_uri_parameters_count()             noexcept
    {
        return MAX_URI_PARAMETERS;
    }

    constexpr static std::size_t max_header_length()                    noexcept
    {
        return MAX_HEADER_LENGTH;
    }

    constexpr static std::size_t max_headers_count()                    noexcept
    {
        return MAX_HEADERS_COUNT;
    }

    method         met                                 { method::GET     };
    version        ver                                 { version::HTTP10 };

    const char*    uri                                 { nullptr         };
    const char*    uri_param_string                    { nullptr         };

    uri_param      uri_parameters[MAX_URI_PARAMETERS];
    std::size_t    uri_parameters_count                { 0               };

    header         headers[MAX_HEADERS_COUNT];
    std::size_t    headers_count                       { 0               };
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_REQUEST_HPP
