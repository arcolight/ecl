#pragma once

#include "status_codes.hpp"
#include "version.hpp"

namespace ecl
{

namespace web
{

struct response
{
    version     ver  { version::UNKNOWN };
    status_code code { status_code::UNKNOWN};
    char*       body { nullptr };
};

} // namespace web
    
} // namespace ecl