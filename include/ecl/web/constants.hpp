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

const version_map_t version_map = {
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

const status_code_map_t status_code_map = {
    { CONTINUE,                   "CONTINUE"                   },
    { SWITCHING_PROTO,            "SWITCHING PROTO"            },
    { OK,                         "OK"                         },
    { CREATED,                    "CREATED"                    },
    { ACCEPTED,                   "ACCEPTED"                   },
    { NO_CONTENT,                 "NO CONTENT"                 },
    { RESET_CONTENT,              "RESET CONTENT"              },
    { PARTIAL_CONTENT,            "PARTIAL CONTENT"            },
    { BAD_REQUEST,                "BAD REQUEST"                },
    { UNAUTHORIZED,               "UNAUTHORIZED"               },
    { PAYMENT_REQUIRED,           "PAYMENT REQUIRED"           },
    { FORBIDDEN,                  "FORBIDDEN"                  },
    { NOT_FOUND,                  "NOT FOUND"                  },
    { METHOD_NOT_ALLOWED,         "METHOD NOT ALLOWED"         },
    { NOT_ACCEPTABLE,             "NOT ACCEPTABLE"             },
    { REQUEST_TIMEOUT,            "REQUEST TIMEOUT"            },
    { CONFLICT,                   "CONFLICT"                   },
    { INTERNAL_SERVER_ERROR,      "INTERNAL SERVER ERROR"      },
    { NOT_IMPLEMENTED,            "NOT IMPLEMENTED"            },
    { BAD_GATEWAY,                "BAD GATEWAY"                },
    { SERVICE_UNAVAILABLE,        "SERVICE UNAVAILABLE"        },
    { GATEWAY_TIMEOUT,            "GATEWAY TIMEOUT"            },
    { HTTP_VERSION_NOT_SUPPORTED, "HTTP VERSION NOT SUPPORTED" }
};

enum method
{
    GET,
    POST
};

typedef std::map<method, const char*> method_map_t;

const method_map_t method_map = {
    { GET,  "GET"  },
    { POST, "POST" }
};

} // namespace web
    
} // namespace ecl

