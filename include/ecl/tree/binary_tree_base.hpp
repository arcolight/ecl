#ifndef ECL_BINARY_TREE_BASE
#define ECL_BINARY_TREE_BASE

#include <cstddef>

#include <utility>
#include <functional>
#include <type_traits>
#include <iterator>

namespace ecl
{

namespace tree
{

namespace detail
{
    template<typename T>
    struct conditional_storage
    {
        T x {};
    };

    template<>
    struct conditional_storage<void>
    {

    };
} // namespace detail

template
<
      typename K
    , typename V
    , template <typename> class Compare
    , template <typename, typename, template <typename> class, typename> class N
    , typename Storage = void
>
struct node_base
{
    using node_t        = N<K, V, Compare, Storage>;

    using pointer       = typename std::add_pointer<node_t>::type;
    using const_pointer = typename std::add_pointer
                                   <
                                       typename std::add_const<node_t>::type
                                   >::type;

    // using key_type      = typename std::add_const<K>::type;
    using key_type      = K;
    using value_type    = V;
    using pair_type     = std::pair<key_type, value_type>;
    using pair_type_ext = std::pair<const key_type&, value_type&>;

    using key_compare   = Compare<key_type>;

    using erase_return  = std::pair<pointer, pointer>;

    constexpr node_base()                                               noexcept
    {
        static_assert(std::is_default_constructible<key_type>::value,
                      "Key should be default constructible!");
        static_assert(std::is_default_constructible<value_type>::value,
                      "Value should be default constructible!");
    }

    constexpr node_base(const key_type& k)                              noexcept
        : key ( k )
    {}

    constexpr node_base(key_type&& k)                                   noexcept
        : key ( std::forward<key_type>(k) )
    {}

    constexpr node_base(const key_type& k, const value_type& v)         noexcept
        : key ( k )
        , val ( v )
    {}

    constexpr node_base(key_type&& k, value_type&& v)                   noexcept
        : key ( std::forward<key_type>   ( k ) )
        , val ( std::forward<value_type> ( v ) )
    {}

    constexpr node_base(const pair_type& p)                             noexcept
        : key ( p.first  )
        , val ( p.second )
    {}

    constexpr node_base(pair_type&& p)                                  noexcept
        : key ( std::forward<key_type>   ( p.first  ) )
        , val ( std::forward<value_type> ( p.second ) )
    {}

    constexpr node_base(const node_base& other)                         noexcept
        : left   (other.left)
        , right  (other.right)
        , parent (other.parent)
        , key    (other.key)
        , val    (other.val)
    {}

    node_base(node_base&& other)
    {
        std::swap(left,   other.left  );
        std::swap(right,  other.right );
        std::swap(parent, other.parent);
        std::swap(key,    other.key   );
        std::swap(val,    other.val   );
    }

    node_base& operator=(const node_base& other)                        noexcept
    {
        left   = other.left;
        right  = other.right;
        parent = other.parent;
        key    = other.key;
        val    = other.val;

        return *this;
    }

    node_base& operator=(const node_base&& other)
    {
        std::swap(left,   other.left  );
        std::swap(right,  other.right );
        std::swap(parent, other.parent);
        std::swap(key,    other.key   );
        std::swap(val,    other.val   );

        return *this;
    }

    bool operator==(const node_base& rhs)                                  const
    {
        return ((key == rhs.key) && (val == rhs.val));
    }

    bool operator!=(const node_base& rhs)                                  const
    {
        return !(*this == rhs);
    }

    inline pointer most_left()                                          noexcept
    {
        pointer result = pointer_to_this();
        while(result->have_left())
        {
            result = result->left;
        }
        return result;
    }

    inline const_pointer most_left()                              const noexcept
    {
        const_pointer result = const_pointer_to_this();
        while(result->have_left())
        {
            result = result->left;
        }
        return result;
    }

    inline pointer most_right()                                         noexcept
    {
        pointer result = pointer_to_this();
        while(result->have_right())
        {
            result = result->right;
        }
        return result;
    }

    inline const_pointer most_right()                             const noexcept
    {
        const_pointer result = const_pointer_to_this();
        while(result->have_right())
        {
            result = result->right;
        }
        return result;
    }

    inline pointer sibling()                                            noexcept
    {
        if( ! have_parent())
        {
            return nullptr;
        }

        if(is_left())
        {
            return parent->right;
        }

        return parent->left;
    }

