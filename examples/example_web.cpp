#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#include <ecl/name_type.hpp>
#include <ecl/stream.hpp>
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

#include "server.hpp"
#include "cgis.hpp"

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
    ECL_DECL_NAME_TYPE_STRING(upload,           "/upload")

    ECL_DECL_NAME_TYPE_STRING(page_400,         "/400.html")
    ECL_DECL_NAME_TYPE_STRING(page_403,         "/403.html")
    ECL_DECL_NAME_TYPE_STRING(page_404,         "/404.html")
    ECL_DECL_NAME_TYPE_STRING(page_500,         "/500.html")
} // namespace name

static int new_sd = 0;

void write_sock(const char* const buf, std::size_t size);

void write_sock(const char* const buf, std::size_t size)
{
//    std::cout << buf;
    send(new_sd, buf, size, 0);
}

// using server_t = ecl::web::server
// <
//     ecl::web::resource_table
//     <
// //                           Data container type          Type of resource           Gzip   Result code                      Name type
// // static resources
//         ecl::web::resource < res_400_html_t,              ecl::web::content_type::TEXT_HTML,       false, ecl::web::status_code::BAD_REQUEST,           name::page_400               >,
//         ecl::web::resource < res_404_html_t,              ecl::web::content_type::TEXT_HTML,       false, ecl::web::status_code::NOT_FOUND,             name::page_404               >,
//         ecl::web::resource < res_500_html_t,              ecl::web::content_type::TEXT_HTML,       false, ecl::web::status_code::INTERNAL_SERVER_ERROR, name::page_500               >,
// // index
//         ecl::web::resource < res_index_html_t,            ecl::web::content_type::TEXT_HTML,       true,  ecl::web::status_code::OK,                    name::index_1, name::index_2 >,
// // authorized_index
//         ecl::web::resource < res_authorized_index_html_t, ecl::web::content_type::TEXT_HTML,       true,  ecl::web::status_code::OK,                    name::authorized_index       >,
// // logo
//         ecl::web::resource < res_icon_png_t,              ecl::web::content_type::IMAGE_PNG,       true,  ecl::web::status_code::OK,                    name::icon                   >,
// // favicon
//         ecl::web::resource < res_favicon_png_t,           ecl::web::content_type::IMAGE_PNG,       true,  ecl::web::status_code::OK,                    name::favicon                >,
// // CSS
//         ecl::web::resource < res_style_css_t,             ecl::web::content_type::TEXT_CSS,        true,  ecl::web::status_code::OK,                    name::style                  >,
// // jquery
//         ecl::web::resource < res_jquery_js_t,             ecl::web::content_type::TEXT_JAVASCRIPT, true,  ecl::web::status_code::OK,                    name::jquery                 >,
// // CGIs
//         info     < name::info     >,
//         auth     < name::auth     >,
//         settings < name::settings >,
//         upload   < name::upload   >
//     >,
// // Max request size
//     RECV_BUFFER_SIZE
// >;

static char buffer[RECV_BUFFER_SIZE];

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

    static server_t server(write_sock);
//    (
//          std::make_pair(name::page_400::name() , ecl::web::static_resource < resources::res_400_html_t    >())
//        , std::make_pair(name::page_404::name() , ecl::web::static_resource < resources::res_404_html_t    >())
//        , std::make_pair(name::page_500::name() , ecl::web::static_resource < resources::res_500_html_t    >())

//        , std::make_pair(name::index_1::name()  , ecl::web::static_resource < resources::res_index_html_t  >())
//        , std::make_pair(name::index_2::name()  , ecl::web::static_resource < resources::res_index_html_t  >())

//        , std::make_pair(name::icon::name()     , ecl::web::static_resource < resources::res_icon_png_t    >())
//        , std::make_pair(name::favicon::name()  , ecl::web::static_resource < resources::res_favicon_png_t >())
//        , std::make_pair(name::style::name()    , ecl::web::static_resource < resources::res_style_css_t   >())
//        , std::make_pair(name::jquery::name()   , ecl::web::static_resource < resources::res_jquery_js_t   >())
//    );

    bool resource_result = true;

    server_t::resource_t < resources::res_400_html_t    > res_400     ( ecl::web::content_type::TEXT_HTML       );
    server_t::resource_t < resources::res_404_html_t    > res_404     ( ecl::web::content_type::TEXT_HTML       );
    server_t::resource_t < resources::res_500_html_t    > res_500     ( ecl::web::content_type::TEXT_HTML       );

    server_t::resource_t < resources::res_index_html_t  > res_index_1 ( ecl::web::content_type::TEXT_HTML       );
    server_t::resource_t < resources::res_index_html_t  > res_index_2 ( ecl::web::content_type::TEXT_HTML       );

    server_t::resource_t < resources::res_icon_png_t    > res_icon    ( ecl::web::content_type::IMAGE_PNG       );
    server_t::resource_t < resources::res_favicon_png_t > res_favicon ( ecl::web::content_type::IMAGE_PNG       );
    server_t::resource_t < resources::res_style_css_t   > res_style   ( ecl::web::content_type::TEXT_CSS        );
    server_t::resource_t < resources::res_jquery_js_t   > res_jquery  ( ecl::web::content_type::TEXT_JAVASCRIPT );

    cgi_info c_info;

    resource_result &= server.attach_resource( name::page_400::name() , res_400     );
    resource_result &= server.attach_resource( name::page_404::name() , res_404     );
    resource_result &= server.attach_resource( name::page_500::name() , res_500     );

    resource_result &= server.attach_resource( name::index_1::name()  , res_index_1 );
    resource_result &= server.attach_resource( name::index_2::name()  , res_index_2 );

    resource_result &= server.attach_resource( name::icon::name()     , res_icon    );
    resource_result &= server.attach_resource( name::favicon::name()  , res_favicon );
    resource_result &= server.attach_resource( name::style::name()    , res_style   );
    resource_result &= server.attach_resource( name::jquery::name()   , res_jquery  );

    resource_result &= server.attach_resource( name::info::name()     , c_info      );

    if(!resource_result)
    {
        std::cout << "Resource adding error!" << std::endl;
        exit(1);
    }

    memset(&host_info, 0, sizeof host_info);

    std::cout << "Setting up the structs..."  << std::endl;

    host_info.ai_family   = AF_UNSPEC;   // IP version not specified. Can be both.
    host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.
    host_info.ai_flags    = AI_PASSIVE;  // IP Wildcard

    // Now fill up the linked list of host_info structs with google's address information.
    status = getaddrinfo(NULL, port, &host_info, &host_info_list);
    // getaddrinfo returns 0 on succes, or some other value when an error occured.
    // (translated into human readable text by the gai_gai_strerror function).
    if (status != 0)
    {
        std::cout << "getaddrinfo error" << gai_strerror(status) << std::endl;
    }

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
        bytes_recieved = recv(new_sd, buffer, RECV_BUFFER_SIZE, 0);
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
        // for(ssize_t i = 0; i < bytes_recieved; ++i)
        // {
        //     if(std::isprint(buffer[i]))
        //     {
        //         std::cout << buffer[i];
        //     }
        //     else
        //     {
        //         std::cout << std::hex << std::setw(2) << std::setfill('0') << int(buffer[i]) << std::dec;
        //     }
        // }
        // std::cout << std::endl;
        std::cout << buffer << std::endl;

        server.process_request(buffer, static_cast<std::size_t>(bytes_recieved));

        close(new_sd);
    }
    // std::cout << "Stopping server..." << std::endl;
    // freeaddrinfo(host_info_list);
    // close(socketfd);
}
