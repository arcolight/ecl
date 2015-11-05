#include <cstddef>
#include <cstdint>

#include <ecl/web/cgi.hpp>

#include <ecl/name_type.hpp>
#include <ecl/json.hpp>

namespace name
{
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

template<typename... NAME>
class upload : public ecl::web::cgi<NAME...>
{
public:
    upload()
    {

    }

    template<typename T>
    ecl::web::status_code exec(T& st, const ecl::web::request* req)
    {
        ecl::web::write_status_line(st, req->ver, ecl::web::status_code::OK);

        return ecl::web::status_code::OK;
    }
};

template<typename... NAME>
class settings : public ecl::web::cgi<NAME...>
{
private:
    using document_t = ecl::json::object
    <
        ecl::json::node<name::val_1, int16_t              >,
        ecl::json::node<name::val_2, uint16_t             >,
        ecl::json::node<name::val_3, ecl::json::string<8> >,
        ecl::json::node<name::val_4, int64_t              >,
        ecl::json::node<name::val_5, uint64_t             >
    >;

    document_t m_doc;

public:
    template<typename T>
    ecl::web::status_code exec(T& st, const ecl::web::request* req)
    {
        (void)st;

        for(auto& h : req->headers)
        {
            if((0 == strcmp(ecl::web::to_string(ecl::web::header_name::CONTENT_TYPE), h.name)) &&
               (nullptr != strstr(h.value, ecl::web::to_string(ecl::web::content_type::APPLICATION_JSON))))
            {
                if(m_doc.deserialize(req->body))
                {
                    m_doc.serialize(std::cout, true);
                    std::cout << std::endl;
                    return ecl::web::status_code::OK;
                }

                break;
            }
        }

        ecl::web::redirect(st, "/400.html", req->ver);

        return ecl::web::status_code::BAD_REQUEST;
    }
};

template<typename... NAME>
class info : public ecl::web::cgi<NAME...>
{
private:
    using document_t = ecl::json::object
    <
        ecl::json::node<name::json_1, bool                  >,
        ecl::json::node<name::json_2, uint32_t              >,
        ecl::json::node<name::json_3, ecl::json::string<64> >
    >;

    document_t m_doc {};

public:
    template<typename T>
    ecl::web::status_code exec(T& st, const ecl::web::request* req)
    {
        ecl::web::write_status_line(st, req->ver, ecl::web::status_code::OK);

        ecl::web::set_content_type_header(st, ecl::web::content_type::APPLICATION_JSON);

        st << "\r\n";

        m_doc.f<name::json_1>() = !m_doc.f<name::json_1>();
        m_doc.f<name::json_2>() = m_counter++;
        m_doc.f<name::json_3>() = "Test json string with \"escaped\" \\characters/.\n\r\tCR LF TAB.";

        m_doc.serialize(st);
        st << "\r\n";

        return ecl::web::status_code::OK;
    }

private:
    uint32_t m_counter { 0 };
};

template<typename... NAME>
class auth : public ecl::web::cgi<NAME...>
{
public:
    template<typename T>
    ecl::web::status_code exec(T& st, const ecl::web::request* req)
    {
        char buf[128];

        if(strlen(req->body) < 128)
        {
            strcpy(buf, req->body);
        }
        else
        {
            std::cout << "Too big body :(" << std::endl;
            return ecl::web::status_code::OK;
        }

        ecl::web::uri_param params[16];

        if(ecl::web::kv_parser_state::done !=
               m_parser.start_parse(buf, params, 16))
        {
            std::cout << "Parse failed!" << std::endl;
            authorized = false;
        }
        else
        {
            std::cout << "Parse ok!" << std::endl;
            authorized = false;
            for(std::size_t i = 0; i < 16; ++i)
            {
                if((0 == strcmp(params[i].name, "pass")) &&
                   (0 == strcmp(params[i].value, "239")))
                {
                    authorized = true;
                    ecl::web::redirect(st, "/authorized_index.html", req->ver);
                    return ecl::web::status_code::OK;
                }
            }
        }

        ecl::web::redirect(st, "/index.html", req->ver);
        return ecl::web::status_code::OK;
    }

private:
    bool authorized { false };
    ecl::web::kv_parser m_parser
    {
        ecl::str_const("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"),
        ecl::str_const("%+.-"),
        ecl::str_const("="),
        ecl::str_const("&;")
    };
};
