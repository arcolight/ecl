#ifndef ECL_STATIC_MAP
#define ECL_STATIC_MAP

#include <utility>
#include <type_traits>
#include <array>

#include <ecl/tree/red_black_tree.hpp>

namespace ecl
{

template<typename K, typename V, std::size_t N, typename Compare = std::less<K>>
class static_map
{
    using tree_t                 = tree::red_black_tree<K, V>;
    using tree_node_t            = typename tree_t::node_t;

public:
    using key_type               = typename tree_t::key_type;
    using mapped_type            = typename tree_t::value_type;
    using value_type             = std::pair<K, V>;
    using key_compare            = Compare;

    using const_iterator         = typename tree_t::const_iterator;
    using const_reverse_iterator = typename tree_t::const_reverse_iterator;

    template<typename... Args>
    explicit static_map (mapped_type&& def, Args&&... args) :
        m_nodes     { { std::forward<Args>(args)... } },
        m_not_found { std::forward<mapped_type>(def) }
    {
        build_tree();

        static_assert(std::is_nothrow_default_constructible<mapped_type>::value,
            "Value type should be nothrow default constructible.");
    }

    inline void build_tree()                                      const noexcept
    {
        for(std::size_t i = 0; i < N; ++i)
        {
            m_tree.insert(&m_nodes[i]);
        }
    }

    const mapped_type& operator[](const key_type& k)              const noexcept
    {
        const_iterator i = search(k);

        if(i == static_cast<const tree_t*>(&m_tree)->end())
        {
            return not_found();
        }

        return *i;
    }

    const mapped_type& operator[](key_type&& k)                   const noexcept
    {
        const_iterator i = search(k);

        if(i == static_cast<const tree_t*>(&m_tree)->end())
        {
            return not_found();
        }

        return *i;
    }

    std::size_t size()                                            const noexcept
    {
        return m_tree.count();
    }

    const V& not_found()                                          const noexcept
    {
        return m_not_found;
    }

    const_iterator begin()                                        const noexcept
    {
        return static_cast<const tree_t*>(&m_tree)->begin();
    }

    const_iterator end()                                          const noexcept
    {
        return static_cast<const tree_t*>(&m_tree)->end();
    }

    const_reverse_iterator rbegin()                               const noexcept
    {
        return static_cast<const tree_t*>(&m_tree)->rbegin();
    }

    const_reverse_iterator rend()                                 const noexcept
    {
        return static_cast<const tree_t*>(&m_tree)->rend();
    }

private:
    const_iterator search(key_type&& k)                           const noexcept
    {
        return static_cast<const tree_t*>(&m_tree)->find(
            std::forward<key_type>(k)
        );
    }

    const_iterator search(const key_type& k)                      const noexcept
    {
        return static_cast<const tree_t*>(&m_tree)->find(k);
    }

    mutable tree_t                     m_tree      {};
    mutable std::array<tree_node_t, N> m_nodes     {};

    const mapped_type                  m_not_found {};
};

template
<
      typename K
    , typename V
    , typename... Args
    , typename Compare = std::less<K>
>
constexpr inline const static_map
                       <
                             K
                           , V
                           , sizeof...(Args)
                           , Compare
                       > create_map(V&& def, Args&&... args)
{
    return static_map
           <
                 K
               , V
               , sizeof...(Args)
               , Compare
           >(std::forward<V>(def), std::forward<Args>(args)...);
}

} // namespace ecl

#endif // ECL_STATIC_MAP
