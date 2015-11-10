#ifndef ECL_STATIC_MAP
#define ECL_STATIC_MAP

#include <utility>
#include <type_traits>
#include <array>

#include <ecl/rb_tree.hpp>

namespace ecl
{

template<typename K, typename V, std::size_t N, typename Compare = std::less<K>>
class static_map
{
    using tree_t                 = rb_tree<const K, V>;
    using tree_node_t            = typename tree_t::node_t;

public:
    using key_type               = typename tree_t::key_type;
    using mapped_type            = typename tree_t::value_type;
    using value_type             = std::pair<const K, V>;
    using key_compare            = Compare;

    using const_iterator         = typename tree_t::const_iterator;
    using const_reverse_iterator = typename tree_t::const_reverse_iterator;

    template<typename... Args>
    constexpr explicit static_map (Args&&... args) :
        // m_pairs { { std::forward<Args>(args)... } }
        m_nodes { { std::forward<Args>(args)... } }
    {
        static_assert(std::is_nothrow_default_constructible<mapped_type>::value,
            "Value type should be nothrow default constructible.");
    }

    void inline build_tree()
    {
        if(m_tree_init)
        {
            return;
        }

        for(std::size_t i = 0; i < N; ++i)
        {
            m_tree.insert(&m_nodes[i]);
        }

        m_tree_init = true;
    }

    const mapped_type& operator[](const key_type& k)              const noexcept
    {
        build_tree();

        const_iterator i = search(k);
        // for(auto& p : m_nodes)
        // {
        //     if(p.first == k)
        //     {
        //         return p.second;
        //     }
        // }

        return not_found();
    }

    const mapped_type& operator[](key_type&& k)                   const noexcept
    {
        build_tree();

        const_iterator i = search(k);
        // for(auto& p : m_nodes)
        // {
        //     if(p.first == k)
        //     {
        //         return p.second;
        //     }
        // }

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

    const const_iterator begin()                                  const noexcept
    {
        return m_nodes.begin();
    }

    const const_iterator end()                                    const noexcept
    {
        return m_nodes.end();
    }

    const const_reverse_iterator rbegin()                         const noexcept
    {
        return m_nodes.rbegin();
    }

    const const_reverse_iterator rend()                           const noexcept
    {
        return m_nodes.rend();
    }

private:
    const_iterator search(const key_type& k)
    {

    }

    bool                            m_tree_init { false };
    const mapped_type               m_not_found {};

    tree_t                          m_tree      {};
    std::array<tree_node_t, N>      m_nodes;

    // const std::array<value_type, N> m_pairs;
};

template
<
    typename K,
    typename V,
    typename... Args,
    typename Compare = std::less<K>
>
constexpr inline static_map
                 <
                     K,
                     V,
                     sizeof...(Args),
                     Compare
                 > create_map(Args&&... args)
{
    return static_map
           <
               K,
               V,
               sizeof...(Args),
               Compare
           >(std::forward<Args>(args)...);
}

} // namespace ecl

#endif // ECL_STATIC_MAP
