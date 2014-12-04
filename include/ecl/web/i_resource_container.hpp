#pragma once

namespace ecl
{

class i_resource_container
{
public:
    ~i_resource_container() {}

    virtual const i_resource* lookup(const char* const name)          const = 0;
};
    
} // namespace ecl
