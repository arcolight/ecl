#ifndef ECL_SPLAY_TREE
#define ECL_SPLAY_TREE

#include <ecl/tree/binary_tree_base.hpp>

namespace ecl
{

namespace tree
{

template
<
      typename K
    , typename V
    , template <typename> class Compare = std::less
    , typename Storage = void
>
struct splay_node : public node_base<K, V, Compare, ecl::tree::splay_node, Storage>
{
    // Full namespace is workaround for clang bug
    // about template-template parameters
    //
    // http://stackoverflow.com/questions/17687459/clang-not-accepting-use-of-template-template-parameter-when-using-crtp
    using base = node_base<K, V, Compare, ecl::tree::splay_node, Storage>;

    using node_base<K, V, Compare, ecl::tree::splay_node, Storage>::node_base;
    using typename base::pointer;

    void replace_from(pointer)                                          noexcept
    {
    }
};

template
<
      typename K
    , typename V
    , template <typename> class Compare = std::less
    , typename Storage = void
>
class splay_tree : public binary_tree_base<K, V, Compare, splay_node, Storage>
{
    using base = binary_tree_base<K, V, Compare, splay_node, Storage>;

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

        splay(inserted_n);

        return it;
    }

    erase_return erase(const key_type& k)                               noexcept
    {
        if(empty())
        {
            return { nullptr, nullptr };
        }

        pointer to_erase = root()->find(k);
        if(nullptr == to_erase)
        {
            return { nullptr, nullptr };
        }

        erase_return ret = this->base::erase_internal(to_erase);

        if(to_erase->have_parent())
        {
            splay(to_erase->parent);
        }

        return ret;
    }

    iterator find(const key_type& k)                                    noexcept
    {
        pointer p = m_header.parent->find(k);
        if(nullptr == p)
        {
            return end();
        }

        splay(p);

        return iterator(p, pointer(&m_header));
    }

private:
    // void join(pointer s, pointer t)                                     noexcept
    // {
    //     splay(s->most_right());
    //     link_as_right(t, s);
    // }

    void splay(pointer n)                                               noexcept
    {
        pointer x = n;
        pointer p = n->parent;

        if(root() == x)
        {
            return;
        }

        bool x_p_is_ll    = p->is_left()  && x->is_left();
        bool x_p_is_rr    = p->is_right() && x->is_right();
        bool x_p_is_ll_rr = x_p_is_ll || x_p_is_rr;
        bool x_p_is_lr_rl = !x_p_is_ll_rr;
        bool p_is_root    = root() == p;

        if(p_is_root)
        {
            zig(x, p);
        }

        if( !p_is_root && x_p_is_ll_rr)
        {
            zig_zig(x, p);
        }

        if( !p_is_root && x_p_is_lr_rl)
        {
            zig_zag(x, p);
        }
    }

    void zig(pointer x, pointer p)                                      noexcept
    {
        if(x->is_left())
        {
            this->rotate_right(p);
        }
        else
        {
            this->rotate_left(p);
        }
    }

    void zig_zig(pointer x, pointer p)                                  noexcept
    {
        if(x->is_left())
        {
            this->rotate_right(p->parent);
            this->rotate_right(p);
        }
        else
        {
            this->rotate_left(p->parent);
            this->rotate_left(p);
        }
    }

    void zig_zag(pointer, pointer p)                                    noexcept
    {
        pointer g = p->parent;

        if(p->is_left())
        {
            this->rotate_left(p);
            this->rotate_right(g);
        }
        else
        {
            this->rotate_right(p);
            this->rotate_left(g);
        }
    }
};

} // namespace tree

} // namespace ecl

#endif // ECL_SPLAY_TREE