    inline const_pointer sibling()                                const noexcept
    {
        if( ! have_parent())
        {
            return nullptr;
        }

        if(is_left())
        {
            return parent->right;
        }

        return parent->left;
    }

    inline pointer grandparent()                                        noexcept
    {
        if( ! have_parent())
        {
            return nullptr;
        }

        return parent->parent;
    }

    inline const_pointer grandparent()                            const noexcept
    {
        if( ! have_parent())
        {
            return nullptr;
        }

        return parent->parent;
    }

    inline pointer uncle()                                              noexcept
    {
        pointer g = grandparent();
        if(nullptr == g)
        {
            return nullptr;
        }

        if(parent->is_left())
        {
            return g->right;
        }

        return g->left;
    }

    inline const_pointer uncle()                                  const noexcept
    {
        const_pointer g = grandparent();
        if(nullptr == g)
        {
            return nullptr;
        }

        if(parent->is_left())
        {
            return g->right;
        }

        return g->left;
    }

    inline bool is_left()                                         const noexcept
    {
        const_pointer t = const_pointer_to_this();

        if(nullptr == t->parent)
        {
            return false;
        }

        return t == t->parent->left;
    }

    inline bool is_right()                                        const noexcept
    {
        const_pointer t = const_pointer_to_this();

        if(nullptr == t->parent)
        {
            return false;
        }

        return t == t->parent->right;
    }

    inline bool have_left()                                       const noexcept
    {
        return nullptr != left;
    }

    inline bool have_right()                                      const noexcept
    {
        return nullptr != right;
    }

    inline bool have_both()                                       const noexcept
    {
        return have_left() && have_right();
    }

    inline bool have_no_child()                                   const noexcept
    {
        return !have_left() && !have_right();
    }

    inline bool have_only_one_child()                             const noexcept
    {
        return ( have_left() && !have_right()) |
               (!have_left() &&  have_right());
    }

    inline bool is_leaf()                                         const noexcept
    {
        return have_no_child();
    }

    inline bool have_parent()                                     const noexcept
    {
        return nullptr != parent;
    }

    inline bool alone()                                           const noexcept
    {
        return !have_parent() && have_no_child();
    }

    inline pointer pointer_to_this()                                    noexcept
    {
        return static_cast<pointer>(this);
    }

    inline const_pointer const_pointer_to_this()                  const noexcept
    {
        return static_cast<const_pointer>(this);
    }

    inline pointer insert(pointer n, bool allow_update = true)          noexcept
    {
        if(key_compare()(n->key, key))
        {
            if( ! have_left())
            {
                left = n;
                n->parent = pointer_to_this();
                return left;
            }
            return left->insert(n);
        }
        else if(key_compare()(key, n->key))
        {
            if( ! have_right())
            {
                right = n;
                n->parent = pointer_to_this();
                return right;
            }
            return right->insert(n);
        }

        if(allow_update)
        {
            val = n->val;
        }

        return pointer_to_this();
    }

    inline void link(pointer p)                                         noexcept
    {
        if(nullptr == p)
        {
            return;
        }

        if(key_compare()(p->key, key))
        {
            left = p;
        }
        else if(key_compare()(key, p->key))
        {
            right = p;
        }
        p->parent = pointer_to_this();
    }

    inline pointer& link_from_parent()                            const noexcept
    {
        return is_left() ? parent->left : parent->right;
    }

    enum class adjacency_type
    {
          none
        , left_child
        , right_child
        , parent
    };

    inline adjacency_type adjacency(pointer p)                    const noexcept
    {
        if(left == p)
        {
            return adjacency_type::left_child;
        }

        if(right == p)
        {
            return adjacency_type::right_child;
        }

        if(p->left  == const_pointer_to_this() ||
           p->right == const_pointer_to_this())
        {
            return adjacency_type::parent;
        }

        return adjacency_type::none;
    }

