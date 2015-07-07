#ifndef ECL_JSON_HELPERS_HPP
#define ECL_JSON_HELPERS_HPP

#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cinttypes>

#ifndef ECL_JSON_INDENT_MULTIPLIER
#define ECL_JSON_INDENT_MULTIPLIER 4
#endif

namespace ecl
{

namespace json
{

template<typename STREAM>
void print_beautify(STREAM& st, bool beautify, size_t indent)
{
    if(beautify)
    {
        st << '\n';
        for(size_t i = 0; i < indent * ECL_JSON_INDENT_MULTIPLIER; ++i)
        {
            st << ' ';
        }
    }
}

void spaces_rollup(const char*& s)
{
    while(*s == ' '  ||
          *s == '\n' ||
          *s == '\r' ||
          *s == '\t')
    {
        ++s;
    }
}

template<typename T>
struct val_size
{
    constexpr static size_t size()
    {
        return T::size();
    }
};

#ifdef ECL_WITH_STD_STRING
template<>
struct val_size<std::string>
{
    constexpr static size_t size()
    {
        return 0; // We don't know real size of this type
    }
};
#endif

template<>
struct val_size<bool>
{
    constexpr static size_t size()
    {
        return 5;
    }
};

template<>
struct val_size<int8_t>
{
    constexpr static size_t size()
    {
        return 4;
    }
};

template<>
struct val_size<uint8_t>
{
    constexpr static size_t size()
    {
        return 3;
    }
};

template<>
struct val_size<int16_t>
{
    constexpr static size_t size()
    {
        return 6;
    }
};

template<>
struct val_size<uint16_t>
{
    constexpr static size_t size()
    {
        return 5;
    }
};

template<>
struct val_size<int32_t>
{
    constexpr static size_t size()
    {
        return 11;
    }
};

template<>
struct val_size<uint32_t>
{
    constexpr static size_t size()
    {
        return 10;
    }
};

template<>
struct val_size<int64_t>
{
    constexpr static size_t size()
    {
        return 21;
    }
};

template<>
struct val_size<uint64_t>
{
    constexpr static size_t size()
    {
        return 20;
    }
};

template<typename T>
struct val_initializer
{
    constexpr static T value() { return T(); };
};

#ifdef ECL_WITH_STD_STRING
template<>
struct val_initializer<std::string>
{
    const static std::string value() { return ""; };
};
#endif

template<>
struct val_initializer<bool>
{
    constexpr static bool value() { return false; };
};

template<>
struct val_initializer<int8_t>
{
    constexpr static int8_t value() { return 0; };
};

template<>
struct val_initializer<uint8_t>
{
    constexpr static uint8_t  value() { return 0; };
};

template<>
struct val_initializer<int16_t>
{
    constexpr static int16_t  value() { return 0; };
};

template<>
struct val_initializer<uint16_t>
{
    constexpr static uint16_t value() { return 0; };
};

template<>
struct val_initializer<int32_t>
{
    constexpr static int32_t  value() { return 0; };
};

template<>
struct val_initializer<uint32_t>
{
    constexpr static uint32_t value() { return 0; };
};

template<>
struct val_initializer<int64_t>
{
    constexpr static int64_t  value() { return 0; };
};

template<>
struct val_initializer<uint64_t>
{
    constexpr static uint64_t value { 0 };
};

template<typename T>
struct val_deserializer_numeric_signed
{
    static bool parse(const char*& s, T& val)
    {
        char* end = nullptr;
        val = static_cast<T>(strtoll(s, &end, 10));
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
        val = static_cast<T>(strtoull(s, &end, 10));
        s = end;
        return true;
    }
};

template<typename T>
struct val_deserializer
{
    static bool parse(const char*& s, T& val)
    {
        return val.deserialize_ref(s);
    }
};

#ifdef ECL_WITH_STD_STRING
template<>
struct val_deserializer<std::string>
{
    static bool parse(const char*& s, std::string& val)
    {
        if(*s != '"')
        {
            return false;
        }
        s++;

        for(size_t i = 0; i < strlen(s); ++i)
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

            val.push_back(*s);
            s++;
        }

        return true;
    }
};
#endif

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

        val = val_initializer<bool>::value();

        return false;
    }
};

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

} // namespace json

} // namespace ecl

#endif // ECL_JSON_HELPERS_HPP
