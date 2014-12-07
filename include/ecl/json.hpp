#pragma once

#include <type_traits>
#include <array>
#include <tuple>
#include <cstdint>

namespace ecl
{

template <typename, typename> struct Cons;

template <typename  T, typename ...Args>
struct Cons<T, std::tuple<Args...>>
{
    using type = std::tuple<T, Args...>;
};

template <template <class, class> class,
          typename,
          template <class...> class,
          class...>
struct filter;

template <template <class, class> class Pred, typename Val, template <class...> class Variadic>
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
    template <class, class> struct Cons;
    template <class Head, class... Tail>
    struct Cons<Head, Variadic<Tail...> >
    {
        using type = Variadic<Head, Tail...>;
    };
 
    using type = typename std::conditional<
        Pred<Val, T>::value,
        typename Cons<T, typename filter<Pred, Val, Variadic, Ts...>::type>::type,
        typename filter<Pred, Val, Variadic, Ts...>::type >::type;
};

namespace json
{

template<typename NAME, typename T>
class node
{
public:
    typedef T    value_t;
    typedef NAME name_t;

protected:
    bool _serialize(uint8_t* buf, size_t sz, uintptr_t& offset)            const
    {
        (void)buf;
        (void)sz;
        (void)offset;
        return true;//_write(NAME, m_val, buf, sz, offset);
    }

    value_t m_val;
};

template<typename NAME, typename N_NAME>
struct name_predicate
{
    constexpr static bool value { std::is_same<NAME, typename N_NAME::name_t>::value };
};

template<typename... NODES>
class object : public NODES...
{
public:
    bool serialize(uint8_t* buf, size_t sz)                                const
    {
        uintptr_t offset = 0;
        return serialize_internal<0, NODES...>(buf, sz, offset);
    }

    template<typename NAME>
    typename std::tuple_element<0, typename filter<name_predicate, NAME, std::tuple, NODES...>::type>::type::value_t& f()
    {
        return this->std::tuple_element<0, typename filter<name_predicate, NAME, std::tuple, NODES...>::type>::type::m_val;
    }

private:
    template<size_t COUNT, typename NODE, typename... TAIL>
    bool serialize_internal(uint8_t* buf, size_t sz, uintptr_t& offset)    const
    {
        this->NODE::_serialize(buf + offset, sz - offset, offset);

        return serialize_internal<COUNT + 1, TAIL...>(buf + offset, sz - offset, offset);
    }

    template<size_t COUNT>
    bool serialize_internal(uint8_t* buf, size_t sz, uintptr_t& offset)    const
    {
        (void)buf;
        (void)offset;
        (void)sz;
        return true;
    }
};

} // namespace json
    
} // namespace ecl