    inline void replace_from(pointer suc)                               noexcept
    {
        switch(adjacency(suc))
        {
            case adjacency_type::left_child:
                if(have_parent())
                {
                    link_from_parent() = suc;
                }

                suc->parent = parent;
                parent      = suc;
                left        = suc->left;
                suc->left   = pointer_to_this();

                if(have_right() && suc->have_right())
                {
                    right->parent      = suc;
                    suc->right->parent = pointer_to_this();
                    std::swap(right, suc->right);
                }
                else if(have_right())
                {
                    suc->right    = right;
                    right->parent = suc;
                }
                else if(suc->have_right())
                {
                    right              = suc->right;
                    suc->right->parent = pointer_to_this();
                }
            break;
            case adjacency_type::right_child:
                if(have_parent())
                {
                    link_from_parent() = suc;
                }

                suc->parent = parent;
                parent      = suc;
                right       = suc->right;
                suc->right  = pointer_to_this();

                if(have_left() && suc->have_left())
                {
                    left->parent      = suc;
                    suc->left->parent = pointer_to_this();
                    std::swap(left, suc->left);
                }
                else if(have_left())
                {
                    suc->left    = left;
                    left->parent = suc;
                }
                else if(suc->have_left())
                {
                    left              = suc->left;
                    suc->left->parent = pointer_to_this();
                }
            break;
            case adjacency_type::parent:
                suc->replace_from(pointer_to_this());
            break;
            case adjacency_type::none:
                if(have_parent() && suc->have_parent())
                {
                    std::swap(link_from_parent(), suc->link_from_parent());
                }
                else if(have_parent())
                {
                    link_from_parent() = suc;
                }
                else if(suc->have_parent())
                {
                    suc->link_from_parent() = pointer_to_this();
                }

                if(have_left() && suc->have_left())
                {
                    std::swap(left->parent, suc->left->parent);
                }
                else if(have_left())
                {
                    left->parent = suc;
                }
                else if(suc->have_left())
                {
                    suc->left->parent = pointer_to_this();
                }

                if(have_right() && suc->have_right())
                {
                    std::swap(right->parent, suc->right->parent);
                }
                else if(have_right())
                {
                    right->parent = suc;
                }
                else if(suc->have_right())
                {
                    suc->right->parent = pointer_to_this();
                }
            break;
        }

//        pointer parent_suc = suc->parent;
//        pointer left_suc   = suc->left;
//        pointer right_suc  = suc->right;

//        if(have_parent()) { parent->link(suc); }
//        suc->link(left);
//        suc->link(right);
//        if(nullptr == left)  { suc->left  = nullptr; }
//        if(nullptr == right) { suc->right = nullptr; }

//        if(nullptr != parent_suc) { parent_suc->link(pointer_to_this()); }
//        link(left_suc);
//        link(right_suc);
//        if(nullptr == left_suc)  { left  = nullptr; }
//        if(nullptr == right_suc) { right = nullptr; }
    }

    inline pointer successor()                                          noexcept
    {
        pointer suc = pointer_to_this();

        if(have_right())
        {
            suc = right->most_left();
        }

        return suc;
    }

    inline void detach()                                                noexcept
    {
        parent = nullptr;
        left   = nullptr;
        right  = nullptr;
    }

    inline void init()                                                  noexcept
    {
        detach();

        val = value_type();
        key = key_type();

        _s = detail::conditional_storage<Storage>();
    }

    inline erase_return erase_internal()                                noexcept
    {
        pointer this_p = pointer_to_this();

        if(have_no_child())
        {
            if(have_parent())
            {
                if(is_left())
                {
                    parent->left = nullptr;
                }
                else
                {
                    parent->right = nullptr;
                }

                return { this_p, nullptr };
            }

            return { this_p, nullptr };
        }

        if(have_parent())
        {
            if(have_left())
            {
                parent->link(left);
                return { this_p, left };
            }
            else
            {
                parent->link(right);
                return { this_p, right };
            }
        }

        if(have_left())
        {
            left->parent = nullptr;
            return { this_p, left };
        }
        else
        {
            right->parent = nullptr;
            return { this_p, right };
        }
    }

    inline erase_return erase()                                         noexcept
    {
        pointer s = successor();
        if(pointer_to_this() != s)
        {
            replace_from(s);
        }
        return erase_internal();
    }

    inline pointer find(const key_type& k)                              noexcept
    {
        if(key_compare()(k, key))
        {
            if( ! have_left())
            {
                return nullptr;
            }
            return left->find(k);
        }
        else if(key_compare()(key, k))
        {
            if( ! have_right())
            {
                return nullptr;
            }
            return right->find(k);
        }

        return pointer_to_this();
    }

    inline const_pointer find(const key_type& k)                  const noexcept
    {
        if(key_compare()(k, key))
        {
            if( ! have_left())
            {
                return nullptr;
            }
            return left->find(k);
        }
        else if(key_compare()(key, k))
        {
            if( ! have_right())
            {
                return nullptr;
            }
            return right->find(k);
        }

        return const_pointer_to_this();
    }

    pointer                              left   { nullptr };
    pointer                              right  { nullptr };
    pointer                              parent { nullptr };

