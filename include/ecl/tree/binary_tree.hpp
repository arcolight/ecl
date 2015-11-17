#ifndef ECL_BINARY_TREE
#define ECL_BINARY_TREE

#include <cstddef>

#include <utility>
#include <functional>
#include <type_traits>
#include <iterator>
#include <algorithm>

#include <iomanip>

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

    void print_indent(std::size_t indent)
    {
        for(std::size_t i = 0; i < indent; ++i)
        {
            std::cout << " ";
        }
    }

    void print(const char* s, std::size_t indent)
    {
        print_indent(indent);
        std::cout << s << key << std::endl;
        if(nullptr != left)
        {
            left->print("l. ", indent + 1);
        }
        if(nullptr != right)
        {
            right->print("r. ", indent + 1);
        }
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

// Iterators end

    iterator insert(pointer n)
    {
        return insert_from_root(n);
    }

    std::size_t count()                                           const noexcept
    {
        return m_size;
    }

    iterator find(const key_type& k)                              const noexcept
    {
        pointer current = m_root;

        while(nullptr != current)
        {
            if(key_compare()(current->key, k))
            {
                std::cout << k << " > " << current->key << std::endl;
                current  = current->right;
            }
            else if(key_compare()(k, current->key))
            {
                std::cout << k << " < " << current->key << std::endl;
                current  = current->left;
            }
            else
            {
                std::cout << "Found!" << std::endl;
                return iterator(current);
            }
        }

        return iterator(current);
    }

protected:
    iterator insert_from_root(pointer n)
    {
        if(nullptr == m_root)
        {
            m_root          = n;
            m_root->parent  = nullptr;
            m_header.left   = m_root;
            m_header.right  = m_root;
            m_header.parent = m_root;

            ++m_size;

            return m_root;
        }

        return insert_from_node(n, m_root);
    }

    iterator insert_from_node(pointer n, pointer node_to)
    {
        if(key_compare()(n->key, node_to->key))
        {
            std::cout << n->key << " < " << node_to->key << std::endl;
            if(nullptr == node_to->left)
            {
                node_to->left = n;
                n->parent = node_to;

                ++m_size;

                return iterator(n);
            }

            return insert_from_node(n, node_to->left);
        }
        else if(key_compare()(node_to->key, n->key))
        {
            std::cout << n->key << " > " << node_to->key << std::endl;
            if(nullptr == node_to->right)
            {
                node_to->right = n;
                n->parent = node_to;

                ++m_size;

                return iterator(n);
            }

            return insert_from_node(n, node_to->right);
        }
        else // equality
        {
            std::cout << n->key << " = " << node_to->key << std::endl;
            node_to->val = n->val;
            return node_to;
        }
    }

    pointer rotate_left(pointer n)                                      noexcept
    {
        std::cout << "rotate left" << std::endl;

        pointer saved_left  = n->left;
        pointer saved_g     = n->grandparent();
        n->left             = n->parent;
        n->left->parent     = n;
        n->left->right      = saved_left;
        n->parent           = saved_g;
        saved_g->left       = n;

        return n;
    }

    pointer rotate_right(pointer n)                                     noexcept
    {
        std::cout << "rotate right" << std::endl;

        pointer saved_right = n->right;
        pointer saved_g     = n->grandparent();
        n->right            = n->parent;
        n->right->parent    = n;
        n->right->left      = saved_right;
        n->parent           = saved_g;
        saved_g->right      = n;

        return n;
    }

    pointer big_rotate_left(pointer n)                                  noexcept
    {
        std::cout << "big rotate left" << std::endl;

        pointer saved_r   = n->right;
        pointer saved_rl  = saved_r->left;
        pointer saved_par = n->parent;

        if(nullptr != saved_par)
        {
            if(saved_par->left == n)
            {
                saved_par->left = saved_r;
            }
            else
            {
                saved_par->right = saved_r;
            }
        }

        saved_r->left    = n;
        n->parent        = saved_r;

        saved_r->parent  = saved_par;

        if(nullptr != saved_rl)
        {
            saved_rl->parent = n;
        }
        n->right         = saved_rl;

        return n->parent;
    }

    pointer big_rotate_right(pointer n)                                 noexcept
    {
        std::cout << "big rotate right" << std::endl;

        pointer saved_l   = n->left;
        pointer saved_lr  = saved_l->right;
        pointer saved_par = n->parent;

        if(nullptr != saved_par)
        {
            if(saved_par->left == n)
            {
                saved_par->left = saved_l;
            }
            else
            {
                saved_par->right = saved_l;
            }
        }

        saved_l->right   = n;
        n->parent        = saved_l;

        saved_l->parent  = saved_par;

        if(nullptr != saved_lr)
        {
            saved_lr->parent = n;
        }
        n->left          = saved_lr;

        return n->parent;
    }

protected:
    node_t      m_header;
    pointer     m_root    {};
    std::size_t m_size    { 0 };
};

} // namespace tree

} // namespace ecl

#endif //ECL_BINARY_TREE
