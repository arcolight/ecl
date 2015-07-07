#ifndef ECL_JSON_OBJECT_HPP
#define ECL_JSON_OBJECT_HPP

#include <type_traits>
#include <tuple>

#include <ecl/json/helpers.hpp>

namespace ecl
{

namespace json
{

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
            typename cons<T, typename filter<Pred,
                                             Val,
                                             Variadic,
                                             Ts...>::type>::type,
            typename filter<Pred, Val, Variadic, Ts...>::type >::type;
    };

public:
    constexpr object() {}

    void disable()
    {
        m_enabled = false;
    }

    void enable()
    {
        m_enabled = true;
    }

    bool is_enabled()                                                      const
    {
        return m_enabled;
    }

    template<typename NAME>
    typename std::tuple_element<0,
        typename filter<name_predicate,
                        NAME,
                        std::tuple,
                        NODES...>::type>::type::value_t& f()
    {
        return this->std::tuple_element<0,
            typename filter<name_predicate,
                            NAME,
                            std::tuple,
                            NODES...>::type>::type::m_val;
    }

    constexpr static size_t size()
    {
        return size_<2, NODES...>(); // 2 for '{' and '}'
    }

    template<typename STREAM, typename T>
    static void print(STREAM& st, const T& val)
    {
        val.serialize(st);
    }

    template<typename STREAM>
    void serialize(STREAM& st, bool beautify = false, size_t indent = 0)   const
    {
        if(m_enabled)
        {
            st << '{';
            print_beautify(st, beautify, indent + 1);

            serialize_internal<STREAM, NODES...>(st, beautify, indent + 1);

            print_beautify(st, beautify, indent);
            st << '}';
        }
    }

    bool deserialize_ref(const char*& s)
    {
        spaces_rollup(s);
        if(*s != '{')
        {
            return false;
        }
        s++;
        spaces_rollup(s);

        if(!deserialize_internal<NODES...>(s))
        {
            return false;
        }

        spaces_rollup(s);
        if(*s != '}')
        {
            return false;
        }
        s++;
        spaces_rollup(s);

        return true;
    }

    bool deserialize(const char* s)
    {
        const char* ptr = s;
        return deserialize_ref(ptr);
    }

private:
    template<size_t SIZE, typename NODE, typename NEXT, typename... TAIL>
    constexpr static size_t size_()
    {
        return size_<SIZE + NODE::size() + 1, NEXT, TAIL...>(); // 1 for ','
    }

    template<size_t SIZE, typename NODE>
    constexpr static size_t size_()
    {
        return size_<SIZE + NODE::size()>();
    }

    template<size_t SIZE>
    constexpr static size_t size_()
    {
        return SIZE; // '}' already counted.
    }

    // Serialization
    template<typename STREAM, typename NODE, typename NEXT, typename... TAIL>
    void serialize_internal(STREAM& st, bool beautify, size_t indent)      const
    {
        this->NODE::serialize(st, beautify, indent);

        st << ',';
        print_beautify(st, beautify, indent);

        serialize_internal<STREAM, NEXT, TAIL...>(st, beautify, indent);
    }

    template<typename STREAM, typename NODE>
    void serialize_internal(STREAM& st, bool beautify, size_t indent)      const
    {
        this->NODE::serialize(st, beautify, indent);

        serialize_internal<STREAM>(st, beautify, indent);
    }

    template<typename STREAM>
    void serialize_internal(STREAM& st, bool beautify, size_t indent)      const
    {
        (void)st;
        (void)beautify;
        (void)indent;
    }

    // Deserialization
    template<typename NODE, typename NEXT, typename... TAIL>
    bool deserialize_internal(const char*& s)
    {
        if(!this->NODE::deserialize(s))
        {
            return false;
        }

        spaces_rollup(s);
        if(*s != ',')
        {
            return false;
        }
        s++;

        return deserialize_internal<NEXT, TAIL...>(s);
    }

    template<typename NODE>
    bool deserialize_internal(const char*& s)
    {
        if(!this->NODE::deserialize(s))
        {
            return false;
        }

        return deserialize_internal(s);
    }

    bool deserialize_internal(const char*& s)
    {
        (void)s;
        return true;
    }

    bool m_enabled { true };
};

} // namespace json

} // namespace ecl

#endif // ECL_JSON_OBJECT_HPP
