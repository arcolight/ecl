#include <initializer_list>
#include <utility>
#include <type_traits>

template<typename K, typename V>
class static_map
{
public:
    using pair_t = std::pair<K, V>;
    using list_t = std::initializer_list<pair_t>;

    static_map (const list_t& list) :
        m_list(list)
    {
        static_assert(std::is_default_constructible<V>::value,
            "Value type should be default constructible!");
    }

    const V& operator[](const K& k)                               const noexcept
    {
        static const V not_found { "not found" };

        for(auto& p : m_list)
        {
            if(p.first == k)
            {
                return p.second;
            }
        }

        return not_found;
    }

private:
    const list_t m_list;
};
