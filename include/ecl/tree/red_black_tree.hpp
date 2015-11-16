#ifndef ECL_RB_TREE
#define ECL_RB_TREE

#include <ecl/tree/binary_tree.hpp>

#include <memory>

namespace ecl
{

namespace tree
{

enum class node_color
{
    BLACK = true  ,
    RED   = false
};

template
<
    typename K,
    typename V
>
struct red_black_node : public node_base<K, V, ecl::tree::red_black_node, std::shared_ptr>
{
    // Full namespace is workaround for clang bug
    // about template-template parameters
    //
    // http://stackoverflow.com/questions/17687459/clang-not-accepting-use-of-template-template-parameter-when-using-crtp
    using base = node_base<K, V, ecl::tree::red_black_node, std::shared_ptr>;

    using node_base<K, V, ecl::tree::red_black_node, std::shared_ptr>::node_base;
    using typename base::pointer;

    node_color color { node_color::RED };
};

template<typename K, typename V, typename Compare = std::less<const K>>
class red_black_tree : public binary_tree<K, V, Compare, red_black_node<K, V>>
{
    using base = binary_tree<K, V, Compare, red_black_node<K, V>>;
    using typename base::key_compare;
    using base::m_root;
    using base::m_header;
public:
    using typename base::node_t;
    using typename base::pointer;

    pointer insert(pointer n)
    {
        pointer result = this->insert_from_root(n);
        fixup(result);
        return result;
    }

private:
    void fixup(pointer x)                                               noexcept
    {
        pointer y = nullptr;

        while ( x->parent->color == node_color::RED )
        {
            if ( x->parent == x->parent->parent->left )
            {
                y = x->parent->parent->right;

                if ( x->color == node_color::RED )
                {
                    x->parent->color = node_color::BLACK;
                    y->color = node_color::BLACK;
                    x->parent->parent->color = node_color::RED;
                    x = x->parent->parent;
                }
                else
                {
                    if ( x == x->parent->right )
                    {
                        x = x->parent;
                        this->rotate_left( x );
                    }

                    x->parent->color = node_color::BLACK;
                    x->parent->parent->color = node_color::RED;
                    this->rotate_right( x->parent->parent );
                }
            }
            else
            {
                y = x->parent->parent->left;

                if ( y->color == node_color::RED )
                {
                    x->parent->color = node_color::BLACK;
                    y->color = node_color::BLACK;
                    x->parent->parent->color = node_color::RED;
                    x = x->parent->parent;
                }
                else
                {
                    if ( x == x->parent->left )
                    {
                        x = x->parent;
                        this->rotate_right( x );
                    }
                    x->parent->color = node_color::BLACK;
                    x->parent->parent->color = node_color::RED;
                    this->rotate_left( x->parent->parent );
                }
            }
        }

        m_root->color = node_color::BLACK;
    }
};

} // namespace tree

} // namespace ecl

#endif // ECL_RB_TREE
