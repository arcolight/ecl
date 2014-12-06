#pragma once

namespace ecl
{

namespace web 
{

struct header
{
    char*  name  { nullptr };
    char*  value { nullptr };

    void clear()
    {
        name =  nullptr;
        value = nullptr;
    }
};

} // namespace web

} // namespace ecl
