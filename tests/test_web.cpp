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
#include <ecl/name_type.hpp>

char FULL_REQUEST[] = "GET /etc/fs/jquery.js HTTP/1.1\r\nHeader 1: 1\r\nHeader 2: 2\r\nHeader 3: 3\r\nHeader 4: 4\r\nHeader 5: 5\r\nHeader 6: 6\r\n\r\nBODY_BODY\r\n\r\n";

constexpr uint8_t res1_data[] = { 'S','T','A','T','I','C',' ','R','E','S','O','U','R','C','E','\0' };

ECL_DECL_NAME_TYPE_STRING(res1,   "RES1")
ECL_DECL_NAME_TYPE_STRING(res2,   "RES2")
ECL_DECL_NAME_TYPE_STRING(jquery, "/etc/fs/jquery.js")

void write_stdout(const char* const buf, size_t size)
{
    (void)(size);
    std::cout << buf << std::endl;
}

template<typename NAME>
class test_cgi : public ecl::web::cgi<NAME>
{
public:
    virtual ~test_cgi() {}

    template<typename T>
    void exec(T& stream, int argc, const char* argv[])                     const
    {
        (void)(argc);
        (void)(argv);
        ecl::web::status_code code = ecl::web::status_code::OK;

        stream << (uint16_t)code << ecl::web::constants::get_status_code(code) << "\r\n";
        stream << "TEST_CGI1" << "\r\n";
    }
};

typedef ecl::web::server<
            1024,
            ecl::web::resource_table<
                ecl::web::resource<res1_data, sizeof(res1_data), res1>,
                ecl::web::resource<res1_data, sizeof(res1_data), jquery>,
                test_cgi<res2>
            >,
            write_stdout
> server_t;

char buffer[512];

int main(int argc, char* argv[])
{
    (void)(argc);
    (void)(argv);

    memset(buffer, 0x00, sizeof(buffer));
    memcpy(buffer, FULL_REQUEST, sizeof(FULL_REQUEST));

    static server_t server;

    server.process_request(buffer, sizeof(FULL_REQUEST));

    return 0;
}
