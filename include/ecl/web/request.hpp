#pragma once

#include "methods.hpp"
#include "version.hpp"
#include "headers.hpp"

namespace ecl
{

template<size_t MAX_URI_LEN             = 128,
         size_t MAX_HEADERS_COUNT       = 8,
         size_t MAX_HEADER_NAME_LENGTH  = DEFAULT_HEADER_NAME_LEN,
         size_t MAX_HEADER_VALUE_LENGTH = DEFAULT_HEADER_VALUE_LEN,
         size_t MAX_BODY_SIZE           = 1024>
struct request
{
    request()
    {
        clear();
    }

    typedef header<MAX_HEADER_NAME_LENGTH, MAX_HEADER_VALUE_LENGTH> header_t;

    constexpr static size_t max_uri_len()          { return MAX_URI_LEN; }
    constexpr static size_t max_headers_count()    { return MAX_HEADERS_COUNT; }
    constexpr static size_t max_header_name_len()  { return MAX_HEADER_NAME_LENGTH; }
    constexpr static size_t max_header_value_len() { return MAX_HEADER_VALUE_LENGTH; }
    constexpr static size_t max_body_size()        { return MAX_BODY_SIZE; }

    method   met                          { method::UNKNOWN  };
    version  ver                          { version::UNKNOWN };
    char     resource[MAX_URI_LEN   + 1];
    header_t hdrs[MAX_HEADERS_COUNT + 1];
    size_t   hdrs_count                   { 0 };
    uint8_t  body[MAX_BODY_SIZE];

    void clear()
    {
        met = method::UNKNOWN;
        ver = version::UNKNOWN;

        for(auto& c : resource)
        {
            c = 0x00;
        }

        for(auto& h : hdrs)
        {
            h.clear();
        }

        hdrs_count = 0;

        for(auto& c : body)
        {
            c = 0x00;
        }
    }
};

} // namespace ecl
