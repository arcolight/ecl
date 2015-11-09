#ifndef ECL_RB_TREE
#define ECL_RB_TREE

#include <cstddef>

#include <utility>
#include <iterator>

namespace ecl
{

enum class rb_color
{
    BLACK = true  ,
    RED   = false
};

template<typename K, typename V, typename Compare = std::less<K>>
struct rb_tree_node
{
    using node_t = rb_tree_node<K, V>;
    using pair_t = std::pair<K, V>;
    using link_t = node_t*;

    rb_color color  { rb_color::RED };

    link_t   left   { nullptr };
    link_t   right  { nullptr };
    link_t   parent { nullptr };

    pair_t*  data   { nullptr };
};

template<typename K, typename V, typename Compare = std::less<K>>
class rb_tree
{
public:
    using key_type     = K;
    using mapped_type  = V;
    using value_type   = std::pair<const K, V>;
    using key_compare  = Compare;

    using node_t       = rb_tree_node<K, V>;
    using node_pointer = node_t*;
    using link_t       = typename node_t::link_t;

    class iterator
    {
    public:
        using self_type         = iterator;
        using value_type        = V;
        using reference         = V&;
        using pointer           = V*;
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = ptrdiff_t;

        self_type& operator++();
        self_type  operator++(int);
        self_type& operator--();
        self_type  operator--(int);
        reference  operator*();
        bool operator==(const self_type& rhs) const;
        bool operator!=(const self_type& rhs) const;
    };

    class const_iterator
    {
    public:
        using self_type         = const_iterator;
        using value_type        = const V;
        using reference         = const V&;
        using pointer           = const V*;
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = ptrdiff_t;

        self_type& operator++();
        self_type  operator++(int);
        self_type& operator--();
        self_type  operator--(int);
        reference  operator*();
        bool operator==(const self_type& rhs) const;
        bool operator!=(const self_type& rhs) const;
    };

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    rb_tree()
    {
    }

    iterator find(const key_type& k)
    {
        node_t*& current = m_root;

        while(nullptr != current)
        {
            if(m_compare(k, current->data.first))
            {
                current = current->left;
            }
            else if(m_compare(current->data.first, k))
            {
                current = current->right;
            }
        }

        return current;
    }

    iterator insert(link_t n, const value_type* data)
    {
        if(nullptr == n)
        {
            return nullptr;
        }

        n->data = data;
    }

private:
    iterator min(link_t n)
    {
        node_t* result = n;
        while(nullptr != result)
        {
            result = result->left;
        }
        return result;
    }

    iterator max(link_t n)
    {
        node_t* result = n;
        while(nullptr != result)
        {
            result = result->right;
        }
        return result;
    }

    void rotate_left(link_t x)
    {
        if(nullptr == x) { return; }

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

    void rotate_right(link_t n)
    {
        if(nullptr == x) { return; }

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

    void insert_fixup(link_t x)
    {
        link_t y;

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

    void link_as_left(link_t p, link_t l)
    {
        p->left   = l;
        l->parent = p;
    }

    void link_as_right(link_t p, link_t r)
    {
        p->right  = r;
        r->parent = p;
    }

private:
    node_t*     m_root       { nullptr };
    std::size_t m_node_count { 0       };
    key_compare m_compare    {         };
};

} // namespace ecl

#endif // ECL_RB_TREE
