#pragma once

#include "methods.hpp"
#include "version.hpp"
#include "headers.hpp"

#ifndef MAX_HEADERS_COUNT
#define MAX_HEADERS_COUNT 64
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

    constexpr static size_t max_headers_count() { return MAX_HEADERS_COUNT; }

    method         met                         { method::UNKNOWN  };
    version        ver                         { version::UNKNOWN };
    const char*    uri                         { nullptr };
    const uint8_t* body                        { nullptr };
    header         headers[MAX_HEADERS_COUNT];
    size_t         headers_count               { 0 };

    void clear()
    {
        met = method::UNKNOWN;
        ver = version::UNKNOWN;

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
