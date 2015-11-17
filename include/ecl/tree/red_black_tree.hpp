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
    typename V,
    pointer_type PT = pointer_type::RAW
>
struct red_black_node : public node_base<K, V, PT, ecl::tree::red_black_node>
{
    // Full namespace is workaround for clang bug
    // about template-template parameters
    //
    // http://stackoverflow.com/questions/17687459/clang-not-accepting-use-of-template-template-parameter-when-using-crtp
    using base = node_base<K, V, PT, ecl::tree::red_black_node>;

    using node_base<K, V, PT, ecl::tree::red_black_node>::node_base;
    using typename base::pointer;
    using base::grandparent;
    using base::uncle;

    using base::print_indent;
    using base::key;
    using base::left;
    using base::right;
    void print(const char* s, std::size_t indent)
    {
        print_indent(indent);
        if(color == node_color::BLACK)
        {
            std::cout << s << key << " [B]" << std::endl;
        }
        else
        {
            std::cout << s << key << " [R]"<< std::endl;
        }
        if(nullptr != left)
        {
            left->print("l. ", indent + 1);
        }
        if(nullptr != right)
        {
            right->print("r. ", indent + 1);
        }
    }

    node_color color { node_color::RED };
};

template
<
    typename K,
    typename V,
    pointer_type PT  = pointer_type::RAW,
    typename Compare = std::less<const K>
>
class red_black_tree : public binary_tree<K, V, PT, Compare, red_black_node>
{
    using base = binary_tree<K, V, PT, Compare, red_black_node>;
    using typename base::key_compare;
    using base::m_root;
    using base::m_header;
public:
    using typename base::node_t;
    using typename base::pointer;

    using typename base::iterator;
    using typename base::const_iterator;
    using typename base::reverse_iterator;
    using typename base::const_reverse_iterator;

    iterator insert(pointer n)
    {
        iterator result = this->insert_from_root(n);
        n->color = node_color::RED;

        m_root->print("p. ", 0);

        insert_case1(n);

        m_root->print("p. ", 0);

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
        std::cout << "case 1." << std::endl;
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
        std::cout << "case 2." << std::endl;
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
        std::cout << "case 3." << std::endl;
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
     *  rotate left case:
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
        std::cout << "case 4." << std::endl;
        pointer g = n->grandparent();

        if((n == n->parent->right) && (n->parent == g->left))
        {
            this->rotate_left(n);
            n->color = node_color::BLACK;
        }
        else if((n == n->parent->left) && (n->parent == g->right))
        {
            this->rotate_right(n);
            n->color = node_color::BLACK;
        }

        insert_case5(n);
    }

    /**
     *  Case 5
     *  ======
     *  big rotate right case:
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
        std::cout << "case 5." << std::endl;
        pointer g = n->grandparent();

        n->parent->color = node_color::BLACK;
        g->color = node_color::RED;

        if((n == n->parent->left) && (n->parent == g->left))
        {
            this->big_rotate_right(g);
        }
        else
        {
            this->big_rotate_left(g);
        }

        if(m_root == g)
        {
            m_root = g->parent;
        }
    }

};

} // namespace tree

} // namespace ecl

#endif // ECL_RB_TREE
