#ifndef ECL_JSON_ARRAY_HPP
#define ECL_JSON_ARRAY_HPP

#include <array>
#include <ecl/json/helpers.hpp>

namespace ecl
{

namespace json
{

template<typename OBJ, size_t COUNT>
class array
{
public:
    constexpr array() {}

    constexpr static size_t size()
    {
        return 2 + OBJ::size() * COUNT + (COUNT - 1); // 2 for '[' and ']'
    }

    bool deserialize(const char* s)
    {
        const char* ptr = s;
        return deserialize_ref(ptr);
    }

    bool deserialize_ref(const char*& s)
    {
        spaces_rollup(s);
        if(*s != '[')
        {
            return false;
        }
        s++;

        for(size_t i = 0; i < COUNT; ++i)
        {
            m_val[i].disable();
        }

        for(size_t i = 0; i < COUNT; ++i)
        {
            m_val[i].disable();
            if(!m_val[i].deserialize_ref(s))
            {
                break;
            }
            m_val[i].enable();

            if(i != COUNT - 1)
            {
                spaces_rollup(s);
                if(*s != ',')
                {
                    break;
                }
                s++;
            }
        }

        spaces_rollup(s);
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
    template<typename STREAM>
    void serialize(STREAM& st, bool beautify = false, size_t indent = 0)               const
    {
        st << '[';
        print_beautify(st, beautify, indent + 1);

        for(size_t i = 0; i < COUNT; ++i)
        {
            m_val[i].serialize(st, beautify, indent + 1);

            if(i != COUNT - 1)
            {
                if(m_val[i + 1].is_enabled())
                {
                    st << ',';
                    print_beautify(st, beautify, indent + 1);
                }
            }
        }

        print_beautify(st, beautify, indent);
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

    OBJ& operator[](size_t index)
    {
        return m_val[index];
    }

    const OBJ& operator[](size_t index)                                    const
    {
        return m_val[index];
    }

protected:
    array_t m_val { { } };
};

} // namespace json

} // namespace ecl

#endif // ECL_JSON_ARRAY_HPP
