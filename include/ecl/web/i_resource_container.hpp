#pragma once

namespace ecl
{

namespace web
{

class i_resource_container
{
public:
    ~i_resource_container() {}

    virtual const i_resource* lookup(const char* const name)          const = 0;
};

} // namespace web
    
} // namespace ecl
