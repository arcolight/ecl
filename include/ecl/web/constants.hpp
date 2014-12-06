#pragma once

namespace ecl
{

namespace web
{

enum version
{
    HTTP10 = 0,
    HTTP11,
    HTTP20
};

template<typename K, typename V>
struct map_entry
{
    typedef K key_t;
    typedef V value_t;

    key_t   k;
    value_t v;
};

template<typename E>
struct static_map
{
    typedef E entry_t;

    template<std::size_t N>
    constexpr static_map(const entry_t(&a)[N]) : 
        map(a)
        {

        }

    entry_t map[];
};

// template<typename MAP>
// typename MAP::value_t

/*constexpr static map_entry<version, const char* const> version_map[] = */
constexpr static static_map<map_entry<version, const char* const> > map = 
{
    {
        { HTTP10, "HTTP/1.0" },
        { HTTP11, "HTTP/1.1" },
        { HTTP20, "HTTP/2.0" }
    }
};

enum status_code
{
    CONTINUE                   = 100,
    SWITCHING_PROTO            = 101,

    OK                         = 200,
    CREATED                    = 201,
    ACCEPTED                   = 202,

    NO_CONTENT                 = 204,
    RESET_CONTENT              = 205,
    PARTIAL_CONTENT            = 206,

    BAD_REQUEST                = 400,
    UNAUTHORIZED               = 401,
    PAYMENT_REQUIRED           = 402,
    FORBIDDEN                  = 403,
    NOT_FOUND                  = 404,
    METHOD_NOT_ALLOWED         = 405,
    NOT_ACCEPTABLE             = 406,
    REQUEST_TIMEOUT            = 408,
    CONFLICT                   = 409,

    INTERNAL_SERVER_ERROR      = 500,
    NOT_IMPLEMENTED            = 501,
    BAD_GATEWAY                = 502,
    SERVICE_UNAVAILABLE        = 503,
    GATEWAY_TIMEOUT            = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505
};

enum method
{
    GET,
    POST
};

} // namespace web
    
} // namespace ecl

