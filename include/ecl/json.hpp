#pragma once

#include <type_traits>
#include <array>
#include <tuple>

#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cinttypes>

/*
 * JSON string size impacts size calculations only.
 * In serialialization whole string will be serialized.
 * If serialized size of JSON document bigger than stream size,
 * stream flush callback will be called.
*/
#ifndef JSON_STRING_SIZE
#define JSON_STRING_SIZE 32
#endif

namespace ecl
{

namespace json
{

template<typename T>
struct val_size           { constexpr static size_t size() { return T::size(); } };
template<>
struct val_size<bool>     { constexpr static size_t size() { return 5;         } };
template<>
struct val_size<int8_t>   { constexpr static size_t size() { return 4;         } };
template<>
struct val_size<uint8_t>  { constexpr static size_t size() { return 3;         } };
template<>
struct val_size<int16_t>  { constexpr static size_t size() { return 6;         } };
template<>
struct val_size<uint16_t> { constexpr static size_t size() { return 5;         } };
template<>
struct val_size<int32_t>  { constexpr static size_t size() { return 11;        } };
template<>
struct val_size<uint32_t> { constexpr static size_t size() { return 10;        } };
template<>
struct val_size<int64_t>  { constexpr static size_t size() { return 21;        } };
template<>
struct val_size<uint64_t> { constexpr static size_t size() { return 20;        } };

template<typename T>
struct val_initializer           { constexpr static T           value { T()   }; };
template<>
struct val_initializer<bool>     { constexpr static bool        value { false }; };
template<>
struct val_initializer<int8_t>   { constexpr static int8_t      value { 0     }; };
template<>
struct val_initializer<uint8_t>  { constexpr static uint8_t     value { 0     }; };
template<>
struct val_initializer<int16_t>  { constexpr static int16_t     value { 0     }; };
template<>
struct val_initializer<uint16_t> { constexpr static uint16_t    value { 0     }; };
template<>
struct val_initializer<int32_t>  { constexpr static int32_t     value { 0     }; };
template<>
struct val_initializer<uint32_t> { constexpr static uint32_t    value { 0     }; };
template<>
struct val_initializer<int64_t>  { constexpr static int64_t     value { 0     }; };
template<>
struct val_initializer<uint64_t> { constexpr static uint64_t    value { 0     }; };

template<size_t SIZE>
class string
{
public:
    string()
    {
        m_val.fill('\0');
    }

    // constexpr string(const string<SIZE>& other) : m_val(other.m_val) {}

    constexpr static size_t size()
    {
        return SIZE;
    }

    template<typename STREAM>
    void serialize(STREAM& st)                                             const
    {
        st << "\"";

        for(size_t i = 0; i < strlen(m_val) && i < SIZE; ++i)
        {
            if(m_val[i] == '"'  ||
               m_val[i] == '\\' ||
               m_val[i] == '/')
            {
                st << "\\";
                st << m_val[i];
            }
            else if(m_val[i] == '\b')
            {
                st << "\\b";
            }
            else if(m_val[i] == '\f')
            {
                st << "\\f";
            }
            else if(m_val[i] == '\n')
            {
                st << "\\n";
            }
            else if(m_val[i] == '\r')
            {
                st << "\\r";
            }
            else if(m_val[i] == '\t')
            {
                st << "\\t";
            }
            else
            {
                st << m_val[i];
            }
        }

        st << "\"";
    }

    bool deserialize(const char*& s)
    {
        if(*s != '"')
        {
            return false;
        }
        s++;

        for(size_t i = 0; i < SIZE; ++i)
        {
            if(*s == '"')
            {
                s++;
                break;
            }

            if(*s == '\\' && *(s + 1) == '"')
            {
                s++;
            }

            m_val[i] = *s;
            s++;
        }

        return true;
    }

    string<SIZE>& operator= (string<SIZE>& other)
    {
        m_val = other.m_val;
        return *this;
    }

    string<SIZE>& operator= (const char* const ptr)
    {
        for(size_t i = 0; i < SIZE; ++i)
        {
            m_val[i] = ptr[i];
        }

        return *this;
    }

