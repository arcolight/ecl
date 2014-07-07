#ifndef ECL_LOG_HPP
#define ECL_LOG_HPP

#include <cstdlib>
#include <cstdint>
#include <cmath>

#include <type_traits>

// template <typename T1, typename T2> struct same_type
// {
//     constexpr static bool value = false;
// };

// template <typename T> struct same_type<T,T>
// {
//     constexpr static bool value = true;
// };

namespace ecl
{

typedef enum class base {
    h,
    d,
    o,
    b
} base;

typedef enum class align {
    l,
    r
} align;

typedef enum class options {
    e, // exponent for float
    s  // signed for numeric
} opt;

// level
enum class lvl {
    critical,
    error,
    warning,
    info,
    debug
};

struct end {};
struct reset {};

template<size_t BUFFER_SIZE, typename... out_t>
class logger{
public:
    explicit logger(const base   def_base = base::d,
                   const align  def_align = align::l,
                   const size_t def_width = 8,
                   const lvl    def_level = lvl::info) : 
        m_def_base(def_base),
        m_def_align(def_align),
        m_def_width(def_width),
        m_def_level(def_level)
    {
        reset();
    }

    logger& operator() (const lvl& l)
    {
        m_level = l;
        return *this;
    }

    logger& operator() (const base& b)
    {
        m_base = b;
        return *this;
    }

    logger& operator() (const align& a)
    {
        m_align = a;
        return *this;
    }

    logger& operator() (const size_t w)
    {
        m_width = w;
        return *this;
    }

    logger& operator<< (const base& b)
    {
        m_base = b;
        return *this;
    }

    logger& operator<< (const align& a)
    {
        m_align = a;
        return *this;
    }

    // logger& operator<< (const size_t w)
    // {
    //     m_width = w;
    //     return *this;
    // }

    logger& operator<< (const reset& r)
    {
        (void)(r);
        reset();
    }

    logger& operator<< (const end& end)
    {
        (void)(end);
        out_all();

        reset();

        return *this;
    }

    template<typename T>
    logger& operator<< (const T& val)
    {
        print_val(val);
        return *this;
    }

    operator const char* ()                                                const
    {
        return m_buf;
    }

    operator const char* ()
    {
        return m_buf;
    }

    constexpr static size_t m_s_size { BUFFER_SIZE };

private:
    logger(const logger& other)                                          = delete;
    logger& operator= (const logger& other)                              = delete;
    logger(const logger&& other)                                         = delete;
    logger& operator= (const logger&& other)                             = delete;

    void reset()
    {
        for(auto &c: m_buf) {
            c = 0;
        }

        m_count = 0;

        m_base =  m_def_base;
        m_align = m_def_align;
        m_width = m_def_width;
        m_level = m_def_level;
    }

    template<typename T>
    void print_num_signed(const T& val)
    {
        if(val < 0) {
            if(base::d == m_base) {
                print_val('-');
                print_num_unsigned(-val);
            } else {
                typename std::make_unsigned<T>::type uval = val;
                // typename signed_to_unsigned<T>::type uval = val;
                print_num_unsigned(uval);
            }
        } else {
            print_num_unsigned(val);
        }
    }

    template<typename T>
    void print_num_unsigned(const T& val)
    {
        T tmp = val;

        for(auto &c : m_num_buf) {
            c = 0;
        }

        uint8_t bs = 0;
        uint8_t p = 0;

        const alphabet_char_t* p_alphabet = nullptr;

        switch(m_base) {
            case base::b:
                p_alphabet = m_s_binary_alphabet;
                bs = 2;
            break;
            case base::o:
                p_alphabet = m_s_octal_alphabet;
                bs = 8;
            break;
            case base::d:
                p_alphabet = m_s_decimal_alphabet;
                bs = 10;
            break;
            case base::h:
                p_alphabet = m_s_hex_alphabet;
                bs = 16;
            break;
        }

        do {
            m_num_buf[p] = p_alphabet[tmp % bs].low;
            tmp /= bs;
            ++p;
        } while(0 != tmp);

        for(uint8_t i = 0; i < (p / 2); ++i) {
            char t = m_num_buf[p - i - 1];
            m_num_buf[p - i - 1] = m_num_buf[i];
            m_num_buf[i] = t;
        }

        print_val((const char* const)m_num_buf);
    }

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
        if(val) {
            print_val("true");
        } else {
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
        for(size_t i = 0; (val[i] != 0) && (m_count < BUFFER_SIZE); ++i) {
            m_buf[m_count] = val[i];
            ++m_count;
        }

        m_buf[BUFFER_SIZE - 1] = 0;
    }

    template<typename T>
    void print_float(const T& val)
    {
        T i = 0.0;
        T f = 0.0;
        f = std::modf(val, &i);

        print_num_signed((int64_t)i);
        print_val('.');

        print_num_unsigned((uint64_t)(f * std::pow(10, m_width)));
    }

    template<typename T>
    void print_val(const T& val)
    {
        T::print(*this, val); // custom types should support print function
    }

    void out_all()                                                         const
    {
        out<0, out_t...>();
    }

    template<size_t COUNT, typename o, typename... tail>
    bool out()                                                             const
    {
        bool res_self = o::print(m_buf);
        bool res_tail = out<COUNT + 1, tail...>();
        return res_self && res_tail;
    }

    template<size_t COUNT>
    bool out()                                                             const
    {
        static_assert((COUNT == sizeof...(out_t)), "Out count missmatch!");
        return true;
    }

    typedef struct alphabet_char {
        char low;
        char high;
    } alphabet_char_t;

    static alphabet_char_t m_s_binary_alphabet[];
    static alphabet_char_t m_s_octal_alphabet[];
    static alphabet_char_t m_s_decimal_alphabet[];
    static alphabet_char_t m_s_hex_alphabet[];

    char         m_num_buf[66]; // (u)int64_t in binary mode takes 64 characters 
    char         m_buf[BUFFER_SIZE];
    size_t       m_count;

    const base   m_def_base;
    const align  m_def_align;
    const size_t m_def_width;
    const lvl    m_def_level;

    base         m_base;
    align        m_align;
    size_t       m_width;
    lvl          m_level;
};

} // namespace ecl

#endif // ECL_LOG_HPP
