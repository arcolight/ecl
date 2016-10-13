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

    virtual bool on_request(ST& st, i_request_cache& cache)             override
    {
        write_status_line(st, cache.get_ver(), status_code::OK);

        set_content_type_header(st, m_type);

        if(T::compressed)
        {
            set_content_encoding_header(st, content_encoding::GZIP);
        }

        st << "\r\n" << T::data << "\r\n";

        st.flush();

        return true;
    }

    virtual std::size_t size()                                          override
    {
        return T::size;
    }

    virtual const uint8_t* data()                                       override
    {
        return T::data;
    }

    virtual bool compressed()                                           override
    {
        return T::compressed;
    }

    virtual content_type type()                                         override
    {
        return m_type;
    }

private:
    content_type m_type { content_type::TEXT_HTML };
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_RESOURCE_HPP
