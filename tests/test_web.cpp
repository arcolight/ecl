#include <cstdint>
#include <cstdlib>
#include <iostream>

#include <ecl/web/resource.hpp>
#include <ecl/web/resource_table.hpp>
#include <ecl/web/request_parser.hpp>
#include <ecl/web/headers.hpp>

#include <ecl/str_const.hpp>

#define TEST_HEADER1 ("   Test header 1 name :     test header 1 value")
#define TEST_HEADER2 ("   Test header 2 name   test  header 2 value")
#define TEST_HEADER3 (":test header 3 value")
#define TEST_HEADER4 ("Test header 4 name:")
#define TEST_HEADER5 ("   :test header 5 value")
#define TEST_HEADER6 ("Test header 6 name:  ")

#define VALID_HEADER1 "Header 1: 1\r\n"
#define VALID_HEADER2 "Header 2: 2\r\n"
#define VALID_HEADER3 "Header 3: 3\r\n"
#define VALID_HEADER4 "Header 4: 4\r\n"
#define VALID_HEADER5 "Header 5: 5\r\n"
#define VALID_HEADER6 "Header 6: 6\r\n"
#define VALID_HEADER7 "Header 7: 7\r\n"
#define VALID_HEADER8 "Header 8: 8\r\n"
#define VALID_HEADER9 "Header 9: 9\r\n"

#define TEST_REQUEST1 "GET /etc/fs/jquery.js HTTP/1.1\r\n"

#define FULL_REQUEST TEST_REQUEST1 VALID_HEADER1 VALID_HEADER2 VALID_HEADER3 VALID_HEADER4 VALID_HEADER5 VALID_HEADER6 VALID_HEADER7 VALID_HEADER8 VALID_HEADER9

constexpr uint8_t res1_data[] = { 'S','T','A','T','I','C',' ','R','E','S','O','U','R','C','E','\0' };

struct res1_name { static const char* name() { return "RES1"; } };
struct res2_name { static const char* name() { return "RES2"; } };

const uint8_t* handler(int argc, const uint8_t** const argv)
{
    (void)(argc);
    (void)(argv);
    return (const uint8_t*)"TEST_CGI1";
}

bool header_test(const char* const str)
{
    ecl::header<32, 32> hdr;

    bool result = hdr.parse(str, strlen(str));
    std::cout << "Parse result: " << result << std::endl;
    if(result)
    {
        std::cout << "Header name:  " << hdr.name << std::endl;
        std::cout << "Header value: " << hdr.value << std::endl;
    }

    return result;
}

int main(int argc, char* argv[])
{
    (void)(argc);
    (void)(argv);

    ecl::resource_table<
        ecl::resource<res1_data, sizeof(res1_data), res1_name>,
        ecl::cgi<handler, res2_name>
    > table;

    const ecl::i_resource* res_ptr = nullptr;

    res_ptr = table.lookup("RES1");
    std::cout << "RES1 ptr: " << std::hex << res_ptr << std::endl;
    std::cout << "Exec(): " << res_ptr->exec(0, nullptr) << std::endl;

    res_ptr = table.lookup("RES2");
    std::cout << "RES2 ptr: " << std::hex << res_ptr << std::endl;
    std::cout << "Exec(): " << res_ptr->exec(0, nullptr) << std::endl;

    res_ptr = table.lookup("RES3");
    std::cout << "RES3 ptr: " << std::hex << res_ptr << std::endl;

    header_test(TEST_HEADER1);
    header_test(TEST_HEADER2);
    header_test(TEST_HEADER3);
    header_test(TEST_HEADER4);
    header_test(TEST_HEADER5);
    header_test(TEST_HEADER6);

    typedef ecl::request<128,8,32,32,1024> request_t;
    ecl::request_parser<request_t> parser;

    request_t* req_ptr = parser.parse(FULL_REQUEST, strlen(FULL_REQUEST));

    if(nullptr != req_ptr)
    {
        std::cout << "Parse successfull!" << std::endl;
        switch(req_ptr->met)
        {
            case ecl::method::GET:
                std::cout << "Method:   GET" << std::endl;
            break;
            case ecl::method::POST:
                std::cout << "Method:   POST" << std::endl;
            break;
            case ecl::method::UNKNOWN:
                std::cout << "Method:   UNKNOWN" << std::endl;
            break;
        }
        std::cout << "Resource: " << req_ptr->resource << std::endl;
        switch(req_ptr->ver)
        {
            case ecl::version::HTTP11:
                std::cout << "Version:  HTTP/1.1" << std::endl;
            break;
            case ecl::version::HTTP20:
                std::cout << "Version:  HTTP/2.0" << std::endl;
            break;
            case ecl::version::UNKNOWN:
                std::cout << "Version:  UNKNOWN" << std::endl;
            break;
        }

        std::cout << "Headers count: " << req_ptr->hdrs_count << std::endl;

        for(size_t i = 0; i < req_ptr->hdrs_count; ++i)
        {
            std::cout << "Header " << i << std::endl;
            std::cout << "  name:  " << req_ptr->hdrs[i].name 
                      << " | value: " << req_ptr->hdrs[i].value << std::endl;
        }
    }
    else
    {
        std::cout << "Parse failed!" << std::endl;
    }

    return 0;
}
