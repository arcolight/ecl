#ifndef ECL_JSON_ARRAY_HPP
#define ECL_JSON_ARRAY_HPP

#include <array>

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
                if(*s != ',')
                {
                    break;
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
    array_t m_val { { } };
};

} // namespace json

} // namespace ecl

#endif // ECL_JSON_ARRAY_HPP
