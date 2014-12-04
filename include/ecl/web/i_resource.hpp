#pragma once

#include <cstdint>

namespace ecl
{

class i_resource
{
public:
    virtual ~i_resource() {}
    virtual const uint8_t* exec(int argc, const uint8_t** const argv) const = 0;
};

} // namespace ecl
