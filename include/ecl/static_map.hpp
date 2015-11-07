#ifndef ECL_STATIC_MAP
#define ECL_STATIC_MAP

#include <utility>
#include <type_traits>
#include <array>

namespace ecl
{

template<typename K, typename V, std::size_t N>
class static_map
{
public:
    using pair_t = std::pair<K, V>;

    template<typename... Args>
    constexpr explicit static_map (V&& not_found_element,
                                   Args&&... args) :
        m_pairs     { { args... } },
        m_not_found { not_found_element }
    {
        static_assert(std::is_nothrow_default_constructible<V>::value,
            "Value type should be nothrow default constructible.");
    }

    const V& operator[](const K& k)                               const noexcept
    {
        for(auto& p : m_pairs)
        {
            if(p.first == k)
            {
                return p.second;
            }
        }

        return not_found();
    }

    constexpr static std::size_t size()                                 noexcept
    {
        return N;
    }

    const V& not_found()                                          const noexcept
    {
        return m_not_found;
    }

    const pair_t* begin()                                         const noexcept
    {
        return m_pairs.begin();
    }

    const pair_t* end()                                           const noexcept
    {
        return m_pairs.end();
    }

    const pair_t* rbegin()                                        const noexcept
    {
        return m_pairs.rbegin();
    }

    const pair_t* rend()                                          const noexcept
    {
        return m_pairs.rend();
    }

private:
    const std::array<pair_t, N> m_pairs {};
    const V m_not_found                 {};
};

template<typename K, typename V, typename... Args>
constexpr inline static_map<K, V, sizeof...(Args)> create_map(
                                                       V&& not_found_element,
                                                       Args&&... args)
{
    return static_map<K, V, sizeof...(Args)>(std::forward<V>(not_found_element),
                                             std::forward<Args>(args)...);
}

} // namespace ecl

#endif // ECL_STATIC_MAP
