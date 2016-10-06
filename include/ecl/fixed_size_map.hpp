#ifndef ECL_map
#define ECL_map

#include <utility>
#include <type_traits>
#include <array>
#include <initializer_list>
#include <bitset>

#include <ecl/tree/red_black_tree.hpp>

namespace ecl
{

template<typename K, typename V, std::size_t N, typename Compare = std::less<K>>
class map
{
    using tree_t                  = tree::red_black_tree<K, V>;
    using tree_node_t             = typename tree_t::node_t;
    using tree_node_pointer_t     = typename std::add_pointer<tree_node_t>::type;
    using tree_node_reference_t   = typename std::add_lvalue_reference<tree_node_t>::type;

public:
    using key_type                = typename tree_t::key_type;
    using mapped_type             = typename tree_t::value_type;
    using value_type              = std::pair<key_type, mapped_type>;
    using size_type               = std::size_t;
    using key_compare             = Compare;
    using value_compare           = std::less<value_type>;

    using reference               = typename std::add_lvalue_reference<value_type>::type;
    using pointer                 = typename std::add_pointer<value_type>::type;
    using const_pointer           = typename std::add_pointer<const value_type>::type;

    using iterator                = typename tree_t::iterator;
    using reverse_iterator        = typename tree_t::reverse_iterator;

    using const_iterator          = typename tree_t::const_iterator;
    using const_reverse_iterator  = typename tree_t::const_reverse_iterator;

    map()                                                               noexcept
    {
        init();
    }

    template<typename... Args>
    explicit map (Args&&... args)                                       noexcept
        : m_nodes_pool { { std::forward<Args>(args)... } }
    {
        static_assert(std::is_nothrow_default_constructible<mapped_type>::value,
            "Mapped type should be nothrow default constructible.");

        init();

        for(auto& n : m_nodes_pool)
        {
            insert({ n.key, n.val });
        }
    }

    map(std::initializer_list<value_type> ilist)                        noexcept
    {
        init();

        for(auto& e : ilist)
        {
            insert(e);
        }
    }

    // map& operator=(const map& other)                                    noexcept
    // map& operator=(map&& other)                                         noexcept;
    map& operator=(std::initializer_list<value_type> ilist)             noexcept;

    mapped_type& at(const key_type& key)                                noexcept
    {
        return *search(key);
    }

    const mapped_type& at(const key_type& key)                    const noexcept
    {
        return *search(key);
    }

    bool empty()                                                  const noexcept
    {
        return (0 == size());
    }

    size_type size()                                              const noexcept
    {
        return m_tree.count();
    }

    size_type max_size()                                          const noexcept
    {
        return N;
    }

    void clear()                                                        noexcept
    {
        m_tree.clear();
    }

    std::pair<iterator, bool> insert(const value_type& value)           noexcept
    {
        tree_node_pointer_t ptr = get_free_node();

        if(nullptr == ptr)
        {
            return { end(), false };
        }

        ptr->key = value.first;
        ptr->val = value.second;
        iterator i = m_tree.insert(ptr);

        if(i != ptr)
        {
            return { i, false };
        }

        mark_as_used(*ptr);

        return { i, true };
    }

    template<class P>
    std::pair<iterator, bool> insert(P&& value)                         noexcept
    {
        insert(value_type(std::forward(value)));
    }

    template< class InputIt >
    void insert( InputIt first, InputIt last )                          noexcept;
    void insert( std::initializer_list<value_type> ilist )              noexcept;

    iterator erase( const_iterator pos )                                noexcept
    {
        return m_tree.erase(pos);
    }

    iterator erase( const_iterator first, const_iterator last )         noexcept
    {
        while(first != last)
        {
            erase(first);
            ++first;
        }

        return first;
    }

    size_type erase( const key_type& key )                              noexcept
    {
        pointer ptr = m_tree.erase(key);

        return (nullptr == ptr) ? 0 : 1;
    }

    void swap( map& other )                                             noexcept
    {
        std::swap(m_tree       , other.m_tree);
        std::swap(m_nodes_pool , other.m_nodes_pool);
        std::swap(m_not_found  , other.m_not_found);
    }

    size_type count( const key_type& key )                        const noexcept
    {
        return (search(key) != end()) ? 1 : 0;
    }

    iterator find( const key_type& key )                                noexcept
    {
        return search(key);
    }

    const_iterator find( const key_type& key )                    const noexcept
    {
        return search(key);
    }

