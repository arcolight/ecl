/**
 * @file
 *
 * @brief JSON node class template.
 *
 * @ingroup json ecl
 */
#ifndef ECL_JSON_NODE_HPP
#define ECL_JSON_NODE_HPP

#include <ecl/json/helpers.hpp>
#include <ecl/json/object.hpp>

namespace ecl
{

namespace json
{

/**
 * @brief Key-value pair class
 * @details Node class describes key-value pair, that can be serialized
 * and deserialized.
 * @code
 * {
 *     ...
 *     "node_name":node_value,
 *     ...
 * }
 * @endcode
 *
 * @tparam NAME Type, that have name() method, that returns const char* and
 * size() method, that returns size of name. Used for serialization and
 * field accessing
 * @tparam T Value type. Can be:
 * @code
 * bool
 * int8_t
 * uint8_t
 * int16_t
 * uint16_t
 * int32_t
 * uint32_t
 * int64_t
 * uint64_t
 * ecl::json::string<SIZE>
 * std::string (if ECL_WITH_STD_STRING defined)
 * ecl::json::object
 * ecl::json::array
 * @endcode
 */
template<typename NAME, typename T>
class node
{
public:
    constexpr node() {}

    typedef T    value_t;
    typedef NAME name_t;

protected:
    /**
     * @brief  Serialization to stream object.
     *
     * @tparam STREAM Stream type. Tested with ecl::stream.
     * Probably std:: streams should work.
     * @param st Reference to stream object.
     * @param beautify true - human readable format.
     * false - no new lines, no spaces.
     * @param indent base indent.
     * @param indent_increment Count of spaces on each indent level.
     */
    template<typename STREAM>
    void serialize(STREAM& st,
                   bool beautify,
                   std::size_t indent,
                   std::size_t indent_increment)                           const
    {
        details::val_serializer<const char*>::template stringify(st,
                                                                 NAME::name(),
                                                                 beautify,
                                                                 indent,
                                                                 indent_increment);
        st << ':';
        details::val_serializer<value_t>::template stringify(st,
                                                             m_val,
                                                             beautify,
                                                             indent,
                                                             indent_increment);
    }

    /**
     * @brief Deserialization from char pointer.
     *
     * @param s serialized JSON string.
     * @return true - deserialization successful, false - unsuccessful.
     */
    bool deserialize(const char*& s)
    {
        details::spaces_rollup(s);
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

        details::spaces_rollup(s);
        if(*s != '"')
        {
            return false;
        }
        s++;

        details::spaces_rollup(s);
        if(*s != ':')
        {
            return false;
        }
        s++;

        if(!details::val_deserializer<value_t>::parse(s, m_val))
        {
            return false;
        }

        return true;
    }

    // 1 -     ':'
    // 2 - 2 * '"'
    /**
     * @brief Serialized size in chars.
     * @details calculates maximum size of serialized node in characters.
     * Used for @ref ecl::json::object serialized size calculation.
     * @return serialized size in chars.
     */
    constexpr static std::size_t size()
    {
        return 1 + 2 + NAME::size() + details::val_size<value_t>::size();
    }

    value_t m_val { details::val_initializer<value_t>::value() };
};

} // namespace json

} // namespace ecl

#endif // ECL_JSON_NODE_HPP
