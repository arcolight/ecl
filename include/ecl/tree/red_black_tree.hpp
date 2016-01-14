#ifndef ECL_RB_TREE
#define ECL_RB_TREE

#include <ecl/tree/binary_tree_base.hpp>

namespace ecl
{

namespace tree
{

enum class node_color
{
      BLACK
    , RED
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
    using typename base::pointer;

    inline bool is_black()                                        const noexcept
    {
        return color == node_color::BLACK;
    }

    inline bool is_red()                                          const noexcept
    {
        return color == node_color::RED;
    }

    inline void mark_black()                                            noexcept
    {
        color = node_color::BLACK;
    }

    inline void mark_red()                                              noexcept
    {
        color = node_color::RED;
    }

    inline void mark_as(pointer p)                                      noexcept
    {
        color = p->color;
    }

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
    using typename base::node_t;

    using typename base::pointer;
    using typename base::const_pointer;

    using typename base::key_type;
    using typename base::value_type;

    using typename base::key_compare;

    using typename base::erase_return;

    using typename base::iterator;
    using typename base::const_iterator;
    using typename base::reverse_iterator;
    using typename base::const_reverse_iterator;

    using base::root;
    using base::count;

    iterator insert(pointer n)                                          noexcept
    {
        iterator result = this->base::insert(n);
        n->mark_red();

        insert_case1(n);

        return result;
    }

    pointer erase(const key_type& k)                                    noexcept
    {
        erase_return ret = this->base::erase_internal(k);
        pointer v = ret.first;  // removed
        pointer u = ret.second; // successor

        if(nullptr == v) // not found
        {
            return nullptr;
        }

        if(nullptr == u)
        {
            return v;
        }

        if(v->is_black())
        {
            if(u->is_red())
            {
                u->mark_black();
            }
            else
            {
                delete_case1(u);
            }
        }

        return v;
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
    void insert_case1(pointer n)                                        noexcept
    {
        if( ! n->have_parent())
        {
            n->mark_black();
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
    void insert_case2(pointer n)                                        noexcept
    {
        if(n->parent->is_black())
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
    void insert_case3(pointer n)                                        noexcept
    {
        pointer u = n->uncle();

        if(nullptr == u)
        {
            insert_case4(n);
            return;
        }

        if(u->is_red() && n->parent->is_red())
        {
            n->parent->mark_black();
            u->mark_black();

            pointer g = n->grandparent();
            g->mark_red();

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
    void insert_case4(pointer n)                                        noexcept
    {
        if(n->is_right() && n->parent->is_left())
        {
            this->rotate_left(n->parent);
            n->mark_black();
            n = n->left;
        }
        else if(n->is_left() && n->parent->is_right())
        {
            this->rotate_right(n->parent);
            n->mark_black();
            n = n->right;
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
    void insert_case5(pointer n)                                        noexcept
    {
        pointer g = n->grandparent();

        n->parent->mark_black();
        g->mark_red();

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

    void delete_case1(pointer n)                                        noexcept
    {
        if(n->have_parent())
        {
            delete_case2(n);
        }
    }

    /**
     *  Case 2
     *  ======
     *  rotate left around P:
     *
     *          ---- P [B] ----                                  ---- S [B] ----
     *        /                 \                              /                 \
     *      N [B]           -- S [R] --                   -- P [R] --          Sr [B]
     *    /      \        /            \        ->      /            \        /      \
     *   1        2     Sl [B]       Sr [B]           N [B]        Sl [B]    5        6
     *                /       \     /      \        /      \      /      \
     *               3         4   5        6      1        2    3        4
     *
     */
    void delete_case2(pointer n)                                        noexcept
    {
        pointer s = n->sibling();

        // nullptr equals 'black' node, so we can safely return from this point
        if(nullptr == s)
        {
            return;
        }

        if(s->is_red())
        {
            n->parent->mark_red();
            s->mark_black();
            if(n->is_left())
            {
                this->rotate_left(n->parent);
            }
            else
            {
                this->rotate_right(n->parent);
            }

            delete_case3(n);
        }
    }

    /**
     *  Case 3
     *  ======
     *  mark S as black:
     *
     *          ---- P [B] ----                             ---- P [B] ----
     *        /                 \                         /                 \
     *      N [B]           -- S [B] --                 N [B]           -- S [R] --
     *    /      \        /            \       ->     /      \        /            \
     *   1        2     Sl [B]       Sr [B]          1        2     Sl [B]       Sr [B]
     *                /       \     /      \                      /       \     /      \
     *               3         4   5        6                    3         4   5        6
     *
     */
    void delete_case3(pointer n)                                        noexcept
    {
        pointer s = n->sibling();

        if(n->parent->is_black() &&
           s->is_black()         &&
           s->left->is_black()   &&
           s->right->is_black() )
        {
            s->mark_red();
            delete_case1(n->parent);
        }
        else
        {
            delete_case4(n);
        }
    }

    /**
     *  Case 4
     *  ======
     *  mark S as red and P as black:
     *
     *          ---- P [R] ----                             ---- P [B] ----
     *        /                 \                         /                 \
     *      N [B]           -- S [B] --                 N [B]           -- S [R] --
     *    /      \        /            \       ->     /      \        /            \
     *   1        2     Sl [B]       Sr [B]          1        2     Sl [B]       Sr [B]
     *                /       \     /      \                      /       \     /      \
     *               3         4   5        6                    3         4   5        6
     *
     */
    void delete_case4(pointer n)                                        noexcept
    {
        pointer s = n->sibling();

        if(n->parent->is_red() &&
           s->is_black()       &&
           s->left->is_black() &&
           s->right->is_black() )
        {
            s->mark_red();
            n->parent->mark_black();
        }
        else
        {
            delete_case5(n);
        }
    }

    /**
     *  Case 5
     *  ======
     *  rotate right around Sl:
     *
     *         -- S [B] --                            Sl [B]
     *       /            \       ->                 /      \
     *     Sl [R]       Sr [B]                      1      S [R]
     *   /       \     /      \                           /     \
     *  1         2   3        4                         2    Sr [B]
     *                                                       /      \
     *                                                      3        4
     *
     */
    void delete_case5(pointer n)                                        noexcept
    {
        pointer s = n->sibling();

        if(s->is_black())
        {
            if(n->is_left()         &&
               s->right->is_black() &&
               s->left->is_right())
            {
                s->mark_red();
                s->left->mark_black();
                this->rotate_right(s);
            }
            else if(n->is_right()       &&
                    s->left->is_black() &&
                    s->right->is_red())
            {
                s->mark_red();
                s->right->mark_black();
                this->rotate_left(s);
            }
        }

        delete_case6(n);
    }

    /**
     *  Case 6
     *  ======
     *  rotate left around P:
     *
     *         -- P [?] --                               -- S [?] --
     *       /            \                            /             \
     *     N [B]         S [B]           ->          P [B]         Sr [B]
     *   /      \       /     \                    /      \       /      \
     *  1        2     3    Sr [R]               N [B]     3     4        5
     *                     /      \             /     \
     *                    4        5           1       2
     *
     */
    void delete_case6(pointer n)                                        noexcept
    {
        pointer s = n->sibling();

        s->mark_as(n->parent);
        n->parent->mark_black();

        if(n->is_left())
        {
            s->right->mark_black();
            this->rotate_left(n->parent);
        }
        else
        {
            s->left->mark_black();
            this->rotate_right(n->parent);
        }
    }
};

} // namespace tree

} // namespace ecl

#endif // ECL_RB_TREE
