/**
 * @file
 *
 * @brief Macro for declare name types.
 * @details Macro for declare structs, that can be used as name types in
 * @ref ecl::command_processor or @ref ecl::json::object
 *
 * @ingroup ecl
 */
#ifndef ECL_NAME_TYPE_HPP
#define ECL_NAME_TYPE_HPP

#include <ecl/str_const.hpp>

namespace ecl
{

#define ECL_DECL_NAME_TYPE(nm)                   \
struct nm                                        \
{                                                \
    constexpr static const char* name() noexcept \
    {                                            \
        return ecl::str_const(#nm);              \
    }                                            \
                                                 \
    constexpr static std::size_t size() noexcept \
    {                                            \
        return ecl::str_const(#nm).size();       \
    }                                            \
};

#define ECL_DECL_NAME_TYPE_STRING(nm, string)    \
struct nm                                        \
{                                                \
    constexpr static const char* name() noexcept \
    {                                            \
        return ecl::str_const(string);           \
    }                                            \
                                                 \
    constexpr static std::size_t size() noexcept \
    {                                            \
        return ecl::str_const(string).size();    \
    }                                            \
};

} //namespace ecl

#endif // ECL_NAME_TYPE_HPP
