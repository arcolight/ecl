#ifndef ECL_STR_CONST
#define ECL_STR_CONST

#include <cstdlib>

// Scott Schurr's str_const class

namespace ecl
{

class str_const
{
public:
    template<std::size_t N>
    constexpr str_const(const char(&a)[N]) : 
        m_p(a), m_sz(N-1)
    {}

    constexpr char operator[](std::size_t n)                               const
    {
        // Don't use throw. In embedded systems we have no exceptions.
        return (n < m_sz) ? m_p[n] : 0;
    }

    constexpr std::size_t size()                                           const
    {
        return m_sz; 
    }

    constexpr operator const char*()                                       const
    {
        return m_p;
    }

private:
    str_const(const str_const& other)                                  = delete;
    str_const& operator= (const str_const& other)                      = delete;
    str_const(const str_const&& other)                                 = delete;
    str_const& operator= (const str_const&& other)                     = delete;

    const char* const m_p;
    const std::size_t m_sz;
};

} // namespace ecl

#endif // ECL_STR_CONST