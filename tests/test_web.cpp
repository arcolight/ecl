#include <cstdint>
#include <cstdlib>
#include <iostream>

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <ecl/name_type.hpp>
#include <ecl/json.hpp>
#include <ecl/web.hpp>

// include generated sources
#include "web_resources/index_html.h"
#include "web_resources/authorized_index_html.h"
#include "web_resources/style_css.h"
#include "web_resources/jquery_js.h"
#include "web_resources/icon_png.h"
#include "web_resources/favicon_png.h"

#include "web_resources/400_html.h"
#include "web_resources/403_html.h"
#include "web_resources/404_html.h"
#include "web_resources/500_html.h"

namespace name
{
    ECL_DECL_NAME_TYPE_STRING(index_1,          "/")
    ECL_DECL_NAME_TYPE_STRING(index_2,          "/index.html")
    ECL_DECL_NAME_TYPE_STRING(authorized_index, "/authorized_index.html")
    ECL_DECL_NAME_TYPE_STRING(icon,             "/etc/img/icon.png")
    ECL_DECL_NAME_TYPE_STRING(favicon,          "/favicon.png")
    ECL_DECL_NAME_TYPE_STRING(style,            "/etc/style.css")
    ECL_DECL_NAME_TYPE_STRING(jquery,           "/etc/js/jquery.js")
    ECL_DECL_NAME_TYPE_STRING(info,             "/info")
    ECL_DECL_NAME_TYPE_STRING(auth,             "/auth")
    ECL_DECL_NAME_TYPE_STRING(settings,         "/settings")

    ECL_DECL_NAME_TYPE_STRING(page_400,         "/400.html")
    ECL_DECL_NAME_TYPE_STRING(page_403,         "/403.html")
    ECL_DECL_NAME_TYPE_STRING(page_404,         "/404.html")
    ECL_DECL_NAME_TYPE_STRING(page_500,         "/500.html")

    ECL_DECL_NAME_TYPE(json_1)
    ECL_DECL_NAME_TYPE(json_2)
    ECL_DECL_NAME_TYPE(json_3)

    ECL_DECL_NAME_TYPE(val_1)
    ECL_DECL_NAME_TYPE(val_2)
    ECL_DECL_NAME_TYPE(val_3)
    ECL_DECL_NAME_TYPE(val_4)
    ECL_DECL_NAME_TYPE(val_5)
    ECL_DECL_NAME_TYPE(val_6)
    ECL_DECL_NAME_TYPE(val_7)
    ECL_DECL_NAME_TYPE(val_8)
    ECL_DECL_NAME_TYPE(val_9)
} // namespace name

static int new_sd = 0;

void write_sock(const char* const buf, std::size_t size);

void write_sock(const char* const buf, std::size_t size)
{
    std::cout << buf;
    send(new_sd, buf, size, 0);
}

template<typename... NAME>
class settings : public ecl::web::cgi<NAME...>
{
private:
    using document_t = ecl::json::object<
        ecl::json::node<name::val_1, int16_t              >,
        ecl::json::node<name::val_2, uint16_t             >,
        ecl::json::node<name::val_3, ecl::json::string<8> >,
        ecl::json::node<name::val_4, int64_t              >,
        ecl::json::node<name::val_5, uint64_t             >
    >;

    document_t m_doc;

public:
    template<typename T>
    const ecl::web::request* exec(T& st, const ecl::web::request* req)
    {
        (void)st;

        for(auto& h : req->headers)
        {
            if(h.name == nullptr || h.value == nullptr)
            {
                continue;
            }

            std::cout << "Checking header: " << h.name << " : " << h.value << std::endl;

            if((0 == strcmp(ecl::web::constants::get_header_name(ecl::web::CONTENT_TYPE), h.name)) &&
               (nullptr != strstr(h.value, ecl::web::constants::get_content_type(ecl::web::APPLICATION_JSON))))
            {
                std::cout << "Trying to deserialize " << req->body << std::endl;
                if(m_doc.deserialize(req->body))
                {
                    m_doc.serialize(std::cout, true);
                    std::cout << std::endl;
                    return nullptr;
                }

                break;
            }
        }

        return this->redirect("/400.html");
    }
};

template<typename... NAME>
class info : public ecl::web::cgi<NAME...>
{
private:
    using document_t = ecl::json::object<
        ecl::json::node<name::json_1, bool                  >,
        ecl::json::node<name::json_2, uint32_t              >,
        ecl::json::node<name::json_3, ecl::json::string<64> >
    >;

