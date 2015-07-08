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

#ifdef ECL_WITH_STD_STRING
#include <string>
#endif

#include <ecl/sized_data.h>

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
typedef enum class base {
    h,
    d,
    o,
    b
} base;

/**
 * @brief Alpha case.
 * @code
 * lower - lower
 * upper - upper
 * @endcode
 *
 */
typedef enum class alpha_case {
    lower,
    upper
} cs;

/**
 * @brief Field width.
 *
 */
typedef struct width
{
    width(size_t w) : m_w(w) {}

    size_t m_w;
} wd;

/**
 * @brief end object
 *
 */
struct end {};
/**
 * @brief reset object
 *
 */
struct reset {};

/**
 * @brief typedef for overflow callback.
 *
 * @param buf pointer to buffer.
 * @param size size of data.
 *
 * @return void
 */
typedef void(*flush_function_t)(const char* const buf, size_t size);

/**
 * @brief Stream class.
 * @details Have fixed-size buffer. Have oveflow callback.
 *
 * @tparam BUFFER_SIZE Size of internal buffer in bytes.
 * @tparam FLUSH_F_PTR = nullptr Pointer to @ref flush_function_t
 * that will be called on internal buffer overflow.
 */
template<size_t BUFFER_SIZE, flush_function_t FLUSH_F_PTR = nullptr>
class stream{
public:
    explicit stream(const base   def_base = base::d,
                    const size_t def_width = 8) :
        m_def_base(def_base),
        m_def_width(def_width)
    {
        reset();
    }

    /**
     * @brief Numeric system base change.
     *
     * @param b @ref base object
     */
    stream& operator() (const base& b)
    {
        m_base = b;
        return *this;
    }

    /**
     * @brief Field width change.
     *
     * @param w @ref width object.
     */
    stream& operator() (const size_t w)
    {
        m_width = w;
        return *this;
    }

    /**
     * @brief Alpha case change.
     *
     * @param c @ref cs object
     */
    stream& operator() (const cs& c)
    {
        m_case = c;
        return *this;
    }

    /**
     * @brief Field width change.
     *
     * @param w @ref width object.
     */
    stream& operator() (const width& w)
    {
        m_width = w.m_w;
        return *this;
    }

    /**
     * @brief Field width change.
     *
     * @param w @ref width object.
     */
    stream& operator<< (const width& w)
    {
        m_width = w.m_w;
        return *this;
    }

    /**
     * @brief Numeric system base change.
     *
     * @param b @ref base object
     */
    stream& operator<< (const base& b)
    {
        m_base = b;
        return *this;
    }

    /**
     * @brief Stream reset.
     * @details Reset will cause erasing all characters from stream.
     *
     * @param end reset object.
     */
    stream& operator<< (const reset& r)
    {
        (void)(r);
        reset();
        return *this;
    }

    /**
     * @brief End of stream.
     * @details End will cause @ref flush and @ref reset.
     *
     * @param end end object.
     */
    stream& operator<< (const end& end)
    {
        (void)(end);
        flush();
        reset();
        return *this;
    }

    /**
     * @brief @ref sized_data serialization.
     * @details @ref sized_data serialized as binary.
     *
     * @param d reference to @ref sized_data object
     */
    stream& operator<< (const sized_data& d)
    {
        print_binary(d.ptr, d.size);
        return *this;
    }

    /**
     * @brief @ref const_sized_data serialization.
     * @details @ref const_sized_data serialized as binary.
     *
     * @param d reference to @ref const_sized_data object
     */
    stream& operator<< (const const_sized_data& d)
    {
        print_binary(d.ptr, d.size);
        return *this;
    }

    template<typename T>
    stream& operator<< (const T& val)
    {
        print_val(val);
        return *this;
    }

    operator const char* ()                                                const
    {
        return (const char*)m_buf;
    }

    operator const char* ()
    {
        return (const char*)m_buf;
    }

    /**
     * @brief Characters count.
     * @return Count of characters in stream.
     */
    size_t count()                                                         const
    {
        return m_count;
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
        if(nullptr != FLUSH_F_PTR)
        {
            FLUSH_F_PTR((const char*)m_buf, m_count);
        }
#pragma GCC diagnostic pop
        reset();
    }

    constexpr static size_t m_s_size { BUFFER_SIZE };

private:
    stream(const stream& other)                                        = delete;
    stream& operator= (const stream& other)                            = delete;
    stream(const stream&& other)                                       = delete;
    stream& operator= (const stream&& other)                           = delete;

    void reset()
    {
        for(auto &c: m_buf)
        {
            c = 0;
        }

        m_count = 0;

        m_base =  m_def_base;
        m_width = m_def_width;
    }

    template<typename T>
    void print_num_signed(const T& val)
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
                typename std::make_unsigned<T>::type uval = val;
                print_num_unsigned(uval);
            }
        }
        else
        {
            print_num_unsigned(val);
        }
    }

    template<typename T>
    void print_num_unsigned(const T& val)
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
            m_num_buf[p] = (cs::upper == m_case) ?
                           toupper(m_alphabet[tmp % bs]) :
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

        print_val((const char* const)m_num_buf);
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
        print_val((const char* const)&tmp[0]);
    }

    void print_val(const char* const val)
    {
        if(nullptr == val)
        {
            return;
        }

        for(size_t i = 0; (val[i] != 0) && (m_count < BUFFER_SIZE + 1); ++i)
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

    void print_binary(const uint8_t* const data, size_t size)
    {
        for(size_t i = 0; i < size; ++i)
        {
            if(m_count == BUFFER_SIZE)
            {
                flush();
            }

            m_buf[m_count] = data[i];
            ++m_count;
        }
    }

    template<typename T>
    void print_float(const T& val)
    {
        T i = 0.0;
        T f = 0.0;
        f = std::modf(val, &i);

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

        print_num_unsigned((uint64_t)i);
        print_val('.');
        print_num_unsigned((uint64_t)(f * std::pow(10, m_width)));
    }

    template<typename T>
    void print_val(const T& val)
    {
        T::print(*this, val); // custom types should support print function
    }

    // can be used for all bases till 16.
    const char* m_alphabet = R"(0123456789abcdef)";

    char         m_num_buf[66]; // (u)int64_t in binary mode takes 64 characters
    uint8_t      m_buf[BUFFER_SIZE + 1];
    size_t       m_count { 0 };

    const base   m_def_base;
    const size_t m_def_width;

    base         m_base  { m_def_base  };
    size_t       m_width { m_def_width };
    cs           m_case  { cs::lower   };
};

} // namespace ecl

#endif // ECL_STREAM_HPP
