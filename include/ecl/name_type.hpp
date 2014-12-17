#pragma once

#include "str_const.hpp"

namespace ecl
{

#define ECL_DECL_NAME_TYPE(nm)                \
struct nm                                     \
{                                             \
    constexpr static const char* name()       \
    {                                         \
        return ecl::str_const(#nm);           \
    }                                         \
                                              \
    constexpr static size_t size()            \
    {                                         \
        return ecl::str_const(#nm).size();    \
    }                                         \
};

#define ECL_DECL_NAME_TYPE_STRING(nm, string) \
struct nm                                     \
{                                             \
    constexpr static const char* name()       \
    {                                         \
        return ecl::str_const(string);        \
    }                                         \
                                              \
    constexpr static size_t size()            \
    {                                         \
        return ecl::str_const(string).size(); \
    }                                         \
};

} //namespace ecl