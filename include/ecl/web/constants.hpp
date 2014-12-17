#pragma once

namespace ecl
{

namespace web
{

enum version
{
    HTTP10        = 0,
    HTTP11,
    HTTP20
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

struct version_str
{

};

struct constants
{
    // typedef std::map<version, const char*>     version_map_t;
    // typedef std::map<status_code, const char*> status_code_map_t;
    // typedef std::map<method, const char*>      method_map_t;

    static const char* get_version(version v)
    {
        switch(v)
        {
            case HTTP10: return "HTTP/1.0"; break;
            case HTTP11: return "HTTP/1.1"; break;
            case HTTP20: return "HTTP/2.0"; break;
            default:     return "UNKNOWN";  break;
        }
    }

    static const char* get_status_code(status_code code)
    {
        switch(code)
        {
            case CONTINUE:                   return "CONTINUE";                   break;
            case SWITCHING_PROTO:            return "SWITCHING PROTO";            break;
            case OK:                         return "OK";                         break;
            case CREATED:                    return "CREATED";                    break;
            case ACCEPTED:                   return "ACCEPTED";                   break;
            case NO_CONTENT:                 return "NO CONTENT";                 break;
            case RESET_CONTENT:              return "RESET CONTENT";              break;
            case PARTIAL_CONTENT:            return "PARTIAL CONTENT";            break;
            case BAD_REQUEST:                return "BAD REQUEST";                break;
            case UNAUTHORIZED:               return "UNAUTHORIZED";               break;
            case PAYMENT_REQUIRED:           return "PAYMENT REQUIRED";           break;
            case FORBIDDEN:                  return "FORBIDDEN";                  break;
            case NOT_FOUND:                  return "NOT FOUND";                  break;
            case METHOD_NOT_ALLOWED:         return "METHOD NOT ALLOWED";         break;
            case NOT_ACCEPTABLE:             return "NOT ACCEPTABLE";             break;
            case REQUEST_TIMEOUT:            return "REQUEST TIMEOUT";            break;
            case CONFLICT:                   return "CONFLICT";                   break;
            case INTERNAL_SERVER_ERROR:      return "INTERNAL SERVER ERROR";      break;
            case NOT_IMPLEMENTED:            return "NOT IMPLEMENTED";            break;
            case BAD_GATEWAY:                return "BAD GATEWAY";                break;
            case SERVICE_UNAVAILABLE:        return "SERVICE UNAVAILABLE";        break;
            case GATEWAY_TIMEOUT:            return "GATEWAY TIMEOUT";            break;
            case HTTP_VERSION_NOT_SUPPORTED: return "HTTP VERSION NOT SUPPORTED"; break;
            default:                         return "UNKNOWN";                    break;
        }
    }

    static const char* get_method(method m)
    {
        switch(m)
        {
            case GET:  return "GET";     break;
            case POST: return "POST";    break;
            default:   return "UNKNOWN"; break;
        }
    }
};

} // namespace web
    
} // namespace ecl

