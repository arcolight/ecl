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

#include "web_resources/index.h"
#include "web_resources/style.h"
#include "web_resources/jquery.h"

char FULL_REQUEST[] = "GET etc/js/jquery.js HTTP/1.1\r\nHeader 1: 1\r\nHeader 2: 2\r\nHeader 3: 3\r\nHeader 4: 4\r\nHeader 5: 5\r\nHeader 6: 6\r\n\r\nBODY_BODY\r\n\r\n";

ECL_DECL_NAME_TYPE_STRING(index_name,  "index.htm")
ECL_DECL_NAME_TYPE_STRING(style_name,  "etc/style.css")
ECL_DECL_NAME_TYPE_STRING(jquery_name, "etc/js/jquery.js")

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
                ecl::web::resource<index_htm, index_htm_len, index_name>,
                ecl::web::resource<style_css, style_css_len, style_name>,
                ecl::web::resource<jquery_js, jquery_js_len, jquery_name>
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
