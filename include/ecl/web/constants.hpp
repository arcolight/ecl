#ifndef ECL_WEB_CONSTANTS_HPP
#define ECL_WEB_CONSTANTS_HPP

#include <cstddef>

#include <ecl/str_const.hpp>

namespace ecl
{

namespace web
{

enum class version
{
    HTTP10 ,
    HTTP11 ,
    HTTP20
};

enum class status_code
{
    CONTINUE                      ,
    SWITCHING_PROTO               ,

    OK                            ,
    CREATED                       ,
    ACCEPTED                      ,
    NO_CONTENT                    ,
    RESET_CONTENT                 ,
    PARTIAL_CONTENT               ,

    MULTIPLE_CHOICES              ,
    MOVED_PERMANENTLY             ,
    FOUND                         ,
    SEE_OTHER                     ,
    NOT_MODIFIED                  ,
    USE_PROXY                     ,
    SWITCH_PROXY                  ,
    TEMPORARY_REDIRECT            ,
    PERMANENT_REDIRECT            ,

    BAD_REQUEST                   ,
    UNAUTHORIZED                  ,
    PAYMENT_REQUIRED              ,
    FORBIDDEN                     ,
    NOT_FOUND                     ,
    METHOD_NOT_ALLOWED            ,
    NOT_ACCEPTABLE                ,
    REQUEST_TIMEOUT               ,
    CONFLICT                      ,
    GONE                          ,
    LENGTH_REQUIRED               ,
    PRECONDITION_FAILED           ,
    REQUEST_ENTITY_TOO_LARGE      ,
    REQUEST_URI_TOO_LONG          ,
    UNSUPPORTED_MEDIA_TYPE        ,
    REQUEST_RANGE_NOT_SATISFIABLE ,
    EXPECTATION_FAILED            ,

