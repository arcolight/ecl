#pragma once

namespace ecl
{

template<typename REQUEST>
class i_request_parser
{
public:
    typedef REQUEST request_t;

    virtual ~i_request_parser() {}

    virtual request_t* parse(const char* raw_req, size_t size)              = 0;
};
    
} // namespace ecl
