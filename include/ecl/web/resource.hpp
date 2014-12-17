#pragma once

#include <cstdint>
#include <cstring>

#include <algorithm>

#include "constants.hpp"

// #include "ecl/str_const.hpp"

namespace ecl
{

namespace web 
{

template<const uint8_t* const DATA,
         size_t               SIZE,
         typename             NAME>
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
        stream << (const char*)DATA << "\r\n";
    }
};

} // namespace web

} // namespace ecl
