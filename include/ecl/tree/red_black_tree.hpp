#ifndef ECL_RB_TREE
#define ECL_RB_TREE

#include <ecl/tree/binary_tree_base.hpp>

namespace ecl
{

namespace tree
{

enum class node_color
{
      BLACK = true
    , RED   = false
};

template
<
      typename K
    , typename V
    , template <typename> class Compare = std::less
>
struct red_black_node : public node_base<K, V, Compare, ecl::tree::red_black_node>
{
    // Full namespace is workaround for clang bug
    // about template-template parameters
    //
    // http://stackoverflow.com/questions/17687459/clang-not-accepting-use-of-template-template-parameter-when-using-crtp
    using base = node_base<K, V, Compare, ecl::tree::red_black_node>;

    using node_base<K, V, Compare, ecl::tree::red_black_node>::node_base;

    node_color color { node_color::RED };
};

template
<
      typename K
    , typename V
    , template <typename> class Compare = std::less
>
class red_black_tree : public binary_tree_base<K, V, Compare, red_black_node>
{
    using base = binary_tree_base<K, V, Compare, red_black_node>;

    using base::m_header;
public:
    using typename base::pointer;
    using typename base::const_pointer;

    using typename base::iterator;
    using typename base::const_iterator;
    using typename base::reverse_iterator;
    using typename base::const_reverse_iterator;

    using base::root;
    using base::count;

    iterator insert(pointer n)
    {
        iterator result = this->base::insert(n);
        n->color = node_color::RED;

        insert_case1(n);

        return result;
    }

private:

    /**
     *  Case 1
     *  ======
     *
     *       N [R]               N [B]
     *     /      \     ->     /      \
     *   null    null        null    null
     *
     */
    void insert_case1(pointer n)
    {
        if(nullptr == n->parent)
        {
            n->color = node_color::BLACK;
            return;
        }

        insert_case2(n);
    }

    /**
     *  Case 2
     *  ======
     *
     *           P [B] - if black OK and return else go to Case 3
     *         /
     *      N [R]
     *     /      \
     *   null    null
     *
     */
    void insert_case2(pointer n)
    {
        if(node_color::BLACK == n->parent->color)
        {
            return;
        }

        insert_case3(n);
    }

    /**
     *  Case 3
     *  ======
     *
     *               G [B]                          G [R]
     *             /      \                       /      \
     *           P [R]     U [R]                P [B]     U [B]
     *         /      \   /     \     ->      /      \   /     \
     *       N [R]     3 4       5          N [R]     3 4       5
     *     /      \                       /      \
     *    1(null)  2(null)               1(null)  2(null)
     *
     */
    void insert_case3(pointer n)
    {
        pointer u = n->uncle();

        if((nullptr != u)                &&
           (node_color::RED == u->color) &&
           (node_color::RED == n->parent->color))
        {
            n->parent->color = node_color::BLACK;
            u->color         = node_color::BLACK;

            pointer g        = n->grandparent();
            g->color         = node_color::RED;

            insert_case1(g);

            return;
        }

        insert_case4(n);
    }

    /**
     *  Case 4
     *  ======
     *  rotate left around P:
     *
     *         -- G [B] --                             -- G [B] --
     *      /              \                        /              \
     *    P [R]             U [B]                 N [B]             U [B]
     *   /     \           /     \     ->       /      \           /     \
     *  1       N [R]     4       5           P [R]     3(null)   4       5
     *        /      \                      /      \
     *       2(null)  3(null)              1        2(null)
     *
     */
    void insert_case4(pointer n)
    {
        if(n->is_right() && n->parent->is_left())
        {
            this->rotate_left(n->parent);
            n->color = node_color::BLACK;
            n        = n->left;
        }
        else if(n->is_left() && n->parent->is_right())
        {
            this->rotate_right(n->parent);
            n->color = node_color::BLACK;
            n        = n->right;
        }

        insert_case5(n);
    }

    /**
     *  Case 5
     *  ======
     *  rotate right around G:
     *
     *             G [B]                       -- P [B] --
     *           /      \                   /              \
     *         P [R]     U [B]            N [R]             G [R]
     *       /      \   /     \    ->   /      \           /    \
     *     N [R]     3 4       5       1(null)  2(null)   3     U [B]
     *   /      \                                              /     \
     *  1(null)  2(null)                                      4       5
     *
     */
    void insert_case5(pointer n)
    {
        pointer g = n->grandparent();

        n->parent->color = node_color::BLACK;
        g->color         = node_color::RED;

        if(n->is_left() && n->parent->is_left())
        {
            this->rotate_right(g);
        }
        else
        {
            this->rotate_left(g);
        }

        if(m_header.parent == g)
        {
            m_header.parent = g->parent;
            m_header.parent->parent = nullptr;
        }
    }
};

} // namespace tree

} // namespace ecl

#endif // ECL_RB_TREE
