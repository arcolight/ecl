/**
 * @file
 *
 * @brief Constant string class.
 * @details Can be assigned only at construction.
 * Can be used for compile time string size calculation.
 *
 * @ingroup ecl
 */
#ifndef ECL_STR_CONST_HPP
#define ECL_STR_CONST_HPP

#include <cstdlib>

// Scott Schurr's str_const class

namespace ecl
{

/**
 * @brief Constant string class.
 * @details Scott Schurr's constant string class.
 * Can be used for compile time string size calculation.
 *
 */
class str_const
{
public:
    template<std::size_t N>
    constexpr explicit str_const(const char(&a)[N]) :
        m_p(a), m_sz(N-1)
    {}

    constexpr str_const(const char* str, std::size_t N)
        :m_p(str), m_sz(N)
    {}

    /**
     * @brief Access by index operator.
     *
     * @param n index
     * @return n-th char in string. If n out of bound, returns 0.
     */
    constexpr char operator[](std::size_t n)                               const
    {
        // Don't use throw. In embedded systems we have no exceptions.
        return (n < m_sz) ? m_p[n] : 0;
    }

    /**
     * @brief String size.
     * @return String length in characters.
     */
    constexpr std::size_t size()                                           const
    {
        return m_sz;
    }

    /**
     * @brief char* cast operator.
     *
     * @return Pointer to constant string.
     */
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

#endif // ECL_STR_CONST_HPP
