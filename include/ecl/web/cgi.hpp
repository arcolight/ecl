#pragma once

#include "i_resource.hpp"

namespace ecl
{

namespace web 
{

template<typename NAME>
class cgi : public i_resource
{
public:
    virtual ~cgi()  {}

    constexpr static const char* name()
    {
        return NAME::name();
    }
};

} // namespace web

} // namespace ecl