    operator char*()
    {
        return m_val.data();
    }

    operator const char*()                                                 const
    {
        return m_val.data();
    }

    char* data()
    {
        return m_val.data();
    }

    const char* data()                                                     const
    {
        return m_val.data();
    }

    std::array<char, SIZE + 1> m_val; // 1 for '0'
};

namespace
{
    template<typename T>
    struct val_deserializer_numeric_signed
    {
        static bool parse(const char*& s, T& val)
        {
            char* end = nullptr;
            val = static_cast<T>(strtoimax(s, &end, 10));
            s = end;
            return true;
        }
    };

    template<typename T>
    struct val_deserializer_numeric_unsigned
    {
        static bool parse(const char*& s, T& val)
        {
            char* end = nullptr;
            val = static_cast<T>(strtoumax(s, &end, 10));
            s = end;
            return true;
        }
    };

} // namespace

template<typename T>
struct val_deserializer
{
    static bool parse(const char*& s, T& val)
    {
        return val.deserialize(s);
    }
};

template<>
struct val_deserializer<bool>
{
    static bool parse(const char*& s, bool& val)
    {
        if(0 == strncmp(s, "true", 4))
        {
            val = true;
            s += 4;
            return true;
        }

        if(0 == strncmp(s, "false", 5))
        {
            val = false;
            s += 5;
            return true;
        }

        val = val_initializer<bool>::value;

        return false;
    }
};

// template<>
// struct val_deserializer<const char*>
// {
//     static bool parse(const char*& s, const char*& val)
//     {
//         if(*s != '"')
//         {
//             return false;
//         }
//         s++;

//         if(*s != '"')
//         {
//             return false;
//         }
//         s++;

//         return true;
//     }
// };

template<>
struct val_deserializer<int8_t>
{
    static bool parse(const char*& s, int8_t& val)
    {
        return val_deserializer_numeric_signed<int8_t>::parse(s, val);
    }
};

template<>
struct val_deserializer<uint8_t>
{
    static bool parse(const char*& s, uint8_t& val)
    {
        return val_deserializer_numeric_unsigned<uint8_t>::parse(s, val);
    }
};

template<>
struct val_deserializer<int16_t>
{
    static bool parse(const char*& s, int16_t& val)
    {
        return val_deserializer_numeric_signed<int16_t>::parse(s, val);
    }
};

template<>
struct val_deserializer<uint16_t>
{
    static bool parse(const char*& s, uint16_t& val)
    {
        return val_deserializer_numeric_unsigned<uint16_t>::parse(s, val);
    }
};

template<>
struct val_deserializer<int32_t>
{
    static bool parse(const char*& s, int32_t& val)
    {
        return val_deserializer_numeric_signed<int32_t>::parse(s, val);
    }
};

template<>
struct val_deserializer<uint32_t>
{
    static bool parse(const char*& s, uint32_t& val)
    {
        return val_deserializer_numeric_unsigned<uint32_t>::parse(s, val);
    }
};

template<>
struct val_deserializer<int64_t>
{
    static bool parse(const char*& s, int64_t& val)
    {
        return val_deserializer_numeric_signed<int64_t>::parse(s, val);
    }
};

template<>
struct val_deserializer<uint64_t>
{
    static bool parse(const char*& s, uint64_t& val)
    {
        return val_deserializer_numeric_unsigned<uint64_t>::parse(s, val);
    }
};

template<typename NAME, typename T>
class node
{
public:
    constexpr node() {}

    typedef T    value_t;
    typedef NAME name_t;

protected:
    template<typename STREAM>
    void serialize(STREAM& st)                                             const
    {
        stringify(st, NAME::name());
        st << ':';
        stringify(st, m_val);
    }

    bool deserialize(const char*& s)
    {
        if(*s != '"')
        {
            return false;
        }
        s++;

        if(0 != strncmp(s, NAME::name(), NAME::size()))
        {
            return false;
        }
        s += NAME::size();

        if(*s != '"')
        {
            return false;
        }
        s++;

        if(*s != ':')
        {
            return false;
        }
        s++;

        if(!val_deserializer<value_t>::parse(s, m_val))
        {
            return false;
        }

        return true;
    }

