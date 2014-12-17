#pragma once

#include <cstdint>
#include <cstring>

#include <algorithm>

#include <ecl/sized_data.h>
#include "constants.hpp"

namespace ecl
{

namespace web 
{

template<const char* DATA,
         size_t      SIZE,
         typename    NAME>
class resource
{
public:
    typedef NAME name_t;

    template<typename T>
    void exec(T& stream, int argc, const char* argv[])                     const
    {
        (void)(argc);
        (void)(argv);

        status_code code = status_code::OK;

        stream << (uint16_t)code << " " << constants::get_status_code(code) << "\r\n";
        const sized_data sd { (uint8_t* const)DATA, SIZE };
        stream << sd << "\r\n";
    }
};

} // namespace web

} // namespace ecl
