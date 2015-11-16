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

    inline bool is_left_of(pointer x)                             const noexcept
    {
        return this == x->parent->left;
    }

    inline bool is_right_of(pointer x)                            const noexcept
    {
        return this == x->parent->right;
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

    pointer insert(pointer n)
    {
        return insert_from_root(n);
    }

    std::size_t count()                                           const noexcept
    {
        return m_size;
    }

protected:
    pointer insert_from_root(pointer n)
    {
        if(nullptr == m_root)
        {
            m_root = n;
            m_root->parent = pointer(&m_header);
            ++m_size;
            return m_root;
        }

        return insert_from_node(n, m_root);
    }

    pointer insert_from_node(pointer n, pointer node_to)
    {
        if(key_compare()(n->key, node_to->key))
        {
            if(nullptr == node_to->left)
            {
                node_to->left = n;
                n->parent = node_to;
                ++m_size;
                return n;
            }

            return insert_from_node(n, node_to->left);
        }
        else if(key_compare()(node_to->key, n->key))
        {
            if(nullptr == node_to->right)
            {
                node_to->right = n;
                n->parent = node_to;
                ++m_size;
                return n;
            }

            return insert_from_node(n, node_to->right);
        }
        else // equality
        {
            node_to->val = n->val;
            return node_to;
        }
    }

    pointer& find(const key_type& k)                              const noexcept
    {
        pointer previous = nullptr;
        pointer current  = m_root;

        if(nullptr == m_root)
        {
            return m_root;
        }

        while(nullptr != current)
        {
            if(key_compare()(current->key, k))
            {
                previous = current;
                current  = current->left;
            }
            else if(key_compare()(k, current->key))
            {
                previous = current;
                current  = current->right;
            }
            else
            {
                return std::make_pair(previous, current);
            }
        }

        return std::make_pair(previous, nullptr);
    }

    void link_as_left(pointer p, pointer l)                       const noexcept
    {
        if(nullptr != p && nullptr != l)
        {
            p->left   = l;
            l->parent = p;
        }
    }

    void link_as_right(pointer p, pointer r)                      const noexcept
    {
        if(nullptr != p && nullptr != r)
        {
            p->right  = r;
            r->parent = p;
        }
    }

    void rotate_left(pointer x)                                         noexcept
    {
        pointer y = x->right;

        link_as_right(x, y->left);

        y->parent = x->parent;

        if ( x->parent == nullptr )
        {
            m_root = y;
        }
        else if ( x == x->parent->left )
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }

        link_as_left(y, x);
    }

    void rotate_right(pointer x)                                        noexcept
    {
        pointer y = x->left;

        link_as_left(x, y->right);

        y->parent = x->parent;

        if ( x->parent == nullptr )
        {
            m_root = y;
        }
        else if ( x == x->parent->left )
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }

        link_as_right(y, x);
    }

    struct base_iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = ptrdiff_t;

        base_iterator(pointer n)                                        noexcept
            : m_n(n)
        {}

        void increment()                                                noexcept
        {
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
                }

                if(m_n->right != y)
                {
                    m_n = y;
                }
            }
        }

        void decrement()                                                noexcept
        {
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
    };

public:
    struct iterator : public base_iterator
    {
    private:
        using base_iterator::increment;
        using base_iterator::decrement;
        using base_iterator::m_n;

    public:
        using self_type  = iterator;
        using value_type = V;
        using reference  = V&;
        using pointer    = V*;

        iterator(binary_tree::pointer n) : base_iterator(n)
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
            return m_n == rhs.m_n;
        }

        bool operator!=(const self_type& rhs)                              const
        {
            return m_n != rhs.m_n;
        }
    };

    struct const_iterator : public base_iterator
    {
    private:
        using base_iterator::increment;
        using base_iterator::decrement;
        using base_iterator::m_n;

    public:
        using self_type  = const_iterator;
        using value_type = const V;
        using reference  = const V&;
        using pointer    = const V*;

        const_iterator(binary_tree::pointer n) : base_iterator(n)
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
            return m_n == rhs.m_n;
        }

        bool operator!=(const self_type& rhs)                              const
        {
            return m_n != rhs.m_n;
        }
    };

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin()                                                    noexcept
    {
        return iterator(m_header.left);
    }

    iterator end()                                                      noexcept
    {
        return iterator(m_header.right);
    }

    const_iterator begin()                                        const noexcept
    {
        return const_iterator(m_header.left);
    }

    const_iterator end()                                          const noexcept
    {
        return const_iterator(m_header.right);
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

protected:
    node_t      m_header;
    pointer     m_root    {};
    std::size_t m_size    { 0 };
};

} // namespace tree

} // namespace ecl

#endif //ECL_BINARY_TREE