    document_t m_doc {};

public:
    template<typename T>
    const ecl::web::request* exec(T& st, const ecl::web::request* req)
    {
        ecl::web::constants::write_status_line(st, req->ver, ecl::web::OK);

        st << ecl::web::constants::get_header_name(ecl::web::CONTENT_TYPE)
           << ":"
           << ecl::web::constants::get_content_type(ecl::web::APPLICATION_JSON)
           << "\r\n";
        st << "\r\n";

        m_doc.f<name::json_1>() = !m_doc.f<name::json_1>();
        m_doc.f<name::json_2>() = m_counter++;
        m_doc.f<name::json_3>() = "Test json string with \"escaped\" \\characters/.\n\r\tCR LF TAB.";

        m_doc.serialize(st);
        st << "\r\n";

        return nullptr;
    }

private:
    uint32_t   m_counter { 0 };
};

static bool authorized = false;

template<typename... NAME>
class auth : public ecl::web::cgi<NAME...>
{
public:
    template<typename T>
    const ecl::web::request* exec(T& st, const ecl::web::request* req)
    {
        std::cout << req->uri              << std::endl;
        std::cout << req->uri_param_string << std::endl;
        for(auto& p : req->uri_parameters)
        {
            if(p.name != nullptr)
            {
                std::cout << p.name << " : " << p.value << std::endl;
            }
        }

        authorized = true;

        ecl::web::redirect(st, "/authorized_index.html", req->ver);

        return nullptr;
    }
};

using server_t = ecl::web::server
<
    ecl::web::resource_table
    <
//                           Data container type          Type of resource           Gzip   Result code                      Name type
// static resources
        ecl::web::resource < res_400_html_t,              ecl::web::TEXT_HTML,       false, ecl::web::BAD_REQUEST,           name::page_400               >,
        ecl::web::resource < res_404_html_t,              ecl::web::TEXT_HTML,       false, ecl::web::NOT_FOUND,             name::page_404               >,
        ecl::web::resource < res_500_html_t,              ecl::web::TEXT_HTML,       false, ecl::web::INTERNAL_SERVER_ERROR, name::page_500               >,
// index
        ecl::web::resource < res_index_html_t,            ecl::web::TEXT_HTML,       true,  ecl::web::OK,                    name::index_1, name::index_2 >,
// authorized_index
        ecl::web::resource < res_authorized_index_html_t, ecl::web::TEXT_HTML,       true,  ecl::web::OK,                    name::authorized_index       >,
// logo
        ecl::web::resource < res_icon_png_t,              ecl::web::IMAGE_PNG,       true,  ecl::web::OK,                    name::icon                   >,
// favicon
        ecl::web::resource < res_favicon_png_t,           ecl::web::IMAGE_PNG,       true,  ecl::web::OK,                    name::favicon                >,
// CSS
        ecl::web::resource < res_style_css_t,             ecl::web::TEXT_CSS,        true,  ecl::web::OK,                    name::style                  >,
// jquery
        ecl::web::resource < res_jquery_js_t,             ecl::web::TEXT_JAVASCRIPT, true,  ecl::web::OK,                    name::jquery                 >,
// CGIs
        info     < name::info     >,
        auth     < name::auth     >,
        settings < name::settings >
    >,
// Max request size
    1024
>;

static char buffer[1024];

[[ noreturn ]]
void start_server(const char*);

int main(int argc, char* argv[])
{
    const char* port = "80";
    if(argc > 1)
    {
        port = argv[1];
    }

    std::cout << "port: " << port << std::endl;

    start_server(port);
}

void start_server(const char* port)
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
    status = getaddrinfo(NULL, port, &host_info, &host_info_list);
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
        exit(1);
    }

    std::cout << "Binding socket..."  << std::endl;
    // we use to make the setsockopt() function to make sure the port is not in use
    // by a previous execution of our code. (see man page for more information)
    int yes = 1;
    status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    status = bind(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1)
    {
        std::cout << "bind error" << std::endl;
        exit(1);
    }

    std::cout << "Listen()ing for connections at port " << port << "..." << std::endl;
    status =  listen(socketfd, 5);
    if (status == -1)
    {
        std::cout << "listen error" << std::endl;
        exit(1);
    }

    while(true)
    {
        memset(buffer, 0x00, sizeof(buffer));

        struct sockaddr their_addr;
        socklen_t addr_size = sizeof(their_addr);
        new_sd = accept(socketfd, &their_addr, &addr_size);
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

        ecl::stream<1024> out_stream(write_sock);
        server.process_request(out_stream, buffer, static_cast<std::size_t>(bytes_recieved));
        out_stream.flush();

        close(new_sd);
    }
    // std::cout << "Stopping server..." << std::endl;
    // freeaddrinfo(host_info_list);
    // close(socketfd);
}