    key_type                             key    { };
    value_type                           val    { };
    pair_type_ext                        pair   { key, val };

    detail::conditional_storage<Storage> _s     {};
};

template
<
      typename K
    , typename V
    , template <typename> class Compare
    , template <typename, typename, template <typename> class, typename> class N
    , typename Storage = void
>
class binary_tree_base
{
public:
    using node_t        = typename N<K, V, Compare, Storage>::node_t;

    using pointer       = typename node_t::pointer;
    using const_pointer = typename node_t::const_pointer;

    using key_type      = typename node_t::key_type;
    using value_type    = typename node_t::value_type;

    using key_compare   = typename node_t::key_compare;

    using erase_return  = typename node_t::erase_return;

    binary_tree_base()
    {
        m_header.parent = &m_header;
        m_header.left   = &m_header;
        m_header.right  = &m_header;
    }

// Iterators start
protected:
    template <typename Pointer>
    struct base_iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = ptrdiff_t;

        base_iterator(Pointer n, Pointer e)                             noexcept
            : m_n(n)
            , m_e(e)
        {}

        void increment()                                                noexcept
        {
            if(m_n == m_e)
            {
                return;
            }

            if(m_n->have_right())
            {
                m_n = m_n->right;
                m_n = m_n->most_left();
            }
            else
            {
                Pointer y = m_n->parent;
                if(nullptr == y)
                {
                    m_n = m_e;
                    return;
                }
                while(m_n == y->right)
                {
                    m_n = y;
                    y = y->parent;
                    if(nullptr == y)
                    {
                        m_n = m_e;
                        return;
                    }
                }

                if(m_n->right != y)
                {
                    m_n = y;
                }
            }
        }

        void decrement()                                                noexcept
        {
            if(m_n == m_e)
            {
                m_n = m_e->right;
                return;
            }

            if(m_n->have_left())
            {
                m_n = m_n->left;
                m_n = m_n->most_right();
            }
            else
            {
                Pointer y = m_n->parent;
                while(m_n == y->left)
                {
                    m_n = y;
                    y = y->parent;
                }

                if(m_n->left != y)
                {
                    m_n = y;
                }
            }
        }

        Pointer m_n { nullptr };
        Pointer m_e { nullptr };
    };

public:
    struct iterator;
    struct const_iterator;

    struct iterator :
        public base_iterator<binary_tree_base::pointer>
    {
    private:
        using base = base_iterator<binary_tree_base::pointer>;
        using base::increment;
        using base::decrement;
        using base::m_n;
        using base::m_e;

    public:
        using self_type  = iterator;
        using value_type = typename node_t::pair_type_ext;
        using reference  = typename std::add_lvalue_reference<value_type>::type;
        using pointer    = typename std::add_pointer<value_type>::type;

        iterator(binary_tree_base::pointer n,
                 binary_tree_base::pointer e)
            : base_iterator<binary_tree_base::pointer>(n, e)
        {}

        self_type& operator++()                                         noexcept
        {
            increment();
            return *this;
        }

        self_type  operator++(int)                                      noexcept
        {
            self_type tmp = *this;
            increment();
            return tmp;
        }

        self_type& operator--()                                         noexcept
        {
            decrement();
            return *this;
        }

        self_type  operator--(int)                                      noexcept
        {
            self_type tmp = *this;
            decrement();
            return tmp;
        }

        reference  operator*()                                          noexcept
        {
            return m_n->pair;
        }

        pointer    operator->()                                         noexcept
        {
            return &(m_n->pair);
        }

        bool operator==(const self_type& rhs)                     const noexcept
        {
            return (m_n == rhs.m_n) && (m_e == rhs.m_e);
        }

        bool operator!=(const self_type& rhs)                     const noexcept
        {
            return !operator==(rhs);
        }

        bool operator==(const binary_tree_base::pointer& rhs)     const noexcept
        {
            return m_n == rhs;
        }

        bool operator!=(const binary_tree_base::pointer& rhs)     const noexcept
        {
            return !operator==(rhs);
        }

        operator const_iterator()                                 const noexcept
        {
            return const_iterator(m_n, m_e);
        }

        operator binary_tree_base::pointer()                      const noexcept
        {
            return m_n;
        }

        operator binary_tree_base::const_pointer()                const noexcept
        {
            return m_n;
        }
    };

