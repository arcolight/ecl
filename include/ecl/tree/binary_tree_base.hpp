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

template
<
      typename K
    , typename V
    , template <typename> class Compare
    , template <typename, typename, template <typename> class> class N
>
struct node_base
{
    using node_t        = N<K, V, Compare>;

    using pointer       = typename std::add_pointer<node_t>::type;
    using const_pointer = typename std::add_pointer
                                   <
                                       typename std::add_const<node_t>::type
                                   >::type;

    // using key_type      = typename std::add_const<K>::type;
    using key_type      = K;
    using value_type    = V;

    using key_compare   = Compare<key_type>;

    using erase_return  = std::pair<pointer, pointer>;

    node_base()
    {
        static_assert(std::is_default_constructible<K>::value,
                      "Key should be default constructible!");
        static_assert(std::is_default_constructible<V>::value,
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
        : key ( std::forward<key_type>   (k) )
        , val ( std::forward<value_type> (v) )
    {}

    constexpr node_base(const std::pair<key_type, value_type>& p)       noexcept
        : key ( p.first  )
        , val ( p.second )
    {}

    constexpr node_base(std::pair<key_type, value_type>&& p)            noexcept
        : key ( std::forward<key_type>   (p.first ) )
        , val ( std::forward<value_type> (p.second) )
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
        std::swap(val,    other.val   );
        std::swap(key,    other.key   );
    }

    inline pointer most_left()                                          noexcept
    {
        pointer result = static_cast<pointer>(this);
        while(result->have_left())
        {
            result = result->left;
        }
        return result;
    }

    inline const_pointer most_left()                              const noexcept
    {
        const_pointer result = static_cast<const_pointer>(this);
        while(result->have_left())
        {
            result = result->left;
        }
        return result;
    }

    inline pointer most_right()                                         noexcept
    {
        pointer result = static_cast<pointer>(this);
        while(result->have_right())
        {
            result = result->right;
        }
        return result;
    }

    inline const_pointer most_right()                             const noexcept
    {
        const_pointer result = static_cast<const_pointer>(this);
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
        const_pointer t = static_cast<const_pointer>(this);

        if(nullptr == t->parent)
        {
            return false;
        }

        return t == t->parent->left;
    }

    inline bool is_right()                                        const noexcept
    {
        const_pointer t = static_cast<const_pointer>(this);

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

    inline pointer insert(pointer n)                                    noexcept
    {
        if(key_compare()(n->key, key))
        {
            if( ! have_left())
            {
                left = n;
                n->parent = static_cast<pointer>(this);
                return left;
            }
            return left->insert(n);
        }
        else if(key_compare()(key, n->key))
        {
            if( ! have_right())
            {
                right = n;
                n->parent = static_cast<pointer>(this);
                return right;
            }
            return right->insert(n);
        }

        val = n->val;

        return static_cast<pointer>(this);
    }

    inline void link(pointer p)                                         noexcept
    {
        if(key_compare()(p->key, key))
        {
            left = p;
        }
        else if(key_compare()(key, p->key))
        {
            right = p;
        }
        p->parent = static_cast<pointer>(this);
    }

    inline void replace_from(pointer successor)                         noexcept
    {
        key = successor->key;
        val = successor->val;
    }

    inline pointer successor()                                          noexcept
    {
        pointer suc = static_cast<pointer>(this);

        if(have_right())
        {
            suc = right->most_left();
        }

        return suc;
    }

    inline erase_return erase_internal()                                noexcept
    {
        pointer this_p = static_cast<pointer>(this);

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
        replace_from(s);
        return s->erase_internal();
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

        return static_cast<pointer>(this);
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

        return static_cast<const_pointer>(this);
    }

    pointer    left   {};
    pointer    right  {};
    pointer    parent {};

    key_type   key    {};
    value_type val    {};
};

template
<
      typename K
    , typename V
    , template <typename> class Compare
    , template <typename, typename, template <typename> class> class N
>
class binary_tree_base
{
public:
    using node_t        = typename N<K, V, Compare>::node_t;

    using pointer       = typename node_t::pointer;
    using const_pointer = typename node_t::const_pointer;

    using key_type      = typename node_t::key_type;
    using value_type    = typename node_t::value_type;

    using key_compare   = typename node_t::key_compare;

    using erase_return  = typename node_t::erase_return;

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
        using value_type = V;
        using reference  = V&;
        using pointer    = V*;

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
            return m_n->val;
        }

        bool operator==(const self_type& rhs)                              const
        {
            return (m_n == rhs.m_n) && (m_e == rhs.m_e);
        }

        bool operator!=(const self_type& rhs)                              const
        {
            return !operator==(rhs);
        }

        bool operator==(const binary_tree_base::pointer& rhs)              const
        {
            return m_n == rhs;
        }

        bool operator!=(const binary_tree_base::pointer& rhs)              const
        {
            return !operator==(rhs);
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
        using value_type = const V;
        using reference  = const V&;
        using pointer    = const V*;

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
            return m_n->val;
        }

        bool operator==(const self_type& rhs)                              const
        {
            return (m_n == rhs.m_n) && (m_e == rhs.m_e);
        }

        bool operator!=(const self_type& rhs)                              const
        {
            return !operator==(rhs);
        }

        bool operator==(const binary_tree_base::const_pointer& rhs)        const
        {
            return m_n == rhs;
        }

        bool operator!=(const binary_tree_base::const_pointer& rhs)        const
        {
            return !operator==(rhs);
        }
    };

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

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

// Iterators end

    iterator insert(pointer n)
    {
        n->parent            = nullptr;
        n->left              = nullptr;
        n->right             = nullptr;

        if( ! m_header.have_parent())
        {
            m_header.parent = n;
            m_header.left   = n;
            m_header.right  = n;

            ++m_size;

            return iterator(m_header.parent, pointer(&m_header));
        }

        pointer inserted_n = m_header.parent->insert(n);
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

        return iterator(inserted_n, pointer(&m_header));
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
        pointer p = m_header.parent->find(k);
        if(nullptr == p)
        {
            return end();
        }
        return iterator(p, pointer(&m_header));
    }

    const_iterator find(const key_type& k)                        const noexcept
    {
        const_pointer p = m_header.parent->find(k);
        if(nullptr == p)
        {
            return end();
        }
        return const_iterator(p, const_pointer(&m_header));
    }

    pointer erase(const key_type& k)                                    noexcept
    {
        erase_return ret = erase_internal(k);

        return ret.first;
    }

protected:

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
