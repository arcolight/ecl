#pragma once
#include "stream.hpp"

namespace ecl
{

// level
enum class lvl {
    critical,
    error,
    warning,
    info,
    debug
};

template<typename... OUT>
class logger
{
    logger(const lvl    def_level = lvl::info)
    
    logger& operator() (const lvl& l)
    {
        m_level = l;
        return *this;
    }

private:

    void out_all()                                                         const
    {
        out<OUT...>();
    }

    template<typename O, typename NEXT, typename... TAIL>
    bool out()                                                             const
    {
        return O::print(m_buf) && out<COUNT + 1, TAIL...>();
    }

    template<typename O>
    bool out()                                                             const
    {
        return O::print(m_buf);
    }
};
    
} // namespace ecl