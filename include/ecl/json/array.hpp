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
     * @brief Deserialization from char pointer.
     *
     * @param s serialized JSON string.
     * @return true - deserialization successful, false - unsuccessful.
     */
    bool deserialize(const char* s)
    {
        const char* ptr = s;
        return deserialize_ref(ptr);
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
    bool deserialize_ref(const char*& s)
    {
        details::spaces_rollup(s);
        if(*s != '[')
        {
            return false;
        }
        s++;

        for(std::size_t i = 0; i < COUNT; ++i)
        {
            m_val[i].disable();
        }

        for(std::size_t i = 0; i < COUNT; ++i)
        {
            m_val[i].disable();
            if(!m_val[i].deserialize_ref(s))
            {
                break;
            }
            m_val[i].enable();

            if(i != COUNT - 1)
            {
                details::spaces_rollup(s);
                if(*s != ',')
                {
                    break;
                }
                s++;
            }
        }

        details::spaces_rollup(s);
        if(*s != ']')
        {
            return false;
        }
        s++;

        return true;
    }

private:
    typedef std::array<OBJ, COUNT> array_t;

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
            m_val[i].serialize(st, beautify, indent + 1, indent_increment);

            if(i != COUNT - 1)
            {
                if(m_val[i + 1].is_enabled())
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
