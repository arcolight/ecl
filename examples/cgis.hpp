#ifndef ECL_EXAMPLES_CGIS_HPP
#define ECL_EXAMPLES_CGIS_HPP

#include "server.hpp"

#include <ecl/web/constants.hpp>

#include <ecl/name_type.hpp>
#include <ecl/json.hpp>

namespace json_name
{
    ECL_DECL_NAME_TYPE(json_1)
    ECL_DECL_NAME_TYPE(json_2)
    ECL_DECL_NAME_TYPE(json_3)

    ECL_DECL_NAME_TYPE(val_1)
    ECL_DECL_NAME_TYPE(val_2)
    ECL_DECL_NAME_TYPE(val_3)
    ECL_DECL_NAME_TYPE(val_4)
    ECL_DECL_NAME_TYPE(val_5)
} // json_name

using json_info_t = ecl::json::object
<
      ecl::json::node < json_name::json_1 , uint32_t >
    , ecl::json::node < json_name::json_2 , uint32_t >
    , ecl::json::node < json_name::json_3 , uint32_t >
>;

struct cgi_info : public server_t::i_resource_t
{
    virtual ~cgi_info()                                        noexcept override
    {}

    virtual ecl::web::status_code on_request(
            server_t::i_resource_t::stream_t& st,
            ecl::web::i_request_cache&        c
        )                                                      noexcept override
    {
        switch(c.get_met())
        {
            case ecl::web::method::GET:
            {
                m_json.f<json_name::json_1>()++;
                m_json.f<json_name::json_2>()++;
                m_json.f<json_name::json_3>()++;

                st << m_json;

                std::cout << m_json << std::endl;

                st.flush();

                return ecl::web::status_code::OK;
            }
            break;
            case ecl::web::method::HEAD    : break;
            case ecl::web::method::PUT     : break;
            case ecl::web::method::DELETE  : break;
            case ecl::web::method::POST    : break;
            case ecl::web::method::OPTIONS : break;
            case ecl::web::method::TRACE   : break;
            case ecl::web::method::CONNECT : break;
        }

        return ecl::web::status_code::METHOD_NOT_ALLOWED;
    }

private:
    json_info_t m_json {};
};

using json_settings_t = ecl::json::object
<
      ecl::json::node < json_name::val_1 , int8_t               >
    , ecl::json::node < json_name::val_2 , uint8_t              >
    , ecl::json::node < json_name::val_3 , ecl::json::string<8> >
    , ecl::json::node < json_name::val_4 , int64_t              >
    , ecl::json::node < json_name::val_5 , uint64_t             >
>;

struct cgi_settings : public server_t::i_resource_t
{
    virtual ~cgi_settings()                                    noexcept override
    {}

    virtual ecl::web::status_code on_request(
            server_t::i_resource_t::stream_t&,
            ecl::web::i_request_cache&        c
        )                                                      noexcept override
    {
        switch(c.get_met())
        {
            case ecl::web::method::GET     : break;
            case ecl::web::method::HEAD    : break;
            case ecl::web::method::PUT     : break;
            case ecl::web::method::DELETE  : break;
            case ecl::web::method::POST    :
            {
                auto b = c.get_body();
                if(m_json.deserialize(b.first))
                {
                    std::cout << m_json << std::endl;
                }
                else
                {
                    std::cout << "deserialize failed." << std::endl;
                }


                return ecl::web::status_code::OK;
            }
            break;
            case ecl::web::method::OPTIONS : break;
            case ecl::web::method::TRACE   : break;
            case ecl::web::method::CONNECT : break;
            break;
        }

        return ecl::web::status_code::METHOD_NOT_ALLOWED;
    }

private:
    json_settings_t m_json {};
};


#endif // ECL_EXAMPLES_CGIS_HPP
