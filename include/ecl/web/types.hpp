#ifndef ECL_WEB_TYPES_HPP
#define ECL_WEB_TYPES_HPP

#include <utility>

namespace ecl
{

namespace web
{

using url_t    = const char*;
using header_t = std::pair<const char*, const char*>;

} // namespace web

} // namespace ecl

#endif // ECL_WEB_TYPES_HPP
