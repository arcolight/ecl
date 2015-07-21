/**
 * @file
 *
 * @brief JSON object class template.
 *
 * @ingroup json ecl
 * @{
 */
#ifndef ECL_JSON_OBJECT_HPP
#define ECL_JSON_OBJECT_HPP

#include <type_traits>
#include <tuple>

#include <ecl/json/helpers.hpp>

namespace ecl
{

namespace json
{

/**
 * @brief JSON object class.
 * @details Top-level class is always object.
 * also objects can be value in key-value pair or element of array:
 * @code
 * {
 *     ...
 *     "val_name":{
 *          ...
 *     }
 *     ...
 *     "ar_name":[
 *         {
 *             ...
 *         },
 *         {
 *             ...
 *         }
 *     ]
 * }
 * @endcode
 * @tparam NODES Internal key-value pairs (See @ref node) list.
 */
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
    constexpr object()
    {
        static_assert(check_names_static<NODES...>(),
            "There are nodes with the same names in one level!");
    }

    /**
     * @brief Object disabling.
     * @details If object disabled, it will not be serialized.
     */
    void disable()
    {
        m_enabled = false;
    }

    /**
     * @brief Object enabling.
     * @details If object enabled, it will be serialized.
     */
    void enable()
    {
        m_enabled = true;
    }

    bool is_enabled()                                                      const
    {
        return m_enabled;
    }

    /**
     * @brief Field accessor.
     * @details Returns reference to field, that associated with
     * name NAME. Search happening in compile-time. If NAME is wrong,
     * compilation will be failed.
     *
     * @tparam NAME name type, that used for search element.
     * @return Reference to value with name NAME.
     */
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

    /**
     * @brief Serialized size in chars.
     * @details Calculates maximum size of serialized object in characters.
     * @return Serialized size in chars.
     */
    constexpr static std::size_t size()
    {
        return size_<2, NODES...>(); // 2 for '{' and '}'
    }

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
        if(m_enabled)
        {
            st << '{';
            details::print_beautify(st, beautify, indent + 1, indent_increment);

            serialize_internal<STREAM, NODES...>(st,
                                                 beautify,
                                                 indent + 1,
                                                 indent_increment);

            details::print_beautify(st, beautify, indent, indent_increment);
            st << '}';
        }
    }

    /**
     * @brief Deserialization from reference to char pointer.
     * @details Used inside JSON objects, but can be used from client code.
     * Pointer will be moved to last
     * successfully parsed position in string.
     *
     * @param s reference to serialized JSON string.
     * @return true - deserialization successful, false - unsuccessful.
     */
    bool deserialize(const char*& s)
    {
        details::spaces_rollup(s);
        if(*s != '{')
        {
            return false;
        }
        s++;
        details::spaces_rollup(s);

        if(!deserialize_internal<NODES...>(s))
        {
            return false;
        }

        details::spaces_rollup(s);
        if(*s != '}')
        {
            return false;
        }
        s++;
        details::spaces_rollup(s);

        return true;
    }

private:
    template<std::size_t SIZE, typename NODE, typename NEXT, typename... TAIL>
    constexpr static std::size_t size_()
    {
        return size_<SIZE + NODE::size() + 1, NEXT, TAIL...>(); // 1 for ','
    }

    template<std::size_t SIZE, typename NODE>
    constexpr static std::size_t size_()
    {
        return size_<SIZE + NODE::size()>();
    }

    template<std::size_t SIZE>
    constexpr static std::size_t size_()
    {
        return SIZE; // '}' already counted.
    }

    // Serialization
    template<typename STREAM, typename NODE, typename NEXT, typename... TAIL>
    void serialize_internal(STREAM& st,
                            bool beautify,
                            std::size_t indent,
                            std::size_t indent_increment)                  const
    {
        this->NODE::serialize(st, beautify, indent, indent_increment);

        st << ',';
        details::print_beautify(st, beautify, indent, indent_increment);

        serialize_internal<STREAM, NEXT, TAIL...>(st,
                                                  beautify,
                                                  indent,
                                                  indent_increment);
    }

    template<typename STREAM, typename NODE>
    void serialize_internal(STREAM& st,
                            bool beautify,
                            std::size_t indent,
                            std::size_t indent_increment)                  const
    {
        this->NODE::serialize(st, beautify, indent, indent_increment);

        serialize_internal<STREAM>(st, beautify, indent, indent_increment);
    }

    template<typename STREAM>
    void serialize_internal(STREAM& st,
                            bool beautify,
                            std::size_t indent,
                            std::size_t indent_increment)                  const
    {
        (void)st;
        (void)beautify;
        (void)indent;
        (void)indent_increment;
    }

    // Deserialization
    template<typename NODE, typename NEXT, typename... TAIL>
    bool deserialize_internal(const char*& s)
    {
        if(!this->NODE::deserialize(s))
        {
            return false;
        }

        details::spaces_rollup(s);
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

    template<typename NODE, typename NEXT, typename... TAIL>
    constexpr static bool check_names_static()
    {
        return (! std::is_same<typename NODE::name_t,
                               typename NEXT::name_t>::value) &&
                check_names_static<NEXT, TAIL...>();
    }

    template<typename NODE>
    constexpr static bool check_names_static()
    {
        return true;
    }

    constexpr static bool check_names_static()
    {
        return true;
    }

    bool m_enabled { true };
};

} // namespace json

} // namespace ecl

/**
 * @}
 */

#endif // ECL_JSON_OBJECT_HPP
