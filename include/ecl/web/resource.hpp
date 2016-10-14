#ifndef ECL_WEB_RESOURCE_HPP
#define ECL_WEB_RESOURCE_HPP

#include <cstdint>

#include <ecl/web/i_request_cache.hpp>
#include <ecl/web/i_resource.hpp>

#include <ecl/web/constants.hpp>

namespace ecl
{

namespace web
{

template<typename T, typename ST>
struct static_resource : public i_resource<ST>
{
    virtual ~static_resource()                                          override
    {}

    static_resource(content_type t)
        : m_type ( t )
    {}

    virtual status_code on_request(ST& st, i_request_cache& cache)      override
    {
        if(method::GET != cache.get_met())
        {
            return status_code::METHOD_NOT_ALLOWED;
        }

        write_status_line(st, cache.get_ver(), status_code::OK);

        set_content_type_header(st, m_type);

        if(T::compressed)
        {
            set_content_encoding_header(st, content_encoding::GZIP);
        }

        st << "\r\n" << T::data << "\r\n";

        st.flush();

        return status_code::OK;
    }

private:
    content_type m_type { content_type::TEXT_HTML };
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_RESOURCE_HPP
