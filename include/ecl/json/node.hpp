#ifndef ECL_JSON_NODE_HPP
#define ECL_JSON_NODE_HPP

#include <ecl/json/helpers.hpp>
#include <ecl/json/object.hpp>

namespace ecl
{

namespace json
{

template<typename NAME, typename T>
class node
{
public:
    constexpr node() {}

    typedef T    value_t;
    typedef NAME name_t;

protected:
    template<typename STREAM>
    void serialize(STREAM& st,
                   bool beautify,
                   size_t indent,
                   size_t indent_increment)                                const
    {
        val_serializer<const char*>::template stringify<STREAM>(st,
                                                                NAME::name(),
                                                                beautify,
                                                                indent,
                                                                indent_increment);
        st << ':';
        val_serializer<value_t>::template stringify<STREAM>(st,
                                                            m_val,
                                                            beautify,
                                                            indent,
                                                            indent_increment);
    }

    bool deserialize(const char*& s)
    {
        spaces_rollup(s);
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

        spaces_rollup(s);
        if(*s != '"')
        {
            return false;
        }
        s++;

        spaces_rollup(s);
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

    value_t m_val { val_initializer<value_t>::value() };
};

} // namespace json

} // namespace ecl

#endif // ECL_JSON_NODE_HPP