    struct const_iterator :
        public base_iterator<binary_tree_base::const_pointer>
    {
    private:
        using base = base_iterator<binary_tree_base::const_pointer>;
        using base::increment;
        using base::decrement;
        using base::m_n;
        using base::m_e;

    public:
        using self_type  = const_iterator;
        using value_type = const typename node_t::pair_type_ext;
        using reference  = typename std::add_lvalue_reference<value_type>::type;
        using pointer    = typename std::add_pointer<value_type>::type;

        const_iterator(binary_tree_base::const_pointer n,
                       binary_tree_base::const_pointer e)
            : base_iterator<binary_tree_base::const_pointer>(n, e)
        {}

        self_type& operator++()                                         noexcept
        {
            increment();
            return *this;
        }

        self_type  operator++(int)                                      noexcept
        {
            self_type tmp = *this;
            increment();
            return tmp;
        }

        self_type& operator--()                                         noexcept
        {
            decrement();
            return *this;
        }

        self_type  operator--(int)                                      noexcept
        {
            self_type tmp = *this;
            decrement();
            return tmp;
        }

        reference  operator*()                                    const noexcept
        {
            return m_n->pair;
        }

        pointer    operator->()                                   const noexcept
        {
            return &(m_n->pair);
        }

        bool operator==(const self_type& rhs)                     const noexcept
        {
            return (m_n == rhs.m_n) && (m_e == rhs.m_e);
        }

        bool operator!=(const self_type& rhs)                     const noexcept
        {
            return !operator==(rhs);
        }

        bool operator==(binary_tree_base::const_pointer& rhs)     const noexcept
        {
            return m_n == rhs;
        }

        bool operator!=(binary_tree_base::const_pointer& rhs)     const noexcept
        {
            return !operator==(rhs);
        }

        operator iterator()                                       const noexcept
        {
            return iterator(const_cast<binary_tree_base::pointer>(m_n),
                            const_cast<binary_tree_base::pointer>(m_e));
        }

        operator binary_tree_base::pointer()                      const noexcept
        {
            return m_n;
        }

        operator binary_tree_base::const_pointer()                const noexcept
        {
            return m_n;
        }
    };

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using insert_return = std::pair<iterator, pointer>;

    iterator begin()                                                    noexcept
    {
        return iterator(m_header.left, pointer(&m_header));
    }

    iterator end()                                                      noexcept
    {
        return iterator(pointer(&m_header), pointer(&m_header));
    }

    const_iterator begin()                                        const noexcept
    {
        return const_iterator(m_header.left, pointer(&m_header));
    }

    const_iterator end()                                          const noexcept
    {
        return const_iterator(pointer(&m_header), pointer(&m_header));
    }

    const_iterator cbegin()                                       const noexcept
    {
        return const_iterator(m_header.left, pointer(&m_header));
    }

    const_iterator cend()                                         const noexcept
    {
        return const_iterator(pointer(&m_header), pointer(&m_header));
    }

    reverse_iterator rbegin()                                           noexcept
    {
        return reverse_iterator(end());
    }

