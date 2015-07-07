/**
 * @file
 *
 * @brief JSON helper functions and classes
 *
 * @defgroup json_internal Internal helper types and functions for JSON classes.
 * @{
 */
#ifndef ECL_JSON_HELPERS_HPP
#define ECL_JSON_HELPERS_HPP

#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cinttypes>

#ifdef ECL_WITH_STD_STRING
#include <string>
#endif

#include <ecl/json/string.hpp>

#ifndef ECL_DEFAULT_INDENT_INCREMENT
#define ECL_DEFAULT_INDENT_INCREMENT 4
#endif

namespace ecl
{

namespace json
{

namespace details
{

template<typename STREAM>
void print_beautify(STREAM& st,
                    bool beautify,
                    size_t indent,
                    size_t indent_increment)
{
    if(beautify)
    {
        st << '\n';
        for(size_t i = 0; i < indent * indent_increment; ++i)
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

template<typename T>
struct val_serializer
{
    template<typename STREAM>
    static void stringify(STREAM& st,
                          const T& val,
                          bool beautify,
                          size_t indent,
                          size_t indent_increment)
    {
        val.serialize(st, beautify, indent, indent_increment);
    }
};

template<>
struct val_serializer<const char*>
{
    template<typename STREAM>
    static void stringify(STREAM& st,
                          const char* val,
                          bool beautify,
                          size_t indent,
                          size_t indent_increment)
    {
        (void)beautify;
        (void)indent;
        (void)indent_increment;

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

template<>
struct val_serializer<bool>
{
    template<typename STREAM>
    static void stringify(STREAM& st,
                          const bool& val,
                          bool beautify,
                          size_t indent,
                          size_t indent_increment)
    {
        (void)beautify;
        (void)indent;
        (void)indent_increment;
        st << val;
    }
};

template<>
struct val_serializer<int8_t>
{
    template<typename STREAM>
    static void stringify(STREAM& st,
                          const int8_t& val,
                          bool beautify,
                          size_t indent,
                          size_t indent_increment)
    {
        (void)beautify;
        (void)indent;
        (void)indent_increment;
        st << val;
    }
};

template<>
struct val_serializer<uint8_t>
{
    template<typename STREAM>
    static void stringify(STREAM& st,
                          const uint8_t& val,
                          bool beautify,
                          size_t indent,
                          size_t indent_increment)
    {
        (void)beautify;
        (void)indent;
        (void)indent_increment;
        st << val;
    }
};

template<>
struct val_serializer<int16_t>
{
    template<typename STREAM>
    static void stringify(STREAM& st,
                          const int16_t& val,
                          bool beautify,
                          size_t indent,
                          size_t indent_increment)
    {
        (void)beautify;
        (void)indent;
        (void)indent_increment;
        st << val;
    }
};

template<>
struct val_serializer<uint16_t>
{
    template<typename STREAM>
    static void stringify(STREAM& st,
                          const uint16_t& val,
                          bool beautify,
                          size_t indent,
                          size_t indent_increment)
    {
        (void)beautify;
        (void)indent;
        (void)indent_increment;
        st << val;
    }
};

template<>
struct val_serializer<int32_t>
{
    template<typename STREAM>
    static void stringify(STREAM& st,
                          const int32_t& val,
                          bool beautify,
                          size_t indent,
                          size_t indent_increment)
    {
        (void)beautify;
        (void)indent;
        (void)indent_increment;
        st << val;
    }
};

template<>
struct val_serializer<uint32_t>
{
    template<typename STREAM>
    static void stringify(STREAM& st,
                          const uint32_t& val,
                          bool beautify,
                          size_t indent,
                          size_t indent_increment)
    {
        (void)beautify;
        (void)indent;
        (void)indent_increment;
        st << val;
    }
};

template<>
struct val_serializer<int64_t>
{
    template<typename STREAM>
    static void stringify(STREAM& st,
                          const int64_t& val,
                          bool beautify,
                          size_t indent,
                          size_t indent_increment)
    {
        (void)beautify;
        (void)indent;
        (void)indent_increment;
        st << val;
    }
};

template<>
struct val_serializer<uint64_t>
{
    template<typename STREAM>
    static void stringify(STREAM& st,
                          const uint64_t& val,
                          bool beautify,
                          size_t indent,
                          size_t indent_increment)
    {
        (void)beautify;
        (void)indent;
        (void)indent_increment;
        st << val;
    }
};

#ifdef ECL_WITH_STD_STRING
template<>
struct val_serializer<std::string>
{
    template<typename STREAM>
    static void stringify(STREAM& st, const
                          std::string& val,
                          bool beautify,
                          size_t indent,
                          size_t indent_increment)
    {
        val_serializer<const char*>::stringify(st,
                                               val.data(),
                                               beautify,
                                               indent,
                                               indent_increment);
    }
};
#endif

} // namespace details

} // namespace json

} // namespace ecl

/**
 * @}
 */

#endif // ECL_JSON_HELPERS_HPP
