#pragma once

#include "request.hpp"

namespace ecl
{

namespace web 
{

class i_request_parser
{
public:
    virtual ~i_request_parser() {}

    virtual request* parse(char* raw, size_t size)                          = 0;
};

} // namespace web
    
} // namespace ecl
