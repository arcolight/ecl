#include <cstdint>
#include <cstdlib>
#include <iostream>

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <ecl/str_const.hpp>
#include <ecl/name_type.hpp>
#include <ecl/json.hpp>
#include <ecl/web.hpp>

#include "web_resources/index_html.h"
#include "web_resources/style_css.h"
#include "web_resources/jquery_js.h"
#include "web_resources/icon_png.h"
#include "web_resources/favicon_png.h"

#include "web_resources/400_html.h"
#include "web_resources/404_html.h"
#include "web_resources/500_html.h"

ECL_DECL_NAME_TYPE_STRING(index_name_1,  "/")
ECL_DECL_NAME_TYPE_STRING(index_name_2,  "/index.html")
ECL_DECL_NAME_TYPE_STRING(icon_name,     "/etc/img/icon.png")
ECL_DECL_NAME_TYPE_STRING(favicon_name,  "/favicon.png")
ECL_DECL_NAME_TYPE_STRING(style_name,    "/etc/style.css")
ECL_DECL_NAME_TYPE_STRING(jquery_name,   "/etc/js/jquery.js")
ECL_DECL_NAME_TYPE_STRING(info_name,     "/info")
ECL_DECL_NAME_TYPE_STRING(auth_name,     "/auth")

ECL_DECL_NAME_TYPE_STRING(page_400_name, "/400.html")
ECL_DECL_NAME_TYPE_STRING(page_404_name, "/403.html")
ECL_DECL_NAME_TYPE_STRING(page_404_name, "/404.html")
ECL_DECL_NAME_TYPE_STRING(page_500_name, "/500.html")

int new_sd = 0;

void write_sock(const char* const buf, size_t size)
{
    std::cout << buf;
    send(new_sd, buf, size, 0);
}

ECL_DECL_NAME_TYPE(json_1)
ECL_DECL_NAME_TYPE(json_2)
ECL_DECL_NAME_TYPE(json_3)

template<typename... NAME>
class info : public ecl::web::cgi<NAME...>
{
private:
    typedef ecl::json::object<
        ecl::json::node<json_1, bool>,
        ecl::json::node<json_2, uint32_t>,
        ecl::json::node<json_3, ecl::json::string<64>>
    > document_t;
public:
    template<typename T>
    void exec(T& st, const ecl::web::request* req)
    {
        ecl::web::constants::write_status_line(st, req->ver, ecl::web::OK);

        st << ecl::web::constants::get_header_name(ecl::web::CONTENT_TYPE)
           << ":"
           << ecl::web::constants::get_content_type(ecl::web::APPLICATION_JSON) << "\r\n";
        st << "\r\n";

        (void)(req);
        m_doc.f<json_1>() = !m_doc.f<json_1>();
        m_doc.f<json_2>() = m_counter++;
        m_doc.f<json_3>() = "Test json string with \"escaped\" \\characters/.\n\r\tCR LF TAB.";

        m_doc.serialize(st);
        st << "\r\n";
    }

private:
    document_t m_doc;
    uint32_t   m_counter { 0 };
};

template<typename... NAME>
class auth : public ecl::web::cgi<NAME...>
{
public:
    template<typename T>
    void exec(T& st, const ecl::web::request* req)
    {
        std::cout << req->uri << std::endl;
        std::cout << req->uri_param << std::endl;

        ecl::web::constants::write_status_line(st, req->ver, ecl::web::OK);
    }
};

