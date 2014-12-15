#pragma once

#include <type_traits>
#include <array>
#include <tuple>

#include <cstdint>
#include <cstring>
#include <cstdio>

namespace ecl
{

namespace json
{

template<typename NAME, typename T>
class node
{
protected:
    typedef T    value_t;
    typedef NAME name_t;

    template<typename STREAM>
    void serialize(STREAM& st)                                             const
    {
        stringify(st, NAME::name());
        st << ':';
        stringify(st, m_val);
    }

    value_t m_val;

private:
    template<typename STREAM, typename V_T>
    void stringify(STREAM& st, const V_T& val)                             const
    {
        st << val;        
    }

    template<typename STREAM>
    void stringify(STREAM& st, const char* val)                            const
    {
        st << "\"" << val << "\"";
    }
};

template<typename OBJ, size_t COUNT>
class array
{
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
                st << ',';
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
    static size_t serialized_size()
    {
        return size_internal(0);
    }

    template<typename STREAM, typename T>
    static void print(STREAM& st, const T& val)
    {
        val.serialize(st);
    }

    template<typename STREAM>
    void serialize(STREAM& st)                                             const
    {
        st << '{';

        serialize_internal<STREAM, NODES...>(st);
    }

    template<typename NAME>
    typename std::tuple_element<0, typename filter<name_predicate, NAME, std::tuple, NODES...>::type>::type::value_t& f()
    {
        return this->std::tuple_element<0, typename filter<name_predicate, NAME, std::tuple, NODES...>::type>::type::m_val;
    }

private:
    template<size_t SIZE, typename NODE, typename NODE_NEXT, typename... TAIL>
    static size_t size_internal(size_t current)
    {
        return size_internal<NODE_NEXT, TAIL...>(current + NODE::size() + 1);
    }

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

        st << '}';
    }
};

} // namespace json
    
} // namespace ecl