    std::pair<iterator,iterator> equal_range( const key_type& key )     noexcept;
    std::pair<const_iterator,const_iterator>
    equal_range(const key_type& key )                             const noexcept;
    iterator lower_bound( const key_type& key )                         noexcept;
    const_iterator lower_bound( const key_type& key )             const noexcept;
    iterator upper_bound( const key_type& key )                         noexcept;
    const_iterator upper_bound( const key_type& key )             const noexcept;

    key_compare key_comp()                                        const noexcept
    {
        return key_compare();
    }

    value_compare value_comp()                                    const noexcept
    {
        return value_compare();
    }

    mapped_type& operator[](key_type&& k)                               noexcept
    {
        iterator i = search(std::forward<key_type>(k));

        if(i == end())
        {
            return *insert({ k, mapped_type() }).first;
        }

        return *i;
    }

    const mapped_type& operator[](key_type&& k)                   const noexcept
    {
        const_iterator i = search(std::forward<key_type>(k));

        if(i == end())
        {
            return not_found();
        }

        return *i;
    }

    iterator begin()                                                    noexcept
    {
        return static_cast<tree_t*>(&m_tree)->begin();
    }

    iterator end()                                                      noexcept
    {
        return static_cast<tree_t*>(&m_tree)->end();
    }

    reverse_iterator rbegin()                                           noexcept
    {
        return static_cast<tree_t*>(&m_tree)->rbegin();
    }

    reverse_iterator rend()                                             noexcept
    {
        return static_cast<tree_t*>(&m_tree)->rend();
    }

    const_iterator begin()                                        const noexcept
    {
        return static_cast<const tree_t*>(&m_tree)->begin();
    }

    const_iterator cbegin()                                       const noexcept
    {
        return begin();
    }

    const_iterator end()                                          const noexcept
    {
        return static_cast<const tree_t*>(&m_tree)->end();
    }

    const_iterator cend()                                         const noexcept
    {
        return end();
    }

    const_reverse_iterator rbegin()                               const noexcept
    {
        return static_cast<const tree_t*>(&m_tree)->rbegin();
    }

    const_reverse_iterator crbegin()                              const noexcept
    {
        return rbegin();
    }

    const_reverse_iterator rend()                                 const noexcept
    {
        return static_cast<const tree_t*>(&m_tree)->rend();
    }

    const_reverse_iterator crend()                                const noexcept
    {
        return rend();
    }

private:
    void init()                                                         noexcept
    {
        m_nodes_used_flags.reset();
    }

    mapped_type& not_found()                                            noexcept
    {
        m_not_found = mapped_type();
        return m_not_found;
    }

    const mapped_type& not_found()                                const noexcept
    {
        return m_not_found;
    }

    tree_node_pointer_t get_free_node()                                 noexcept
    {
        for(std::size_t i = 0; i < N; ++i)
        {
            if(!m_nodes_used_flags[i])
            {
                return &m_nodes_pool[i];
            }
        }

        return nullptr;
    }

    void mark_as_free(tree_node_t& n)                                   noexcept
    {
        for(std::size_t i = 0; i < N; ++i)
        {
            if(n == m_nodes_pool[i])
            {
                m_nodes_used_flags[i] = false;
            }
        }
    }

    void mark_as_used(tree_node_t& n)                                   noexcept
    {
        for(std::size_t i = 0; i < N; ++i)
        {
            if(n == m_nodes_pool[i])
            {
                m_nodes_used_flags[i] = true;
            }
        }
    }

    iterator search(key_type&& k)                                       noexcept
    {
        return static_cast<tree_t*>(&m_tree)->find(
            std::forward<key_type>(k)
        );
    }

    iterator search(const key_type& k)                                  noexcept
    {
        return static_cast<tree_t*>(&m_tree)->find(k);
    }

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

    tree_t                             m_tree             {};
    std::array<tree_node_t, N>         m_nodes_pool       {};

    std::bitset<N>                     m_nodes_used_flags {};

    mapped_type                        m_not_found        {};
};

template
<
      typename K
    , typename V
    , typename... Args
    , typename Compare = std::less<K>
>
constexpr inline const map
                       <
                             K
                           , V
                           , sizeof...(Args)
                           , Compare
                       > create_map(Args&&... args)                     noexcept
{
    static_assert(std::is_nothrow_default_constructible<V>::value,
        "Value type should be nothrow default constructible.");

    return map
           <
                 K
               , V
               , sizeof...(Args)
               , Compare
           >(std::forward<Args>(args)...);
}

} // namespace ecl

#endif // ECL_map
