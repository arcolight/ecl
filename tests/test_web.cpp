#include <cstdint>
#include <cstdlib>
#include <iostream>

#include <ecl/web/resource.hpp>
#include <ecl/web/resource_table.hpp>
#include <ecl/web/request_parser.hpp>
#include <ecl/web/headers.hpp>

#include <ecl/str_const.hpp>

char TEST_HEADER1[] = "   Test header 1 name :     test header 1 value";
char TEST_HEADER2[] = "   Test header 2 name   test  header 2 value";
char TEST_HEADER3[] = ":test header 3 value";
char TEST_HEADER4[] = "Test header 4 name:";
char TEST_HEADER5[] = "   :test header 5 value";
char TEST_HEADER6[] = "Test header 6 name:  ";

char VALID_HEADER1[] = "Header 1: 1\r\n";
char VALID_HEADER2[] = "Header 2: 2\r\n";
char VALID_HEADER3[] = "Header 3: 3\r\n";
char VALID_HEADER4[] = "Header 4: 4\r\n";
char VALID_HEADER5[] = "Header 5: 5\r\n";
char VALID_HEADER6[] = "Header 6: 6\r\n";
char VALID_HEADER7[] = "Header 7: 7\r\n";
char VALID_HEADER8[] = "Header 8: 8\r\n";
char VALID_HEADER9[] = "Header 9: 9\r\n";

char TEST_REQUEST1[] = "GET /etc/fs/jquery.js HTTP/1.1\r\n";

// #define FULL_REQUEST TEST_REQUEST1 VALID_HEADER1 VALID_HEADER2 VALID_HEADER3 VALID_HEADER4 VALID_HEADER5 VALID_HEADER6 VALID_HEADER7 VALID_HEADER8 VALID_HEADER9
char FULL_REQUEST[] = "GET /etc/fs/jquery.js HTTP/1.1\r\nHeader 1: 1\r\nHeader 2: 2\r\nHeader 3: 3\r\nHeader 4: 4\r\nHeader 5: 5\r\nHeader 6: 6\r\n\r\nBODY_BODY\r\n\r\n";

constexpr uint8_t res1_data[] = { 'S','T','A','T','I','C',' ','R','E','S','O','U','R','C','E','\0' };

struct res1_name { static const char* name() { return "RES1"; } };
struct res2_name { static const char* name() { return "RES2"; } };

const uint8_t* handler(int argc, const uint8_t** const argv)
{
    (void)(argc);
    (void)(argv);
    return (const uint8_t*)"TEST_CGI1";
}

int main(int argc, char* argv[])
{
    (void)(argc);
    (void)(argv);

    ecl::web::resource_table<
        ecl::web::resource<res1_data, sizeof(res1_data), res1_name>,
        ecl::web::cgi<handler, res2_name>
    > table;

    const ecl::web::i_resource* res_ptr = nullptr;

    res_ptr = table.lookup("RES1");
    std::cout << "RES1 ptr: " << std::hex << res_ptr << std::endl;
    std::cout << "Exec(): " << res_ptr->exec(0, nullptr) << std::endl;

    res_ptr = table.lookup("RES2");
    std::cout << "RES2 ptr: " << std::hex << res_ptr << std::endl;
    std::cout << "Exec(): " << res_ptr->exec(0, nullptr) << std::endl;

    res_ptr = table.lookup("RES3");
    std::cout << "RES3 ptr: " << std::hex << res_ptr << std::endl;

    ecl::web::request_parser parser;

    ecl::web::request* req_ptr = parser.parse(FULL_REQUEST, strlen(FULL_REQUEST));

    if(nullptr != req_ptr)
    {
        std::cout << "Parse successfull!" << std::endl;
        switch(req_ptr->met)
        {
            case ecl::web::method::GET:
                std::cout << "Method:   GET" << std::endl;
            break;
            case ecl::web::method::POST:
                std::cout << "Method:   POST" << std::endl;
            break;
            case ecl::web::method::UNKNOWN:
                std::cout << "Method:   UNKNOWN" << std::endl;
            break;
        }
        std::cout << "Resource: " << req_ptr->uri << std::endl;
        switch(req_ptr->ver)
        {
            case ecl::web::version::HTTP11:
                std::cout << "Version:  HTTP/1.1" << std::endl;
            break;
            case ecl::web::version::HTTP20:
                std::cout << "Version:  HTTP/2.0" << std::endl;
            break;
            case ecl::web::version::UNKNOWN:
                std::cout << "Version:  UNKNOWN" << std::endl;
            break;
        }

        std::cout << "Headers count: " << req_ptr->headers_count << std::endl;

        for(size_t i = 0; i < req_ptr->headers_count; ++i)
        {
            std::cout << "Header " << i << std::endl;
            std::cout << "  name:  " << req_ptr->headers[i].name 
                      << " | value: " << req_ptr->headers[i].value << std::endl;
        }
    }
    else
    {
        std::cout << "Parse failed!" << std::endl;
    }

    return 0;
}
