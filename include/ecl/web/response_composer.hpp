#pragma once

#include "response.hpp"
#include "constants.hpp"

namespace ecl
{

namespace web
{

class response_composer
{
public:
    void compose(version        ver,
                 status_code    code,
                 const uint8_t* body,
                 size_t         body_size,
                 uint8_t*       buf,
                 size_t         buf_size)
    {
        switch(ver)
        {
            case version::HTTP10:
                // snprintf();
            break;
            case version::HTTP11:
                // snprintf();
            break;
            case version::HTTP20:
            break;
        }

    }
};

} // namespace web
    
} // namespace ecl