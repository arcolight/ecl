#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <algorithm>

#include "i_resource.hpp"

// #include "ecl/str_const.hpp"

namespace ecl
{

namespace web 
{

template<const uint8_t* const DATA,
         size_t               SIZE,
         typename             NAME>
class resource : public i_resource
{
public:
    virtual ~resource()  {}

    constexpr static const char* name()
    {
        return NAME::name();
    }

    virtual status_code exec(uint8_t* out_buf, 
                             size_t size,
                             int argc,
                             const uint8_t** const argv)          const override
    {
        (void)(argc);
        (void)(argv);
        memcpy(out_buf, DATA, std::min(size, SIZE));

        return status_code::OK;
    }
};

} // namespace web

} // namespace ecl
