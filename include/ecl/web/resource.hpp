#pragma once

#include <cstdint>
#include <cstdlib>

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
    constexpr resource() {}
    virtual ~resource()  {}

    constexpr static const char* name()
    {
        return NAME::name();
    }

    virtual const uint8_t* exec(int argc, 
                                const uint8_t** const argv)       const override
    {
        (void)(argc);
        (void)(argv);
        return DATA;
    }
};

typedef const uint8_t* (*cgi_handler_t)(int argc, const uint8_t** const argv);

template<cgi_handler_t CGI_HANDLER,
         typename      NAME>
class cgi : public i_resource
{
public:
    constexpr cgi() {}
    virtual ~cgi()  {}

    constexpr static const char* name()
    {
        return NAME::name();
    }

    virtual const uint8_t* exec(int argc, 
                                const uint8_t** const argv)       const override
    {
        // static_assert((CGI_HANDLER != nullptr), "CGI handle can't be nullptr!");
        return CGI_HANDLER(argc, argv);
    }
};

} // namespace web

} // namespace ecl
