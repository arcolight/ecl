#pragma once

#include <cstdint>

#include "constants.hpp"

namespace ecl
{

namespace web
{

typedef size_t (*write_function)(const uint8_t* buf, size_t size);

class i_resource
{
public:
    virtual ~i_resource() {}
    virtual status_code exec(write_function f,
                             int argc,
                             const uint8_t** const argv)              const = 0;
};

} // namespace web

} // namespace ecl
