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
    , typename Storage = void
>
struct red_black_node : public node_base
                               <
                                     K
                                   , V
                                   , Compare
                                   , ecl::tree::red_black_node
                                   , Storage
                               >
{
    // Full namespace is workaround for clang bug
    // about template-template parameters
    //
    // http://stackoverflow.com/questions/17687459/clang-not-accepting-use-of-template-template-parameter-when-using-crtp
    using base = node_base<K, V, Compare, ecl::tree::red_black_node, Storage>;

    using node_base
          <
                K
              , V
              , Compare
              , ecl::tree::red_black_node
              , Storage
          >::node_base;
    using typename base::pointer;

    node_color color { node_color::RED };
};

template
<
      typename K
    , typename V
    , template <typename> class Compare
    , typename Storage
>
bool is_black(const red_black_node<K, V, Compare, Storage>* n)          noexcept
{
    if(nullptr == n)
    {
        return true;
    }

    return n->color == node_color::BLACK;
}

template
<
      typename K
    , typename V
    , template <typename> class Compare
    , typename Storage
>
bool is_red(const red_black_node<K, V, Compare, Storage>* n)            noexcept
{
    if(nullptr == n)
    {
        return false;
    }

    return n->color == node_color::RED;
}

template
<
      typename K
    , typename V
    , template <typename> class Compare
    , typename Storage
>
void mark_black(red_black_node<K, V, Compare, Storage>* n)              noexcept
{
    if(nullptr == n)
    {
        return;
    }

    n->color = node_color::BLACK;
}

template
<
      typename K
    , typename V
    , template <typename> class Compare
    , typename Storage
>
void mark_red(red_black_node<K, V, Compare, Storage>* n)                noexcept
{
    if(nullptr == n)
    {
        return;
    }

    n->color = node_color::RED;
}

template
<
      typename K
    , typename V
    , template <typename> class Compare
    , typename Storage
>
void mark_as(red_black_node<K, V, Compare, Storage>* n,
             red_black_node<K, V, Compare, Storage>* other)             noexcept
{
    if(nullptr == n)
    {
        return;
    }

    if(nullptr == other)
    {
        n->color = node_color::BLACK;
    }
    else
    {
        n->color = other->color;
    }
}

template
<
      typename K
    , typename V
    , template <typename> class Compare = std::less
    , typename Storage = void
>
class red_black_tree : public binary_tree_base
                              <
                                    K
                                  , V
                                  , Compare
                                  , red_black_node
                                  , Storage
                              >
{
    using base = binary_tree_base<K, V, Compare, red_black_node, Storage>;

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
    using base::empty;

    using base::begin;
    using base::end;
    using base::cbegin;
    using base::cend;
    using base::rbegin;
    using base::rend;
    using base::crbegin;
    using base::crend;

    iterator insert(pointer n, bool allow_update = true)                noexcept
    {
        auto result        = this->base::insert_internal(n, allow_update);
        pointer inserted_n = result.second;
        iterator it        = result.first;

        // Node with such key is existing.
        if(n != inserted_n)
        {
            return it;
        }

        mark_red(inserted_n);

        insert_case1(inserted_n);

        return it;
    }

    erase_return erase(const key_type& k)                               noexcept
    {
        if(empty())
        {
            return { nullptr, nullptr };
        }

        pointer to_erase = root()->find(k);
        if(nullptr == to_erase) // not found
        {
            return { nullptr, nullptr };
        }

        pointer suc   = to_erase->successor();
        pointer child = suc->have_left() ? suc->left : suc->right;

        if(is_black(suc))
        {
            // delete_fixup(suc);
            if(is_red(child))
            {
                mark_black(child);
            }
            else
            {
                if(suc->have_no_child())
                {
                    delete_case1(suc);
                }
                else
                {
                    delete_case1(child);
                }
            }
        }

        return this->erase_internal(to_erase);
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
            mark_black(n);
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
        if(is_black(n->parent))
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

        if(is_red(u) && is_red(n->parent))
        {
            mark_black(n->parent);
            mark_black(u);

            pointer g = n->grandparent();
            mark_red(g);

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
            mark_black(n);
            n = n->left;
        }
        else if(n->is_left() && n->parent->is_right())
        {
            this->rotate_right(n->parent);
            mark_black(n);
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

        mark_black(n->parent);
        mark_red(g);

        if(n->is_left() && n->parent->is_left())
        {
            this->rotate_right(g);
        }
        else
        {
            this->rotate_left(g);
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
        if(is_red(s))
        {
            mark_red(n->parent);
            mark_black(s);
            if(n->is_left())
            {
                this->rotate_left(n->parent);
            }
            else
            {
                this->rotate_right(n->parent);
            }
        }
        delete_case3(n);
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
        if(is_black(n->parent) &&
           is_black(s)         &&
           ((s == nullptr) || is_black(s->left) ) &&
           ((s == nullptr) || is_black(s->right)))
        {
            mark_red(s);
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
        if(is_red(n->parent) &&
           is_black(s)       &&
           ((s == nullptr) || is_black(s->left) ) &&
           ((s == nullptr) || is_black(s->right)))
        {
            mark_red(s);
            mark_black(n->parent);
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
        if(is_black(s))
        {
            if(n->is_left()       &&
               ((s == nullptr) || is_black(s->right)) &&
               ((s != nullptr) && is_red(s->left)))
            {
                mark_red(s);
                mark_black(s->left);
                this->rotate_right(s);
            }
            else if(n->is_right()     &&
                    ((s == nullptr) || is_black(s->left)) &&
                    ((s != nullptr) && is_red(s->right)))
            {
                mark_red(s);
                mark_black(s->right);
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
        mark_as(s, n->parent);
        mark_black(n->parent);

        if(n->is_left())
        {
            if(nullptr != s)
            {
                mark_black(s->right);
            }
            this->rotate_left(n->parent);
        }
        else
        {
            if(nullptr != s)
            {
                mark_black(s->left);
            }
            this->rotate_right(n->parent);
        }
    }
};

} // namespace tree

} // namespace ecl

#endif // ECL_RB_TREE