    // 1 -     ':'
    // 2 - 2 * '"'
    constexpr static size_t size()
    {
        return 1 + 2 + NAME::size() + val_size<value_t>::size();
    }

    value_t m_val { /*val_initializer<value_t>::value*/ };

private:
    template<typename STREAM, typename V_T>
    void stringify(STREAM& st, const V_T& val)                             const
    {
        st << val;
    }

    template<typename STREAM, size_t N>
    void stringify(STREAM& st, const string<N>& str)                       const
    {
        stringify(st, str.data());
    }

    template<typename STREAM>
    void stringify(STREAM& st, const char* val)                            const
    {
        st << "\"";

        for(size_t i = 0; i < strlen(val); ++i)
        {
            if(val[i] == '"'  ||
               val[i] == '\\' ||
               val[i] == '/')
            {
                st << "\\";
                st << val[i];
            }
            else if(val[i] == '\b')
            {
                st << "\\b";
            }
            else if(val[i] == '\f')
            {
                st << "\\f";
            }
            else if(val[i] == '\n')
            {
                st << "\\n";
            }
            else if(val[i] == '\r')
            {
                st << "\\r";
            }
            else if(val[i] == '\t')
            {
                st << "\\t";
            }
            else
            {
                st << val[i];
            }
        }

        st << "\"";
    }
};

template<typename OBJ, size_t COUNT>
class array
{
public:
    constexpr array() {}

    constexpr static size_t size()
    {
        return 2 + OBJ::size() * COUNT + (COUNT - 1); // 2 for '[' and ']'
    }

    bool deserialize(const char*& s)
    {
        if(*s != '[')
        {
            return false;
        }
        s++;

        for(size_t i = 0; i < COUNT; ++i)
        {
            m_val[i].disable();
            if(!m_val[i].deserialize(s))
            {
                break;
            }
            m_val[i].enable();

            if(i != COUNT - 1)
            {
                if(*s != ',')
                {
                    return false;
                }
                s++;
            }
        }

        if(*s != ']')
        {
            return false;
        }
        s++;

        return true;
    }

private:
    typedef std::array<OBJ, COUNT> array_t;
protected:
    template<typename STREAM>
    void serialize(STREAM& st)                                             const
    {
        st << '[';

        for(size_t i = 0; i < COUNT; ++i)
        {
            m_val[i].serialize(st);

            if(i != COUNT - 1)
            {
                if(m_val[i + 1].is_enabled())
                {
                    st << ',';
                }
            }
        }

        st << ']';
    }

public:
    template<typename STREAM, typename T>
    static void print(STREAM& st, const T& val)
    {
        val.serialize(st);
    }

    typename array_t::iterator begin()
    {
        return m_val.begin();
    }

    typename array_t::iterator end()
    {
        return m_val.end();
    }

    typename array_t::const_iterator begin()                               const
    {
        return m_val.begin();
    }

    typename array_t::const_iterator end()                                 const
    {
        return m_val.end();
    }

    typename array_t::reverse_iterator rbegin()
    {
        return m_val.rbegin();
    }

    typename array_t::reverse_iterator rend()
    {
        return m_val.rend();
    }

    typename array_t::const_reverse_iterator rbegin()                      const
    {
        return m_val.rbegin();
    }

    typename array_t::const_reverse_iterator rend()                        const
    {
        return m_val.rend();
    }

    OBJ& operator[](size_t index)
    {
        return m_val[index];
    }

    const OBJ& operator[](size_t index)                                    const
    {
        return m_val[index];
    }

protected:
    array_t m_val;
};

template<typename... NODES>
class object : public NODES...
{
private:
    template<typename NAME, typename N_NAME>
    struct name_predicate
    {
        constexpr static bool value
        {
            std::is_same<NAME, typename N_NAME::name_t>::value
        };
    };

    template <typename, typename> struct cons;

    template <typename  T, typename ...Args>
    struct cons<T, std::tuple<Args...>>
    {
        using type = std::tuple<T, Args...>;
    };

