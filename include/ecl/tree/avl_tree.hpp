#ifndef ECL_AVL_TREE
#define ECL_AVL_TREE

#include <ecl/tree/binary_tree_base.hpp>

#include <algorithm>
#include <cstdint>

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
struct avl_node : public node_base<K, V, Compare, ecl::tree::avl_node, Storage>
{
    // Full namespace is workaround for clang bug
    // about template-template parameters
    //
    // http://stackoverflow.com/questions/17687459/clang-not-accepting-use-of-template-template-parameter-when-using-crtp
    using base = node_base<K, V, Compare, ecl::tree::avl_node, Storage>;

    using node_base<K, V, Compare, ecl::tree::avl_node, Storage>::node_base;
    using typename base::pointer;

    using height_t = std::int8_t;

    height_t height { 1 };

    void replace_from(pointer s)                                        noexcept
    {
        height = s->height;
        fix_height(this);
    }
};

template
<
      typename K
    , typename V
    , template <typename> class Compare
    , typename Storage
>
auto height(const avl_node<K, V, Compare, Storage>* n)                  noexcept
    -> typename avl_node<K, V, Compare, Storage>::height_t
{
    return (nullptr == n) ? 0 : n->height;
}

template
<
      typename K
    , typename V
    , template <typename> class Compare
    , typename Storage
>
auto balance_factor(const avl_node<K, V, Compare, Storage>* n)          noexcept
    -> typename avl_node<K, V, Compare, Storage>::height_t
{
    if(nullptr == n)
    {
        return 0;
    }

    return height(n->right) - height(n->left);
}

template
<
      typename K
    , typename V
    , template <typename> class Compare
    , typename Storage
>
bool is_left_heavy(const avl_node<K, V, Compare, Storage>* n)           noexcept
{
    return (balance_factor(n) < 0);
}

template
<
      typename K
    , typename V
    , template <typename> class Compare
    , typename Storage
>
bool is_right_heavy(const avl_node<K, V, Compare, Storage>* n)          noexcept
{
    return (balance_factor(n) > 0);
}

template
<
      typename K
    , typename V
    , template <typename> class Compare
    , typename Storage
>
bool is_balanced(const avl_node<K, V, Compare, Storage>* n)             noexcept
{
    return (balance_factor(n) == 0);
}

template
<
      typename K
    , typename V
    , template <typename> class Compare
    , typename Storage
>
void fix_height(avl_node<K, V, Compare, Storage>* n)                    noexcept
{
    if(nullptr == n)
    {
        return;
    }

    auto hl = height(n->left);
    auto hr = height(n->right);

    n->height = std::max(hl, hr) + 1;
}

template
<
      typename K
    , typename V
    , template <typename> class Compare = std::less
    , typename Storage = void
>
class avl_tree : public binary_tree_base<K, V, Compare, avl_node, Storage>
{
    using base = binary_tree_base<K, V, Compare, avl_node, Storage>;

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

        balance(inserted_n->parent);

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

        erase_return ret  = this->base::erase_internal(to_erase);
        pointer successor = ret.second;

        if(nullptr != successor)
        {
            balance(successor->successor());
        }
        else
        {
            balance(to_erase->parent);
        }

        return ret;
    }

private:
    void balance(pointer n)                                             noexcept
    {
        if(nullptr == n)
        {
            return;
        }

        fix_height(n);

        if(balance_factor(n) == 2)
        {
            if(balance_factor(n->right) < 0)
            {
                this->rotate_right_balance(n->right);
            }
            this->rotate_left_balance(n);
        }
        else if(balance_factor(n) == -2)
        {
            if(balance_factor(n->left) > 0)
            {
                this->rotate_left_balance(n->left);
            }
            this->rotate_right_balance(n);
        }

        balance(n->parent);
    }

    void rotate_left_balance(pointer n)                                 noexcept
    {
        this->rotate_left(n);
        fix_height(n->right);
        fix_height(n);
    }

    void rotate_right_balance(pointer n)                                noexcept
    {
        this->rotate_right(n);
        fix_height(n->left);
        fix_height(n);
    }
};

} // namespace tree

} // namespace ecl

#endif // ECL_AVL_TREE
