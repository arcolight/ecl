#pragma once

namespace ecl
{

namespace web
{

enum class status_code
{
    CONTINUE_100,
    SWITCHING_PROTO_101,

    OK_200,
    CREATED_201,
    ACCEPTED_202,

    NO_CONTENT_204,
    RESET_CONTENT_205,
    PARTIAL_CONTENT_206,

    BAD_REQUEST_400,
    UNAUTHORIZED_401,
    PAYMENT_REQIRED_402,
    FORBIDDEN_403,
    NOT_FOUND_404,
    METHOD_NOT_ALLOWED_405,
    NOT_ACCEPTABLE_406,
    PROXY_AUTHENTICATION_REQUIRED_407,
    REQUEST_TIMEOUT_408,
    CONFLICT_409,

    UNKNOWN
};

} // namespace web
    
} // namespace ecl