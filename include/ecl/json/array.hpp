/**
 * @file
 *
 * @brief JSON array class template.
 *
 * @ingroup json ecl
 */
#ifndef ECL_JSON_ARRAY_HPP
#define ECL_JSON_ARRAY_HPP

#include <array>
#include <ecl/json/helpers.hpp>

namespace ecl
{

namespace json
{

/**
 * @brief JSON array class.
 * @details JSON array class. Uses std::array as internal container.
 * Elements can be 'enabled' or 'disabled'. If element 'disabled' it will not be
 * serialized.
 *
 * @tparam OBJ JSON object type. Type of array element.
 * @tparam COUNT Elements count.
 */
template<typename OBJ, std::size_t COUNT>
class array
{
public:
    constexpr array() {}

    /**
     * @brief Maximum serialized size in characters.
     * @return Maximum serialized size in characters.
     */
    constexpr static std::size_t size()
    {
        return 2 + OBJ::size() * COUNT + (COUNT - 1); // 2 for '[' and ']'
    }

    /**
     * @brief Deserialization from reference to char pointer.
     * @details Deserialization from reference to char pointer. Used inside JSON
     * objects, but can be used from client code. Pointer will be moved to last
     * successfully parsed position in string.
     *
     * @param s reference to serialized JSON string.
     * @return true - deserialization successful, false - unsuccessful.
     */
    bool deserialize_ref(const char*& s, std::size_t& length, bool)
    {
        ECL_JSON_TEST_SYMBOL_SPACES_ROUND(s, '[', length, false)

        for(std::size_t i = 0; i < COUNT; ++i)
        {
            details::disable<OBJ>()(m_val[i]);
        }

        for(std::size_t i = 0; i < COUNT; ++i)
        {
            details::disable<OBJ>()(m_val[i]);
            if(! details::val_deserializer<OBJ>::parse(s, length, m_val[i]))
            {
                break;
            }
            details::enable<OBJ>()(m_val[i]);

            if(i != COUNT - 1)
            {
                ECL_JSON_TEST_SYMBOL_SPACES_ROUND_BREAK(s, ',', length)
            }
        }

        ECL_JSON_TEST_SYMBOL_SPACES_ROUND(s, ']', length, false)

        return true;
    }

private:
    using array_t = std::array<OBJ, COUNT>;

public:
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
                   bool beautify = false,
                   std::size_t indent = 0,
                   std::size_t indent_increment =
                               ECL_DEFAULT_INDENT_INCREMENT)               const
    {
        st << '[';
        details::print_beautify(st, beautify, indent + 1, indent_increment);

        for(std::size_t i = 0; i < COUNT; ++i)
        {
            details::val_serializer<OBJ>::stringify(st, m_val[i], beautify, indent + 1, indent_increment);

            if(i != COUNT - 1)
            {
                if(details::is_enabled<OBJ>()(m_val[i + 1]))
                {
                    st << ',';
                    details::print_beautify(st, beautify, indent + 1, indent_increment);
                }
            }
        }

        details::print_beautify(st, beautify, indent, indent_increment);
        st << ']';
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

    OBJ& operator[](std::size_t index)
    {
        return m_val[index];
    }

    const OBJ& operator[](std::size_t index)                               const
    {
        return m_val[index];
    }

protected:
    array_t m_val { { } };
};

} // namespace json

} // namespace ecl

#endif // ECL_JSON_ARRAY_HPP
