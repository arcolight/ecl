/**
 * @file
 *
 * @brief Stream class.
 * @details Stream class with fixed size buffer and overflow-callback.
 * Can be used for data serialization.
 * Suitable for embedded devices.
 *
 * @ingroup ecl
 */
#ifndef ECL_STREAM_HPP
#define ECL_STREAM_HPP

#include <cstdlib>
#include <cstdint>
#include <cmath>
#include <cctype>

#include <type_traits>
#include <functional>

#ifdef ECL_WITH_STD_STRING
#include <string>
#endif

namespace ecl
{

/**
 * @brief Numeric system base.
 * @code
 * h - hex
 * d - decimal
 * o - octal
 * b - binary
 * @endcode
 *
 */
enum class base {
    h,
    d,
    o,
    b
};

/**
 * @brief Alpha case.
 * @code
 * lower - lower
 * upper - upper
 * @endcode
 *
 */
enum class alpha_case {
    lower,
    upper
};

/**
 * @brief Field width.
 *
 */
struct width
{
    explicit width(std::size_t w) : m_w(w) {}

    std::size_t m_w;
};

/**
 * @brief end object
 *
 */
struct end {};
/**
 * @brief reset object
 *
 */
struct rst {};

/**
 * @brief alias for overflow callback.
 *
 * @param buf pointer to buffer.
 * @param size size of data.
 *
 */
using flush_function_t = std::function
                         <
                             void(const char* const buf, std::size_t size)
                         >;

/**
 * @brief Stream class.
 * @details Have fixed-size buffer. Have oveflow callback.
 *
 * @tparam BUFFER_SIZE Size of internal buffer in bytes.
 * @tparam FLUSH_F_PTR = nullptr Pointer to @ref flush_function_t
 * that will be called on internal buffer overflow.
 */
template<std::size_t BUFFER_SIZE>
class stream
{
private:
    void reset()                                                        noexcept
    {
        for(auto &c: m_buf)
        {
            c = 0;
        }

        m_count = 0;

        m_base =  m_def_base;
        m_width = m_def_width;
    }

public:
    explicit stream(flush_function_t  flush_function  = nullptr,
                    const base        def_base        = base::d,
                    const std::size_t def_width       = 8)              noexcept
    :
        m_flush_function ( flush_function ),
        m_def_base       ( def_base       ),
        m_def_width      ( def_width      )
    {
        reset();
    }

    /**
     * @brief Flush stream.
     * @details Flush stream. If flush callback specified, it would be called.
     */
    void flush()
    {
        // For GCC 4.7. We can pass nullptr to stream. Check is needed.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waddress"
        if(nullptr != m_flush_function)
        {
            m_flush_function(m_buf, m_count);
        }
#pragma GCC diagnostic pop
        reset();
    }

    /**
     * @brief Reset Flush function pointer
     *
     * @param Flush function pointer
     */
    void set_flush_function(flush_function_t f)                         noexcept
    {
        m_flush_function = f;
    }

    /**
     * @brief Numeric system base change.
     *
     * @param b Numeric base enumeration. @ref base
     */
    stream& operator() (const base& b)                                  noexcept
    {
        m_base = b;
        return *this;
    }

    /**
     * @brief Field width change.
     *
     * @param w Field width. @ref width
     */
    stream& operator() (const std::size_t w)                            noexcept
    {
        m_width = w;
        return *this;
    }

    /**
     * @brief Alpha case change.
     *
     * @param c Alpha case. @ref cs
     */
    stream& operator() (const alpha_case& c)                            noexcept
    {
        m_case = c;
        return *this;
    }

    /**
     * @brief Field width change.
     *
     * @param w Field width. @ref width
     */
    stream& operator() (const width& w)                                 noexcept
    {
        m_width = w.m_w;
        return *this;
    }

    /**
     * @brief Field width change.
     *
     * @param w Field width. @ref width
     */
    stream& operator<< (const width& w)                                 noexcept
    {
        m_width = w.m_w;
        return *this;
    }

    /**
     * @brief Numeric system base change.
     *
     * @param b Numeric base enumeration. @ref base
     */
    stream& operator<< (const base& b)                                  noexcept
    {
        m_base = b;
        return *this;
    }

    /**
     * @brief Stream reset.
     * @details Reset will cause erasing all characters from stream.
     *
     * @param r reset object. @ref reset
     */
    stream& operator<< (const rst&)                                     noexcept
    {
        reset();
        return *this;
    }

    /**
     * @brief End of stream.
     * @details End will cause @ref flush and @ref reset.
     *
     * @param end end of stream object. @ref end
     */
    stream& operator<< (const end&)                                     noexcept
    {
        flush();
        reset();
        return *this;
    }

    template<std::size_t N>
    stream& operator<< (const uint8_t(&d)[N])
    {
        print_binary(d, N);
        return *this;
    }

    template<typename T>
    stream& operator<< (const T& val)
    {
        print_val(val);
        return *this;
    }

    operator const char* ()                                       const noexcept
    {
        return m_buf;
    }

    operator char* ()                                                   noexcept
    {
        return m_buf;
    }

    const char* data()                                            const noexcept
    {
        return m_buf;
    }

    char* data()                                                        noexcept
    {
        return m_buf;
    }

