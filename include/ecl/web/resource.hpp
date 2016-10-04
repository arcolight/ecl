#ifndef ECL_WEB_RESOURCE_HPP
#define ECL_WEB_RESOURCE_HPP

#include <cstdint>

#include <ecl/web/i_request_cache.hpp>
#include <ecl/web/i_resource.hpp>

namespace ecl
{

namespace web
{

template<typename T>
struct static_resource : public i_resource
{
    virtual ~static_resource()                                          override
    {}

    static_resource()
        : data       ( T::data       )
        , size       ( T::size       )
        , compressed ( T::compressed )
    {}

    const std::uint8_t* data       { nullptr };
    const std::size_t   size       { 0       };
    bool                compressed { false   };

    virtual bool on_request(i_request_cache&)                           override
    {
        return true;
    }
};

} // namespace web

} // namespace ecl

#endif // ECL_WEB_RESOURCE_HPP
