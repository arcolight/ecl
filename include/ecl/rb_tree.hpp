#ifndef ECL_STATIC_RB_TREE
#define ECL_STATIC_RB_TREE

#include <cstddef>

namespace ecl
{

enum class rb_color
{
    BLACK,
    RED
};

template<typename T, std::size_t N>
class rb_tree
{
public:

    template<typename T>
    struct rb_tree_node
    {
        rb_color         color  { rb_color::BLACK };

        rb_tree_node<T>* left   { nullptr         };
        rb_tree_node<T>* right  { nullptr         };
        rb_tree_node<T>* parent { nullptr         };

        T*               data   { nullptr         };
    };

    using node_t = rb_tree_node<T>;

    rb_tree()
    {
    }

private:
    void rotate_left(node_t* x)
    {
        node_t* y = x->right;

        x->right = y->left;

        if(nullptr != y->left)
        {
            y->left->parent = x;
        }

        if (nullptr != y)
        {
            y->parent = x->parent;
        }

        if (nullptr != x->parent)
        {
            if (x == x->parent->left)
            {
                x->parent->left = y;
            }
            else
            {
                x->parent->right = y;
            }
        }
        else
        {
            m_root = y;
        }

        y->left = x;

        if (nullptr != x)
        {
            x->parent = y;
        }
    }

    void rotate_right(node_t* x)
    {
        node_t* y = x->left;

        x->left = y->right;
        if (nullptr != y->right)
        {
            y->right->parent = x;
        }

        if (nullptr != y)
        {
            y->parent = x->parent;
        }

        if (x->parent)
        {
            if (x == x->parent->right)
            {
                x->parent->right = y;
            }
            else
            {
                x->parent->left = y;
            }
        }
        else
        {
            m_root = y;
        }

        y->right = x;

        if (nullptr != x)
        {
            x->parent = y;
        }
    }

    void fix(node_t* x)
    {

    }

private:
    node_t  m_nodes[N];
    node_t* m_root { nullptr };
};

} // namespace ecl

#endif // ECL_STATIC_RB_TREE
