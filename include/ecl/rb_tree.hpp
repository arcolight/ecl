#ifndef ECL_RB_TREE
#define ECL_RB_TREE

#include <cstddef>

#include <utility>
#include <iterator>

namespace ecl
{

template<typename K, typename V, typename Compare = std::less<const K>>
class rb_tree
{
public:
    enum class rb_color
    {
        BLACK = true  ,
        RED   = false
    };

    using key_type       = const K; // we cannot change keys after init
    using value_type     = V;
    using key_compare    = Compare;

    struct node_base
    {
        using pointer = node_base*;
        pointer left   { nullptr };
        pointer right  { nullptr };
        pointer parent { nullptr };
    };

    using node_t       = node;
    using pointer      = node_base::pointer;

    struct node : public node_base
    {
        using base    = node_base;
        using pointer = typename base::pointer;

        explicit node (key_type&& k, value_type&& v)
            : key  ( std::forward<key_type>   (k) )
            , data ( std::forward<value_type> (v) )
        {}

        explicit node (std::pair<key_type, value_type>&& p)
            : key  ( std::forward<key_type>   (p.first ) )
            , data ( std::forward<value_type> (p.second) )
        {}

        node(node&& other)
        {
            std::swap(color,  other.color );
            std::swap(left,   other.left  );
            std::swap(right,  other.right );
            std::swap(parent, other.parent);
            key = other.key; // we cannot swap constant key
            std::swap(data,   other.data  );
        }

        rb_color   color  { rb_color::RED };

        key_type   key  {};
        value_type data {};

        pointer min()                                             const noexcept
        {
            pointer result = this;
            while(nullptr != result->left)
            {
                result = result->left;
            }
            return result;
        }

        pointer max()                                             const noexcept
        {
            pointer result = this;
            while(nullptr != result->right)
            {
                result = result->right;
            }
            return result;
        }
    };

    struct base_iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = ptrdiff_t;

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
                link_t y = m_n->parent;
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

        }

        link_t m_n;
    };

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

        iterator(link_t n)
        {
            m_n = n;
        }

        self_type& operator++()                                        noexcept(
                                            noexcept(base_iterator::increment())
                                                                               )
        {
            increment();
            return *this;
        }

        self_type  operator++(int)                                     noexcept(
                                            noexcept(base_iterator::increment())
                                                                               )
        {
            self_type tmp = *this;
            increment();
            return tmp;
        }

        self_type& operator--()                                        noexcept(
                                            noexcept(base_iterator::decrement())
                                                                               )
        {
            decrement();
            return *this;
        }

        self_type  operator--(int)                                     noexcept(
                                            noexcept(base_iterator::decrement())
                                                                               )
        {
            self_type tmp = *this;
            decrement();
            return tmp;
        }

        reference  operator*()                                          noexcept
        {
            return *(m_n->data);
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

        const_iterator(link_t n)
        {
            m_n = n;
        }

        self_type& operator++()                                        noexcept(
                                            noexcept(base_iterator::increment())
                                                                               )
        {
            increment();
            return *this;
        }

        self_type  operator++(int)                                     noexcept(
                                            noexcept(base_iterator::increment())
                                                                               )
        {
            self_type tmp = *this;
            increment();
            return tmp;
        }

        self_type& operator--()                                        noexcept(
                                            noexcept(base_iterator::decrement())
                                                                               )
        {
            decrement();
            return *this;
        }

        self_type  operator--(int)                                     noexcept(
                                            noexcept(base_iterator::decrement())
                                                                               )
        {
            self_type tmp = *this;
            decrement();
            return tmp;
        }

        reference  operator*()                                    const noexcept
        {
            return *(m_n->data);
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
        return const_iterator(m_header.end);
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

    rb_tree()
    {

    }

    iterator find(key_type& k)           const noexcept(noexcept(key_compare()))
    {
        node_t*& current = m_root;

        while(nullptr != current)
        {
            if(m_compare(k, current->key))
            {
                current = current->left;
            }
            else if(m_compare(current->key, k))
            {
                current = current->right;
            }
            else
            {
                return current;
            }
        }

        return current;
    }

    iterator insert(link_t n)
    {
        if(nullptr == n)
        {
            return end();
        }

        return iterator(n);
    }

private:
    void link_as_left(link_t p, link_t l)                         const noexcept
    {
        if(nullptr != p && nullptr != l)
        {
            p->left   = l;
            l->parent = p;
        }
    }

    void link_as_right(link_t p, link_t r)                        const noexcept
    {
        if(nullptr != p && nullptr != r)
        {
            p->right  = r;
            r->parent = p;
        }
    }

    void rotate_left(link_t x)                                   const noexcept(
                                                    noexcept(link_as_right()) &&
                                                    noexcept(link_as_left())   )
    {
        link_t y = x->right;

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

    void rotate_right(link_t x)                                  const noexcept(
                                                    noexcept(link_as_right()) &&
                                                    noexcept(link_as_left())   )
    {
        link_t y = x->left;

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

    void fixup(link_t x)                                          const noexcept
    {
        link_t y = nullptr;

        while ( x->parent->color == rb_color::RED )
        {
            if ( x->parent == x->parent->parent->left )
            {
                y = x->parent->parent->right;

                if ( x->color == rb_color::RED )
                {
                    x->parent->color = rb_color::BLACK;
                    y->color = rb_color::BLACK;
                    x->parent->parent->color = rb_color::RED;
                    x = x->parent->parent;
                }
                else
                {
                    if ( x == x->parent->right )
                    {
                        x = x->parent;
                        rotate_left( x );
                    }

                    x->parent->color = rb_color::BLACK;
                    x->parent->parent->color = rb_color::RED;
                    rotate_right( x->parent->parent );
                }
            }
            else
            {
                y = x->parent->parent->left;

                if ( y->color == rb_color::RED )
                {
                    x->parent->color = rb_color::BLACK;
                    y->color = rb_color::BLACK;
                    x->parent->parent->color = rb_color::RED;
                    x = x->parent->parent;
                }
                else
                {
                    if ( x == x->parent->left )
                    {
                        x = x->parent;
                        rotate_right( x );
                    }
                    x->parent->color = rb_color::BLACK;
                    x->parent->parent->color = rb_color::RED;
                    rotate_left( x->parent->parent );
                }
            }
        }

        m_root->color = rb_color::BLACK;
    }

    node_base   node_base    {};

    link_t      m_root       { nullptr };
    std::size_t m_node_count { 0       };
    key_compare m_compare    {         };
};

} // namespace ecl

#endif // ECL_RB_TREE