    INTERNAL_SERVER_ERROR         ,
    NOT_IMPLEMENTED               ,
    BAD_GATEWAY                   ,
    SERVICE_UNAVAILABLE           ,
    GATEWAY_TIMEOUT               ,
    HTTP_VERSION_NOT_SUPPORTED
};

enum class method
{
    GET     ,
    HEAD    ,
    PUT     ,
    DELETE  ,
    POST    ,
    OPTIONS ,
    TRACE   ,
    CONNECT
};

enum class header_name
{
    CONTENT_TYPE     ,
    CONTENT_LENGTH   ,
    CONTENT_ENCODING ,
    ACCEPT_ENCODING  ,
    LOCATION
};

enum class content_type
{
    APPLICATION_JSON ,
    TEXT_HTML        ,
    TEXT_CSS         ,
    TEXT_JAVASCRIPT  ,
    IMAGE_PNG        ,
    IMAGE_JPEG       ,
    IMAGE_X_ICON     ,
    IMAGE_GIF        ,
    TEXT_PLAIN
};

enum class content_encoding
{
    GZIP
};

static str_const to_string(version v)                                   noexcept
{
    switch(v)
    {
        case version::HTTP10: return { "HTTP/1.0" };
        case version::HTTP11: return { "HTTP/1.1" };
        case version::HTTP20: return { "HTTP/2.0" };
    }
    return { "" };
}

static str_const to_string(status_code c)                               noexcept
{
    switch(c)
    {
        case status_code::CONTINUE:                      return { "CONTINUE"                      };
        case status_code::SWITCHING_PROTO:               return { "SWITCHING_PROTO"               };

        case status_code::OK:                            return { "OK"                            };
        case status_code::CREATED:                       return { "CREATED"                       };
        case status_code::ACCEPTED:                      return { "ACCEPTED"                      };
        case status_code::NO_CONTENT:                    return { "NO_CONTENT"                    };
        case status_code::RESET_CONTENT:                 return { "RESET_CONTENT"                 };
        case status_code::PARTIAL_CONTENT:               return { "PARTIAL_CONTENT"               };

        case status_code::MULTIPLE_CHOICES:              return { "MULTIPLE_CHOICES"              };
        case status_code::MOVED_PERMANENTLY:             return { "MOVED_PERMANENTLY"             };
        case status_code::FOUND:                         return { "FOUND"                         };
        case status_code::SEE_OTHER:                     return { "SEE_OTHER"                     };
        case status_code::NOT_MODIFIED:                  return { "NOT_MODIFIED"                  };
        case status_code::USE_PROXY:                     return { "USE_PROXY"                     };
        case status_code::SWITCH_PROXY:                  return { "SWITCH_PROXY"                  };
        case status_code::TEMPORARY_REDIRECT:            return { "TEMPORARY_REDIRECT"            };
        case status_code::PERMANENT_REDIRECT:            return { "PERMANENT_REDIRECT"            };

        case status_code::BAD_REQUEST:                   return { "BAD_REQUEST"                   };
        case status_code::UNAUTHORIZED:                  return { "UNAUTHORIZED"                  };
        case status_code::PAYMENT_REQUIRED:              return { "PAYMENT_REQUIRED"              };
        case status_code::FORBIDDEN:                     return { "FORBIDDEN"                     };
        case status_code::NOT_FOUND:                     return { "NOT_FOUND"                     };
        case status_code::METHOD_NOT_ALLOWED:            return { "METHOD_NOT_ALLOWED"            };
        case status_code::NOT_ACCEPTABLE:                return { "NOT_ACCEPTABLE"                };
        case status_code::REQUEST_TIMEOUT:               return { "REQUEST_TIMEOUT"               };
        case status_code::CONFLICT:                      return { "CONFLICT"                      };
        case status_code::GONE:                          return { "GONE"                          };
        case status_code::LENGTH_REQUIRED:               return { "LENGTH_REQUIRED"               };
        case status_code::PRECONDITION_FAILED:           return { "PRECONDITION_FAILED"           };
        case status_code::REQUEST_ENTITY_TOO_LARGE:      return { "REQUEST_ENTITY_TOO_LARGE"      };
        case status_code::REQUEST_URI_TOO_LONG:          return { "REQUEST_URI_TOO_LONG"          };
        case status_code::UNSUPPORTED_MEDIA_TYPE:        return { "UNSUPPORTED_MEDIA_TYPE"        };
        case status_code::REQUEST_RANGE_NOT_SATISFIABLE: return { "REQUEST_RANGE_NOT_SATISFIABLE" };
        case status_code::EXPECTATION_FAILED:            return { "EXPECTATION_FAILED"            };

        case status_code::INTERNAL_SERVER_ERROR:         return { "INTERNAL_SERVER_ERROR"         };
        case status_code::NOT_IMPLEMENTED:               return { "NOT_IMPLEMENTED"               };
        case status_code::BAD_GATEWAY:                   return { "BAD_GATEWAY"                   };
        case status_code::SERVICE_UNAVAILABLE:           return { "SERVICE_UNAVAILABLE"           };
        case status_code::GATEWAY_TIMEOUT:               return { "GATEWAY_TIMEOUT"               };
        case status_code::HTTP_VERSION_NOT_SUPPORTED:    return { "HTTP_VERSION_NOT_SUPPORTED"    };
    }
    return { "" };
}

static str_const to_string(method m)                                    noexcept
{
    switch(m)
    {
        case method::GET:     return { "GET"     };
        case method::HEAD:    return { "HEAD"    };
        case method::PUT:     return { "PUT"     };
        case method::DELETE:  return { "DELETE"  };
        case method::POST:    return { "POST"    };
        case method::OPTIONS: return { "OPTIONS" };
        case method::TRACE:   return { "TRACE"   };
        case method::CONNECT: return { "CONNECT" };
    }
    return { "" };
}

static str_const to_string(header_name n)                               noexcept
{
    switch(n)
    {
        case header_name::CONTENT_TYPE:     return { "Content-Type"     };
        case header_name::CONTENT_LENGTH:   return { "Content-Length"   };
        case header_name::CONTENT_ENCODING: return { "Content-Encoding" };
        case header_name::ACCEPT_ENCODING:  return { "Accept-Encoding"  };
        case header_name::LOCATION:         return { "Location"         };
    }
    return { "" };
}

static str_const to_string(content_type t)                              noexcept
{
    switch(t)
    {
        case content_type::APPLICATION_JSON: return { "application/json" };
        case content_type::TEXT_HTML:        return { "text/html"        };
        case content_type::TEXT_CSS:         return { "text/css"         };
        case content_type::TEXT_JAVASCRIPT:  return { "text/javascript"  };
        case content_type::IMAGE_PNG:        return { "image/png"        };
        case content_type::IMAGE_JPEG:       return { "image/jpeg"       };
        case content_type::IMAGE_X_ICON:     return { "image/x-icon"     };
        case content_type::IMAGE_GIF:        return { "image/gif"        };
        case content_type::TEXT_PLAIN:       return { "text/plain"       };
    }
    return { "" };
}

static str_const to_string(content_encoding e)                          noexcept
{
    switch(e)
    {
        case content_encoding::GZIP: return { "gzip" }; break;
    }
    return { "" };
}

static int to_int(status_code c)                                        noexcept
{
    switch(c)
    {
        case status_code::CONTINUE:                      return 100;
        case status_code::SWITCHING_PROTO:               return 101;

        case status_code::OK:                            return 200;
        case status_code::CREATED:                       return 201;
        case status_code::ACCEPTED:                      return 202;
        case status_code::NO_CONTENT:                    return 204;
        case status_code::RESET_CONTENT:                 return 205;
        case status_code::PARTIAL_CONTENT:               return 206;

        case status_code::MULTIPLE_CHOICES:              return 300;
        case status_code::MOVED_PERMANENTLY:             return 301;
        case status_code::FOUND:                         return 302;
        case status_code::SEE_OTHER:                     return 303;
        case status_code::NOT_MODIFIED:                  return 304;
        case status_code::USE_PROXY:                     return 305;
        case status_code::SWITCH_PROXY:                  return 306;
        case status_code::TEMPORARY_REDIRECT:            return 307;
        case status_code::PERMANENT_REDIRECT:            return 308;

        case status_code::BAD_REQUEST:                   return 400;
        case status_code::UNAUTHORIZED:                  return 401;
        case status_code::PAYMENT_REQUIRED:              return 402;
        case status_code::FORBIDDEN:                     return 403;
        case status_code::NOT_FOUND:                     return 404;
        case status_code::METHOD_NOT_ALLOWED:            return 405;
        case status_code::NOT_ACCEPTABLE:                return 406;
        case status_code::REQUEST_TIMEOUT:               return 408;
        case status_code::CONFLICT:                      return 409;
        case status_code::GONE:                          return 410;
        case status_code::LENGTH_REQUIRED:               return 411;
        case status_code::PRECONDITION_FAILED:           return 412;
        case status_code::REQUEST_ENTITY_TOO_LARGE:      return 413;
        case status_code::REQUEST_URI_TOO_LONG:          return 414;
        case status_code::UNSUPPORTED_MEDIA_TYPE:        return 415;
        case status_code::REQUEST_RANGE_NOT_SATISFIABLE: return 416;
        case status_code::EXPECTATION_FAILED:            return 417;

        case status_code::INTERNAL_SERVER_ERROR:         return 500;
        case status_code::NOT_IMPLEMENTED:               return 501;
        case status_code::BAD_GATEWAY:                   return 502;
        case status_code::SERVICE_UNAVAILABLE:           return 503;
        case status_code::GATEWAY_TIMEOUT:               return 504;
        case status_code::HTTP_VERSION_NOT_SUPPORTED:    return 505;
    }
    return 0;
}

template<typename T>
static void write_status(T& st, status_code code)
{
    st << to_int(code) << " " << to_string(code);
}

template<typename T>
static void write_version(T& st, version ver)
{
    st << to_string(ver) << " ";
}

template<typename T>
static void write_status_line(T& st, version ver, status_code code)
{
    write_version<T>(st, ver);
    write_status<T>(st, code);
    st << "\r\n";
}

template<typename T>
void set_content_type_header(T& st, content_type t)
{
    st << to_string(header_name::CONTENT_TYPE)
       << ":"
       << to_string(t) << "\r\n";
}

template<typename T>
void set_content_encoding_header(T& st, content_encoding e)
{
    st << to_string(header_name::CONTENT_ENCODING)
       << ":"
       << to_string(e) << "\r\n";
}

template<typename T>
static void redirect(T& st, const char* location, version ver)
{
    ecl::web::write_status_line(st, ver, ecl::web::status_code::SEE_OTHER);

    st << ecl::web::to_string(ecl::web::header_name::LOCATION)
       << ":" << location << "\r\n" << "\r\n";
}

} // namespace web

} // namespace ecl

#endif // ECL_WEB_CONSTANTS_HPP
