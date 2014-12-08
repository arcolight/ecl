#pragma once

#include <type_traits>
#include <array>
#include <tuple>

#include <cstdint>
#include <cstring>
#include <cstdio>

#include "log.hpp"

namespace ecl
{

namespace json
{

// bool serialize_val(const char& val, uint8_t* buf, size_t sz, uintptr_t& offset)
// {
//     if(sz == 0)
//     {
//         return false;
//     }

//     *buf = val;
//     offset += 1;
//     return true;
// }

// bool serialize_val(const char* const val, uint8_t* buf, size_t sz, uintptr_t& offset)
// {
//     size_t len = strlen(val);
//     printf("val: %s | len: %u\r\n", val, len);
//     if(len > sz)
//     {
//         return false;
//     }

//     snprintf((char*)buf, len + 1, val);
//     printf("%s\r\n", buf);
//     offset += len;
//     printf("offset: %u\n\r", offset);

//     return true;
// }

// bool serialize_val(const bool& val, uint8_t* buf, size_t sz, uintptr_t& offset)
// {
//     if(val)
//     {
//         return serialize_val("true", buf, sz, offset);
//     }
//     else
//     {
//         return serialize_val("false", buf, sz, offset);
//     }
// }

// template<typename T>
// bool serialize_val(const T& val, uint8_t* buf, size_t sz, uintptr_t& offset)
// {
//     return true;
// }

template<typename NAME, typename T>
class node
{
protected:
    typedef T    value_t;
    typedef NAME name_t;

    template<typename BUFFER>
    bool serialize(BUFFER& buf)                                            const
    {
        stringify(buf, NAME::name());
        buf << ':';
        stringify(buf, m_val);

        return true;
    }

    value_t m_val;

private:
    template<typename BUFFER, typename V_T>
    void stringify(BUFFER& buf, const V_T& val)                            const
    {
        buf << val;        
    }

    template<typename BUFFER>
    void stringify(BUFFER& buf, const char* val)                           const
    {
        buf << "\"" << val << "\"";
    }
};

template<typename OBJ, size_t COUNT>
class array
{
private:
    typedef std::array<OBJ, COUNT> array_t;
protected:
    template<typename BUFFER>
    bool serialize(BUFFER& buf)                                            const
    {
        buf << '[';

        bool result = true;

        for(size_t i = 0; i < COUNT; ++i)
        {
            bool res_o = m_val[i].serialize(buf);
            if(!res_o)
            {
                result = false;
            }

            if(i != COUNT - 1)
            {
                buf << ',';
            }
        }

        buf << ']';

        return result;
    }

public:
    template<typename BUFFER, typename T>
    static void print(BUFFER& buf, const T& val)
    {
        val.serialize(buf);
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
    template<typename BUFFER, typename T>
    static void print(BUFFER& buf, const T& val)
    {
        val.serialize(buf);
    }

    template<typename BUFFER>
    bool serialize(BUFFER& buf)                                            const
    {
        buf << '{';

        return serialize_internal<BUFFER, NODES...>(buf);
    }

    template<typename NAME>
    typename std::tuple_element<0, typename filter<name_predicate, NAME, std::tuple, NODES...>::type>::type::value_t& f()
    {
        return this->std::tuple_element<0, typename filter<name_predicate, NAME, std::tuple, NODES...>::type>::type::m_val;
    }

private:
    template<typename BUFFER, typename NODE, typename NODE_NEXT, typename... TAIL>
    bool serialize_internal(BUFFER& buf)                                   const
    {
        if(!this->NODE::serialize(buf))
        {
            return false;
        }

        buf << ',';

        return serialize_internal<BUFFER, NODE_NEXT, TAIL...>(buf);
    }

    template<typename BUFFER, typename NODE>
    bool serialize_internal(BUFFER& buf)                                   const
    {
        if(!this->NODE::serialize(buf))
        {
            return false;
        }

        buf << '}';

        return true;
    }
};

} // namespace json
    
} // namespace ecl