    reverse_iterator rend()                                             noexcept
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rbegin()                               const noexcept
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator rend()                                 const noexcept
    {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crbegin()                              const noexcept
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crend()                                const noexcept
    {
        return const_reverse_iterator(begin());
    }

    iterator make_iterator(pointer p)                             const noexcept
    {
        if(nullptr == p)
        {
            return end();
        }

        return iterator(p, pointer(&m_header));
    }

    const_iterator make_iterator(const_pointer p)                 const noexcept
    {
        if(nullptr == p)
        {
            return end();
        }

        return const_iterator(p, const_pointer(&m_header));
    }

// Iterators end

    iterator insert(pointer n, bool allow_update = true)
    {
        return insert_internal(n, allow_update).first;
    }

    std::size_t count()                                           const noexcept
    {
        return m_size;
    }

    bool empty()                                                  const noexcept
    {
        return m_size == 0; // m_header.parent == pointer(&m_header); ?
    }

    pointer root()                                                const noexcept
    {
        if(empty())
        {
            return nullptr;
        }

        return m_header.parent;
    }

    pointer min()                                                 const noexcept
    {
        if(empty())
        {
            return nullptr;
        }

        return m_header.left;
    }

    pointer max()                                                 const noexcept
    {
        if(empty())
        {
            return nullptr;
        }

        return m_header.right;
    }

    iterator find(const key_type& k)                                    noexcept
    {
        if(empty())
        {
            return end();
        }

        pointer p = m_header.parent->find(k);
        if(nullptr == p)
        {
            return end();
        }
        return iterator(p, pointer(&m_header));
    }

    const_iterator find(const key_type& k)                        const noexcept
    {
        if(empty())
        {
            return end();
        }

        const_pointer p = m_header.parent->find(k);
        if(nullptr == p)
        {
            return end();
        }
        return const_iterator(p, const_pointer(&m_header));
    }

    erase_return erase(const key_type& k)                               noexcept
    {
        auto p = erase_internal(k);

        if(nullptr != p.first)
        {
            p.first->detach();
        }

        return p;
    }

protected:
    insert_return insert_internal(pointer n, bool allow_update)         noexcept
    {
        n->detach();

        if(m_header.parent == &m_header)
        {
            m_header.parent = n;
            m_header.left   = n;
            m_header.right  = n;

            ++m_size;

            return
            {
                  iterator(m_header.parent, pointer(&m_header))
                , m_header.parent
            };
        }

        pointer inserted_n = m_header.parent->insert(n, allow_update);
        // new node
        if(inserted_n == n)
        {
            if(key_compare()(n->key, m_header.left->key))
            {
                m_header.left = n;
            }
            else if(key_compare()(m_header.right->key, n->key))
            {
                m_header.right = n;
            }

            ++m_size;
        }

        return { iterator(inserted_n, pointer(&m_header)), inserted_n };
    }

    erase_return erase_internal(pointer p)                              noexcept
    {
        erase_return ret  = p->erase();
        pointer removed   = ret.first;
        pointer successor = ret.second;

        if(removed == pointer(m_header.parent) && (nullptr == successor))
        {
            m_size = 0;
            m_header.parent = pointer(&m_header);
            m_header.left   = pointer(&m_header);
            m_header.right  = pointer(&m_header);

            return ret;
        }

        --m_size;

        if(m_header.parent == removed)
        {
            m_header.parent = successor;
        }

        if(m_header.left == removed)
        {
            if(nullptr != successor)
            {
                m_header.left = successor->most_left();
            }
            else
            {
                m_header.left = removed->parent->most_left();
            }
        }

        if(m_header.right == removed)
        {
            if(nullptr != successor)
            {
                m_header.right = successor->most_right();
            }
            else
            {
                m_header.right = removed->parent->most_right();
            }
        }

        return ret;
    }

    erase_return erase_internal(const key_type& k)                      noexcept
    {
        if(empty())
        {
            return { nullptr, nullptr };
        }

        pointer to_erase = m_header.parent->find(k);
        if(nullptr == to_erase)
        {
            return { nullptr, nullptr };
        }

        return erase_internal(to_erase);
    }

    void link_as_left(pointer c, pointer f)                             noexcept
    {
        if((nullptr == c) && (nullptr == f))
        {
            return;
        }

        if(nullptr != f)
        {
            f->left   = c;
        }

        if(nullptr != c)
        {
            c->parent = f;
        }
    }

    void link_as_right(pointer c, pointer f)                            noexcept
    {
        if((nullptr == c) && (nullptr == f))
        {
            return;
        }

        if(nullptr != f)
        {
            f->right  = c;
        }

        if(nullptr != c)
        {
            c->parent = f;
        }
    }

    void rotate_left(pointer n)                                         noexcept
    {
        pointer p = n->parent;
        pointer r = n->right;
        pointer rl = (nullptr == r) ? nullptr : r->left;

        if(m_header.parent == n)
        {
            m_header.parent = n->right;
        }

        if(nullptr != p)
        {
            if(p->left == n)
            {
                link_as_left(r, p);
            }
            if(p->right == n)
            {
                link_as_right(r, p);
            }
        }

        link_as_left(n, r);
        link_as_right(rl, n);

        root()->parent = nullptr;
    }

    void rotate_right(pointer n)                                        noexcept
    {
        pointer p = n->parent;
        pointer l = n->left;
        pointer lr = (nullptr == l) ? nullptr : l->right;

        if(m_header.parent == n)
        {
            m_header.parent = n->left;
        }

        if(nullptr != p)
        {
            if(p->left == n)
            {
                link_as_left(l, p);
            }
            if(p->right == n)
            {
                link_as_right(l, p);
            }
        }

        link_as_right(n, l);
        link_as_left(lr, n);

        root()->parent = nullptr;
    }

protected:
    node_t      m_header {   };
    std::size_t m_size   { 0 };
};

} // namespace tree

} // namespace ecl

#endif //ECL_BINARY_TREE_BASE
