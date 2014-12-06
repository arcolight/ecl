#pragma once

namespace ecl
{

template<typename KEY, typename VALUE>
struct pair
{
    typedef KEY   key_t;
    typedef VALUE value_t;
};

template<typename PAIR, typename PAIR::key_t K, typename PAIR::value_t V>
struct entry
{
    constexpr static typename PAIR::key_t   key()   { return m_key; }
    constexpr static typename PAIR::value_t value() { return m_value; }

    constexpr static typename PAIR::key_t   m_key   { K };
    constexpr static typename PAIR::value_t m_value { V };
};

template<typename    PAIR,
         typename    PAIR::value_t DUMMY_VAL,
         typename... ENTR>
class static_map
{
public:
    template<typename PAIR::key_t K>
    constexpr static typename PAIR::value_t get()
    {
        return get_internal<K, ENTR...>();
    }

private:
    template<typename PAIR::key_t K, typename E, typename... TAIL>
    constexpr static typename PAIR::value_t get_internal()
    {
        return (K == E::key()) ? E::value() : get_internal<K, TAIL...>();
    }

    template<typename PAIR::key_t K>
    constexpr static typename PAIR::value_t get_internal()
    {
        return m_dummy;
    }

    constexpr static typename PAIR::value_t m_dummy { DUMMY_VAL };
};

} // namespace ecl
