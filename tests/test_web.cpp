#include <cstdint>
#include <cstdlib>
#include <iostream>

#include <ecl/web/resource.hpp>
#include <ecl/web/resource_table.hpp>
#include <ecl/web/request_parser.hpp>
#include <ecl/web/headers.hpp>
#include <ecl/web/server.hpp>
#include <ecl/web/cgi.hpp>

#include <ecl/str_const.hpp>

#include <sys/socket.h>

char FULL_REQUEST[] = "GET /etc/fs/jquery.js HTTP/1.1\r\nHeader 1: 1\r\nHeader 2: 2\r\nHeader 3: 3\r\nHeader 4: 4\r\nHeader 5: 5\r\nHeader 6: 6\r\n\r\nBODY_BODY\r\n\r\n";

constexpr uint8_t res1_data[] = { 'S','T','A','T','I','C',' ','R','E','S','O','U','R','C','E','\0' };

struct res1_name   { constexpr static char* name() { return "RES1"; } };
struct jquery_name { constexpr static char* name() { return "/etc/fs/jquery.js"; } };
struct res2_name   { constexpr static char* name() { return "RES2"; } };

template<typename NAME>
class test_cgi : public ecl::web::cgi<NAME>
{
public:
    virtual ~test_cgi() {}

    ecl::web::status_code exec(uint8_t* out_buf, 
                               size_t size,
                               int argc,
                               const uint8_t** const argv)        const override
    {
        (void)(argc);
        (void)(argv);
        strncpy((char*)out_buf, "TEST_CGI1", strlen("TEST_CGI1"));

        return ecl::web::status_code::OK;
    }

};

typedef ecl::web::server<512,
                         1024,
                         ecl::web::resource_table<
                             ecl::web::resource<res1_data, sizeof(res1_data), res1_name>,
                             ecl::web::resource<res1_data, sizeof(res1_data), jquery_name>,
                             test_cgi<res2_name>
                         >
> server_t;

int main(int argc, char* argv[])
{
    (void)(argc);
    (void)(argv);

    static server_t server;

    server_t::in_buffer_t& buf = server.get_in_buffer();

    memcpy(buf.data(), FULL_REQUEST, sizeof(FULL_REQUEST));

    ecl::web::status_code code = server.process_request(sizeof(FULL_REQUEST));

    std::cout << "Status code: " << code << std::endl;

    return 0;
}
