#ifndef ECL_BINARY_TREE
#define ECL_BINARY_TREE

#include <cstddef>

#include <utility>
#include <functional>
#include <type_traits>
#include <iterator>

#ifdef ECL_ENABLE_TREE_SHARED_PTR
#include <memory>
#endif // ECL_ENABLE_TREE_SHARED_PTR

namespace ecl
{

namespace tree
{

enum class pointer_type
{
      RAW
#ifdef ECL_ENABLE_TREE_SHARED_PTR
    , SHARED
#endif // ECL_ENABLE_TREE_SHARED_PTR
};

template<pointer_type P, typename T>
struct pointer_wrap {};

template<typename T>
struct pointer_wrap<pointer_type::RAW, T>
{
    using type = typename std::add_pointer<T>::type;
};

#ifdef ECL_ENABLE_TREE_SHARED_PTR
template<typename T>
struct pointer_wrap<pointer_type::SHARED, T>
{
    using type = std::shared_ptr<T>;
};
#endif // ECL_ENABLE_TREE_SHARED_PTR

template
<
    typename K,
    typename V,
    pointer_type PT,
    template<typename, typename, pointer_type> class N
>
struct node_base
{
    using pointer     = typename pointer_wrap<PT, N<K, V, PT>>::type;

    using key_type    = typename std::add_const<K>::type;
    using value_type  = V;

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

    pointer min()                                                 const noexcept
    {
        pointer result = this;
        while(nullptr != result->left)
        {
            result = result->left;
        }
        return result;
    }

    pointer max()                                                 const noexcept
    {
        pointer result = this;
        while(nullptr != result->right)
        {
            result = result->right;
        }
        return result;
    }

    inline pointer grandparent()
    {
        if(nullptr == parent)
        {
            return nullptr;
        }

        return parent->parent;
    }

    inline pointer uncle()
    {
        pointer g = grandparent();
        if(nullptr == g)
        {
            return nullptr;
        }

        if(parent == g->left)
        {
            return g->right;
        }

        return g->left;
    }

    pointer    left   {};
    pointer    right  {};
    pointer    parent {};

    key_type   key    {};
    value_type val    {};
};

template
<
    typename K,
    typename V,
    pointer_type PT = pointer_type::RAW
>
struct node : public node_base<K, V, PT, ecl::tree::node>
{
    // Full namespace is workaround for clang bug
    // about template-template parameters
    //
    // http://stackoverflow.com/questions/17687459/clang-not-accepting-use-of-template-template-parameter-when-using-crtp
    using base = node_base<K, V, PT, ecl::tree::node>;

    using node_base<K, V, PT, ecl::tree::node>::node_base;
    using typename base::pointer;
};

template
<
    typename K,
    typename V,
    pointer_type PT                                     = pointer_type::RAW,
    typename Compare                                    = std::less<const K>,
    template <typename, typename, pointer_type> class N = node
>
class binary_tree
{
public:
    using node_t      = N<K, V, PT>;
    using pointer     = typename node_t::pointer;

    using key_type    = typename node_t::key_type;
    using value_type  = typename node_t::value_type;
    using key_compare = Compare;

// Iterators start
protected:
    struct base_iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = ptrdiff_t;

        base_iterator(pointer n, pointer e)                             noexcept
            : m_n(n)
            , m_e(e)
        {}

