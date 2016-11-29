#ifndef ECL_EXAMPLES_SERVER_HPP
#define ECL_EXAMPLES_SERVER_HPP

#include <ecl/web.hpp>

#define RECV_BUFFER_SIZE 2048

using server_t = ecl::web::server<RECV_BUFFER_SIZE>;

#endif // ECL_EXAMPLES_SERVER_HPP
