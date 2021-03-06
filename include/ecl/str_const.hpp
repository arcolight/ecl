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
    constexpr str_const(const char(&a)[N])                              noexcept
        : m_p(a), m_sz(N-1)
    {}

    constexpr str_const(const char* str, std::size_t N)                 noexcept
        : m_p(str), m_sz(N)
    {}

    constexpr str_const(const str_const& other)                         noexcept
        : m_p(other.m_p), m_sz(other.m_sz)
    {}

    /**
     * @brief Access by index operator.
     *
     * @param n index
     * @return n-th char in string. If n out of bound, returns 0.
     */
    constexpr char operator[](std::size_t n)                      const noexcept
    {
        // Don't use throw. In embedded systems we have no exceptions.
        return (n < m_sz) ? m_p[n] : 0;
    }

    /**
     * @brief String size.
     * @return String length in characters.
     */
    constexpr std::size_t size()                                  const noexcept
    {
        return m_sz;
    }

    /**
     * @brief char* cast operator.
     *
     * @return Pointer to constant string.
     */
    constexpr operator const char*()                              const noexcept
    {
        return m_p;
    }

    /**
     * @brief begining of the string
     *
     * @return Pointer to begining of the constant string
     */
    const char* begin()                                           const noexcept
    {
        return m_p;
    }

    /**
     * @brief end of the string
     *
     * @return Pointer to end of the constant string
     */
    const char* end()                                             const noexcept
    {
        return m_p + m_sz;
    }

    /**
     * @brief      stream operator
     *
     * @param      stream reference to stream object
     *
     * @tparam     T      type of the stream
     *
     * @return     reference to stream object
     */
    template<typename T>
    T& operator<<(T& stream)                                               const
    {
        stream << m_p;
        return stream;
    }

private:
    str_const& operator= (const str_const& other)                      = delete;
    str_const(const str_const&& other)                                 = delete;
    str_const& operator= (const str_const&& other)                     = delete;

    const char* const m_p;
    const std::size_t m_sz;
};

} // namespace ecl

#endif // ECL_STR_CONST_HPP