    template <template <class, class> class,
              typename,
              template <class...> class,
              class...>
    struct filter;

    template <template <class, class> class Pred,
              typename Val,
              template <class...> class Variadic>
    struct filter<Pred, Val, Variadic>
    {
        using type = Variadic<>;
    };

    template <template <class, class> class Pred,
              typename Val,
              template <class...> class Variadic,
              class T, class... Ts>
    struct filter<Pred, Val, Variadic, T, Ts...>
    {
        template <class, class> struct cons;
        template <class Head, class... Tail>
        struct cons<Head, Variadic<Tail...> >
        {
            using type = Variadic<Head, Tail...>;
        };

        using type = typename std::conditional<
            Pred<Val, T>::value,
            typename cons<T, typename filter<Pred, Val, Variadic, Ts...>::type>::type,
            typename filter<Pred, Val, Variadic, Ts...>::type >::type;
    };

public:
    constexpr object() {}

    void disable()
    {
        m_enabled = false;
    }

    void enable()
    {
        m_enabled = true;
    }

    bool is_enabled()                                                      const
    {
        return m_enabled;
    }

    template<typename NAME>
    typename std::tuple_element<0, typename filter<name_predicate, NAME, std::tuple, NODES...>::type>::type::value_t& f()
    {
        return this->std::tuple_element<0, typename filter<name_predicate, NAME, std::tuple, NODES...>::type>::type::m_val;
    }

    constexpr static size_t size()
    {
        return size_<2, NODES...>(); // 2 for '{' and '}'
    }

    template<typename STREAM, typename T>
    static void print(STREAM& st, const T& val)
    {
        val.serialize(st);
    }

    template<typename STREAM>
    void serialize(STREAM& st)                                             const
    {
        if(m_enabled)
        {
            st << '{';

            serialize_internal<STREAM, NODES...>(st);

            st << '}';
        }
    }

    bool deserialize(const char*& s)
    {
        if(*s != '{')
        {
            return false;
        }
        s++;

        if(!deserialize_internal<NODES...>(s))
        {
            return false;
        }

        if(*s != '}')
        {
            return false;
        }
        s++;

        return true;
    }

private:
    template<size_t SIZE, typename NODE, typename NODE_NEXT, typename... TAIL>
    constexpr static size_t size_()
    {
        return size_<SIZE + NODE::size() + 1, NODE_NEXT, TAIL...>(); // 1 for ','
    }

    template<size_t SIZE, typename NODE>
    constexpr static size_t size_()
    {
        return size_<SIZE + NODE::size()>();
    }

    template<size_t SIZE>
    constexpr static size_t size_()
    {
        return SIZE; // '}' already counted.
    }

    // Serialization
    template<typename STREAM, typename NODE, typename NODE_NEXT, typename... TAIL>
    void serialize_internal(STREAM& st)                                    const
    {
        this->NODE::serialize(st);

        st << ',';

        serialize_internal<STREAM, NODE_NEXT, TAIL...>(st);
    }

    template<typename STREAM, typename NODE>
    void serialize_internal(STREAM& st)                                    const
    {
        this->NODE::serialize(st);

        serialize_internal<STREAM>(st);
    }

    template<typename STREAM>
    void serialize_internal(STREAM& st)                                    const
    {
        (void)st;
    }

    // Deserialization
    template<typename NODE, typename NODE_NEXT, typename... TAIL>
    bool deserialize_internal(const char*& s)
    {
        if(!this->NODE::deserialize(s))
        {
            return false;
        }

        if(*s != ',')
        {
            return false;
        }
        s++;

        return deserialize_internal<NODE_NEXT, TAIL...>(s);
    }

    template<typename NODE>
    bool deserialize_internal(const char*& s)
    {
        if(!this->NODE::deserialize(s))
        {
            return false;
        }

        return deserialize_internal(s);
    }

    bool deserialize_internal(const char*& s)
    {
        (void)s;
        return true;
    }

    bool m_enabled { true };
};

} // namespace json

} // namespace ecl