    /**
     * @brief Characters count.
     * @return Count of characters in stream.
     */
    std::size_t count()                                           const noexcept
    {
        return m_count;
    }

    constexpr static std::size_t m_s_size { BUFFER_SIZE };

private:
    stream(const stream& other)                                        = delete;
    stream& operator= (const stream& other)                            = delete;
    stream(const stream&& other)                                       = delete;
    stream& operator= (const stream&& other)                           = delete;

    template<typename T>
    void print_num_signed(const T& val)                                 noexcept
    {
        if(val < 0)
        {
            if(base::d == m_base)
            {
                print_val('-');
                print_num_unsigned(-val);
            }
            else
            {
                typename std::make_unsigned<T>::type uval =
                    static_cast<typename std::make_unsigned<T>::type>(val);
                print_num_unsigned(uval);
            }
        }
        else
        {
            print_num_unsigned(val);
        }
    }

    template<typename T>
    void print_num_unsigned(const T& val)                               noexcept
    {
        T tmp = val;

        for(auto &c : m_num_buf)
        {
            c = 0;
        }

        uint8_t bs = 0;
        uint8_t p = 0;

        switch(m_base) {
            case base::b:
                bs = 2;
            break;
            case base::o:
                bs = 8;
            break;
            case base::d:
                bs = 10;
            break;
            case base::h:
                bs = 16;
            break;
        }

        do {
            m_num_buf[p] = (alpha_case::upper == m_case) ?
                           static_cast<char>(toupper(m_alphabet[tmp % bs])) :
                           m_alphabet[tmp % bs];
            tmp /= bs;
            ++p;
        } while(0 != tmp);

        for(uint8_t i = 0; i < (p / 2); ++i)
        {
            char t = m_num_buf[p - i - 1];
            m_num_buf[p - i - 1] = m_num_buf[i];
            m_num_buf[i] = t;
        }

        print_val(static_cast<const char*>(m_num_buf));
    }

#ifdef ECL_WITH_STD_STRING
    void print_val(const std::string& str)
    {
        print_val(str.data());
    }
#endif

    void print_val(const int8_t& val)
    {
        print_num_signed(val);
    }

    void print_val(const uint8_t& val)
    {
        print_num_unsigned(val);
    }

    void print_val(const int16_t& val)
    {
        print_num_signed(val);
    }

    void print_val(const uint16_t& val)
    {
        print_num_unsigned(val);
    }

    void print_val(const int32_t& val)
    {
        print_num_signed(val);
    }

    void print_val(const uint32_t& val)
    {
        print_num_unsigned(val);
    }

    void print_val(const int64_t& val)
    {
        print_num_signed(val);
    }

    void print_val(const uint64_t& val)
    {
        print_num_unsigned(val);
    }

    void print_val(const float_t& val)
    {
        print_float(val);
    }

    void print_val(const double_t& val)
    {
        print_float(val);
    }

    void print_val(const long double& val)
    {
        print_float(val);
    }

    void print_val(const bool& val)
    {
        if(val)
        {
            print_val("true");
        }
        else
        {
            print_val("false");
        }
    }

    void print_val(const char& val)
    {
        char tmp[2] = { val, 0 };
        print_val(static_cast<const char*>(tmp));
    }

    void print_val(const char* const val)
    {
        if(nullptr == val)
        {
            return;
        }

        for(std::size_t i = 0; (val[i] != 0) && (m_count < BUFFER_SIZE + 1); ++i)
        {
            if(m_count == BUFFER_SIZE)
            {
                flush();
            }

            m_buf[m_count] = val[i];
            ++m_count;
        }

        m_buf[BUFFER_SIZE] = 0;
    }

    void print_binary(const uint8_t* const bin_data, std::size_t size)  noexcept
    {
        for(std::size_t i = 0; i < size; ++i)
        {
            if(m_count == BUFFER_SIZE)
            {
                flush();
            }

            m_buf[m_count] = static_cast<char>(bin_data[i]);
            ++m_count;
        }
    }

    template<typename T>
    void print_float(const T& val)
    {
        T i = 0.0;
        T f = std::modf(val, &i);

        if((i < 0.0) || (f < 0.0))
        {
            print_val('-');
        }

        if(i < 0.0)
        {
            i = -i;
        }

        if(f < 0.0)
        {
            f = -f;
        }

        print_num_unsigned(static_cast<uint64_t>(i));
        print_val('.');
        print_num_unsigned(static_cast<uint64_t>(f * std::pow(10, m_width)));
    }

    template<typename T>
    void print_val(const T& val)
    {
        // We don't know how to handle this type,
        // maybe it knows hot to be streamed
        val.operator<<(*this);
    }

    // can be used for all bases till 16.
    const char* m_alphabet = R"(0123456789abcdef)";

    flush_function_t  m_flush_function { nullptr };

    char              m_num_buf[66] {};
    char              m_buf[BUFFER_SIZE + 1] {};
    std::size_t       m_count { 0 };

    const base        m_def_base;
    const std::size_t m_def_width;

    base              m_base  { m_def_base        };
    std::size_t       m_width { m_def_width       };
    alpha_case        m_case  { alpha_case::lower };
};

} // namespace ecl

#endif // ECL_STREAM_HPP