typedef ecl::web::server<
            ecl::web::resource_table<
                ecl::web::resource<res_400_html_t,    ecl::web::TEXT_HTML,       ecl::web::BAD_REQUEST,           page_400_name>,
                ecl::web::resource<res_403_html_t,    ecl::web::TEXT_HTML,       ecl::web::NOT_FOUND,             page_403_name>,
                ecl::web::resource<res_404_html_t,    ecl::web::TEXT_HTML,       ecl::web::NOT_FOUND,             page_404_name>,
                ecl::web::resource<res_500_html_t,    ecl::web::TEXT_HTML,       ecl::web::INTERNAL_SERVER_ERROR, page_500_name>,
                ecl::web::resource<res_index_html_t,  ecl::web::TEXT_HTML,       ecl::web::OK,                    index_name_1, index_name_2>,
                ecl::web::resource<res_icon_png_t,    ecl::web::IMAGE_PNG,       ecl::web::OK,                    icon_name>,
                ecl::web::resource<res_favicon_png_t, ecl::web::IMAGE_PNG,       ecl::web::OK,                    favicon_name>,
                ecl::web::resource<res_style_css_t,   ecl::web::TEXT_CSS,        ecl::web::OK,                    style_name>,
                ecl::web::resource<res_jquery_js_t,   ecl::web::TEXT_JAVASCRIPT, ecl::web::OK,                    jquery_name>,
                info<info_name>,
                auth<auth_name>
            >
> server_t;

char buffer[1024];

void start_server(void);

int main(int argc, char* argv[])
{
    (void)(argc);
    (void)(argv);

    start_server();

    return 0;
}

void start_server()
{
    int status;
    struct addrinfo  host_info;
    struct addrinfo* host_info_list;

    static server_t server;

    memset(&host_info, 0, sizeof host_info);

    std::cout << "Setting up the structs..."  << std::endl;

    host_info.ai_family = AF_UNSPEC;     // IP version not specified. Can be both.
    host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.
    host_info.ai_flags = AI_PASSIVE;     // IP Wildcard

    // Now fill up the linked list of host_info structs with google's address information.
    status = getaddrinfo(NULL, "5556", &host_info, &host_info_list);
    // getaddrinfo returns 0 on succes, or some other value when an error occured.
    // (translated into human readable text by the gai_gai_strerror function).
    if (status != 0)  std::cout << "getaddrinfo error" << gai_strerror(status) ;

    std::cout << "Creating a socket..."  << std::endl;
    int socketfd ; // The socket descripter
    socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype,
                      host_info_list->ai_protocol);
    if (socketfd == -1)
    {
        std::cout << "socket error ";
    }

    std::cout << "Binding socket..."  << std::endl;
    // we use to make the setsockopt() function to make sure the port is not in use
    // by a previous execution of our code. (see man page for more information)
    int yes = 1;
    status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    status = bind(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1)  std::cout << "bind error" << std::endl ;

    std::cout << "Listen()ing for connections..."  << std::endl;
    status =  listen(socketfd, 5);
    if (status == -1)  std::cout << "listen error" << std::endl ;

    while(true)
    {
        memset(buffer, 0x00, sizeof(buffer));

        struct sockaddr_storage their_addr;
        socklen_t addr_size = sizeof(their_addr);
        new_sd = accept(socketfd, (struct sockaddr *)&their_addr, &addr_size);
        if (new_sd == -1)
        {
            std::cout << "listen error" << std::endl ;
        }
        else
        {
            std::cout << "Connection accepted. Using new socketfd : "  <<  new_sd << std::endl;
        }

        std::cout << "Waiting to recieve data..."  << std::endl;
        ssize_t bytes_recieved;
        bytes_recieved = recv(new_sd, buffer, 1024, 0);
        // If no data arrives, the program will just wait here until some data arrives.
        if (bytes_recieved == 0)
        {
            std::cout << "host shut down." << std::endl;
        }
        else if (bytes_recieved == -1)
        {
            std::cout << "recieve error!" << std::endl;
        }

        std::cout << bytes_recieved << " bytes recieved :" << std::endl;
        buffer[bytes_recieved] = 0;
        std::cout << buffer << std::endl;

        ecl::stream<1024, write_sock> out_stream;
        server.process_request(buffer, bytes_recieved, out_stream);
        out_stream.flush();

        close(new_sd);
    }
    std::cout << "Stopping server..." << std::endl;
    freeaddrinfo(host_info_list);
    close(socketfd);
}
