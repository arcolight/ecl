#pragma once

#include <cstdint>

#include "constants.hpp"

namespace ecl
{

namespace web
{

class i_resource
{
public:
    virtual ~i_resource() {}
    virtual status_code exec(uint8_t* out_buf, 
                             size_t size,
                             int argc,
                             const uint8_t** const argv)              const = 0;
};

} // namespace web

} // namespace ecl
