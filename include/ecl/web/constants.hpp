#pragma once

#include <map>

namespace ecl
{

namespace web
{

enum version
{
    HTTP10        = 0,
    HTTP11,
    HTTP20,
    VERSION_COUNT
};

typedef std::map<version, const char*> version_map_t;

version_map_t version_map = {
    { HTTP10, "HTTP/1.0" },
    { HTTP11, "HTTP/1.1" },
    { HTTP20, "HTTP/2.0" }
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

typedef std::map<status_code, const char*> status_code_map_t;

status_code_map_t status_code_map = {
    { CONTINUE,                   "CONTINUE"                   },
    { SWITCHING_PROTO,            "SWITCHING_PROTO"            },
    { OK,                         "OK"                         },
    { CREATED,                    "CREATED"                    },
    { ACCEPTED,                   "ACCEPTED"                   },
    { NO_CONTENT,                 "NO_CONTENT"                 },
    { RESET_CONTENT,              "RESET_CONTENT"              },
    { PARTIAL_CONTENT,            "PARTIAL_CONTENT"            },
    { BAD_REQUEST,                "BAD_REQUEST"                },
    { UNAUTHORIZED,               "UNAUTHORIZED"               },
    { PAYMENT_REQUIRED,           "PAYMENT_REQUIRED"           },
    { FORBIDDEN,                  "FORBIDDEN"                  },
    { NOT_FOUND,                  "NOT_FOUND"                  },
    { METHOD_NOT_ALLOWED,         "METHOD_NOT_ALLOWED"         },
    { NOT_ACCEPTABLE,             "NOT_ACCEPTABLE"             },
    { REQUEST_TIMEOUT,            "REQUEST_TIMEOUT"            },
    { CONFLICT,                   "CONFLICT"                   },
    { INTERNAL_SERVER_ERROR,      "INTERNAL_SERVER_ERROR"      },
    { NOT_IMPLEMENTED,            "NOT_IMPLEMENTED"            },
    { BAD_GATEWAY,                "BAD_GATEWAY"                },
    { SERVICE_UNAVAILABLE,        "SERVICE_UNAVAILABLE"        },
    { GATEWAY_TIMEOUT,            "GATEWAY_TIMEOUT"            },
    { HTTP_VERSION_NOT_SUPPORTED, "HTTP_VERSION_NOT_SUPPORTED" }
};

enum method
{
    GET,
    POST
};

typedef std::map<method, const char*> method_map_t;

method_map_t method_map = {
    { GET,  "GET"  },
    { POST, "POST" }
};

} // namespace web
    
} // namespace ecl

