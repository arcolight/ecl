#pragma once

#include <ecl/name_type.hpp>
#include "constants.hpp"

namespace ecl
{

namespace web
{

extern constexpr unsigned char default_page_400_data[] = " \
<!DOCTYPE html PUBLIC \"-//IETF//DTD HTML 2.0//EN\"> \
<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\"> \
<title>400 Bad Request</title></head><body><h1>400 Bad Request</h1> \
<p>Bad Request.</p> \
</body></html>";

extern constexpr unsigned char default_page_404_data[] = " \
<!DOCTYPE html PUBLIC \"-//IETF//DTD HTML 2.0//EN\"> \
<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\"> \
<title>404 Page not found</title></head><body><h1>404 Page not found</h1> \
<p>Page not found.</p> \
</body></html>";

extern constexpr unsigned char default_page_500_data[] = " \
<!DOCTYPE html PUBLIC \"-//IETF//DTD HTML 2.0//EN\"> \
<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\"> \
<title>500 Internal server error</title></head><body><h1>500 Internal server error</h1> \
<p>Internal server error.</p> \
</body></html>";

ECL_DECL_NAME_TYPE_STRING(page_400_name, "/400.html")
ECL_DECL_NAME_TYPE_STRING(page_404_name, "/404.html")
ECL_DECL_NAME_TYPE_STRING(page_500_name, "/500.html")

typedef resource<sizeof(default_page_400_data),
                 default_page_400_data,
                 TEXT_HTML,
                 BAD_REQUEST,
                 page_400_name> default_page_400;
typedef resource<sizeof(default_page_404_data),
                 default_page_404_data, 
                 TEXT_HTML,
                 NOT_FOUND,
                 page_404_name> default_page_404;
typedef resource<sizeof(default_page_500_data),
                 default_page_500_data,
                 TEXT_HTML,
                 INTERNAL_SERVER_ERROR,
                 page_500_name> default_page_500;

} // namespace web

} // namespace ecl