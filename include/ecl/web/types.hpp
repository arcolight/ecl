#ifndef ECL_WEB_TYPES_HPP
#define ECL_WEB_TYPES_HPP

#include <utility>
#include <functional>
#include <cstddef>

namespace ecl
{

namespace web
{

using url_t           = const char*;
using header_name_t   = const char*;
using header_value_t  = const char*;
using header_t        = std::pair<header_name_t, header_value_t>;

using send_callback_t = std::function
                        <
                            void(const char* const, std::size_t)
                        >;

} // namespace web

} // namespace ecl

#endif // ECL_WEB_TYPES_HPP