        void increment()                                                noexcept
        {
            if(m_n == m_e)
            {
                return;
            }

            if(nullptr != m_n->right)
            {
                m_n = m_n->right;
                while(nullptr != m_n->left)
                {
                    m_n = m_n->left;
                }
            }
            else
            {
                pointer y = m_n->parent;
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

            if(nullptr != m_n->left)
            {
                m_n = m_n->left;
                while(nullptr != m_n->right)
                {
                    m_n = m_n->right;
                }
            }
            else
            {
                pointer y = m_n->parent;
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

        pointer m_n { nullptr };
        pointer m_e { nullptr };
    };

public:
    struct iterator : public base_iterator
    {
    private:
        using base_iterator::increment;
        using base_iterator::decrement;
        using base_iterator::m_n;
        using base_iterator::m_e;

    public:
        using self_type  = iterator;
        using value_type = V;
        using reference  = V&;
        using pointer    = V*;

        iterator(binary_tree::pointer n, binary_tree::pointer e)
            : base_iterator(n, e)
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

        bool operator==(const binary_tree::pointer& rhs)                   const
        {
            return m_n == rhs;
        }

        bool operator!=(const binary_tree::pointer& rhs)                   const
        {
            return !operator==(rhs);
        }
    };

    struct const_iterator : public base_iterator
    {
    private:
        using base_iterator::increment;
        using base_iterator::decrement;
        using base_iterator::m_n;
        using base_iterator::m_e;

    public:
        using self_type  = const_iterator;
        using value_type = const V;
        using reference  = const V&;
        using pointer    = const V*;

        const_iterator(binary_tree::pointer n, binary_tree::pointer e)
            : base_iterator(n, e)
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

        bool operator==(const binary_tree::pointer& rhs)                   const
        {
            return m_n == rhs;
        }

        bool operator!=(const binary_tree::pointer& rhs)                   const
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

// Iterators end

    iterator insert(pointer n)
    {
        return insert_from_root(n, true, true);
    }

    std::size_t count()                                           const noexcept
    {
        return m_size;
    }

    pointer root()                                                      noexcept
    {
        return m_root;
    }

    iterator find(const key_type& k)                              const noexcept
    {
        pointer current = m_root;

        while(nullptr != current)
        {
            if(key_compare()(current->key, k))
            {
                current  = current->right;
            }
            else if(key_compare()(k, current->key))
            {
                current  = current->left;
            }
            else
            {
                return iterator(current, pointer(&m_header));
            }
        }

        return iterator(current, pointer(&m_header));
    }

protected:
    iterator insert_from_root(pointer n, bool most_left, bool most_right)
    {
        if(nullptr == m_root)
        {
            m_root          = n;
            m_root->parent  = nullptr;
            m_header.left   = m_root;
            m_header.right  = m_root;
            m_header.parent = m_root;

            ++m_size;

            return iterator(m_root, pointer(&m_header));
        }

        return insert_from_node(n, m_root, most_left, most_right);
    }

    iterator insert_from_node(pointer n,
                              pointer node_to,
                              bool    most_left,
                              bool    most_right)
    {
        if(key_compare()(n->key, node_to->key))
        {
            if(nullptr == node_to->left)
            {
                node_to->left = n;
                n->parent = node_to;

                if(most_left)
                {
                    m_header.left = n;
                }

                ++m_size;

                return iterator(n, pointer(&m_header));
            }

            return insert_from_node(n, node_to->left, most_left & true, false);
        }
        else if(key_compare()(node_to->key, n->key))
        {
            if(nullptr == node_to->right)
            {
                node_to->right = n;
                n->parent = node_to;

                if(most_right)
                {
                    m_header.right = n;
                }

                ++m_size;

                return iterator(n, pointer(&m_header));
            }

            return insert_from_node(n, node_to->right, false, most_right & true);
        }
        else // equality
        {
            node_to->val = n->val;
            return iterator(node_to, pointer(&m_header));
        }
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

    pointer rotate_left(pointer n)                                      noexcept
    {
        pointer p = n->parent;
        pointer r = n->right;
        pointer rl = (nullptr == r) ? nullptr : r->left;
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

        return n;
    }

    pointer rotate_right(pointer n)                                     noexcept
    {
        pointer p = n->parent;
        pointer l = n->left;
        pointer lr = (nullptr == l) ? nullptr : l->right;
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

        return n;
    }

protected:
    node_t      m_header;
    pointer     m_root    {};
    std::size_t m_size    { 0 };
};

} // namespace tree

} // namespace ecl

#endif //ECL_BINARY_TREE
