#pragma once

namespace ecl
{

namespace web
{

enum version
{
    HTTP10,
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
    GET  = 0x00,
    POST = 0x01,
    PUT  = 0x02
};

enum header_name
{
    CONTENT_TYPE,
    CONTENT_LENGTH,
};

enum content_type_header
{
    APPLICATION_JSON,
    TEXT_HTML,
    TEXT_CSS,
    TEXT_JAVASCRIPT,
    IMAGE_PNG,
    IMAGE_JPEG,
    IMAGE_X_ICON,
    IMAGE_GIF,
    TEXT_PLAIN
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

    static const char* get_header_name(header_name n)
    {
        switch(n)
        {
            case CONTENT_TYPE:   return "Content-Type";   break;
            case CONTENT_LENGTH: return "Content-Length"; break;
            default:             return "UNKNOWN";        break;
        }
    }

    static const char* get_content_type(content_type_header t)
    {
        switch(t)
        {
            case APPLICATION_JSON: return "application/json"; break;
            case TEXT_HTML:        return "text/html";        break;
            case TEXT_CSS:         return "text/css";         break;
            case TEXT_JAVASCRIPT:  return "text/javascript";  break;
            case IMAGE_PNG:        return "image/png";        break;
            case IMAGE_JPEG:       return "image/jpeg";       break;
            case IMAGE_X_ICON:     return "image/x-icon";     break;
            case IMAGE_GIF:        return "image/gif";        break;
            case TEXT_PLAIN:       return "text/plain";       break;
            default:               return "UNKNOWN";          break;
        }
    }

    template<typename T>
    static void write_status(T& st, status_code code)
    {
        st << (uint16_t)code << " " << get_status_code(code);
    }

    template<typename T>
    static void write_version(T& st, version ver)
    {
        st << get_version(ver) << " ";
    }

    template<typename T>
    static void write_status_line(T& st, version ver, status_code code)
    {
        write_version<T>(st, ver);
        write_status<T>(st, code);
        st << "\r\n";
    }
};

} // namespace web
    
} // namespace ecl

