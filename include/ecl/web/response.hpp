#pragma once

#include "constants.hpp"

namespace ecl
{

namespace web
{

struct response
{
    version     ver  { version::HTTP10                    };
    status_code code { status_code::INTERNAL_SERVER_ERROR };
    char*       body { nullptr                            };
};

} // namespace web
    
} // namespace ecl