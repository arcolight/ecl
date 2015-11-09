#ifndef ECL_STATIC_RB_TREE
#define ECL_STATIC_RB_TREE

#include <cstddef>
#include <utility>

namespace ecl
{

enum class rb_color
{
    BLACK = true  ,
    RED   = false
};

template<typename K, typename V>
struct rb_tree_node
{
    using node_t = rb_tree_node<K, V>;
    using pair_t = std::pair<K, V>;

    constexpr rb_tree_node(pair_t&& p) :
        data { std::forward<pair_t>(p) }
    {}

    rb_color color  { rb_color::RED };

    node_t*  left   { nullptr };
    node_t*  right  { nullptr };
    node_t*  parent { nullptr };

    pair_t   data   {};
};

template<typename K, typename V, typename Compare = std::less<K>>
class rb_tree
{
public:
    using key_type    = K;
    using mapped_type = V;
    using value_type  = std::pair<const K, V>;
    using key_compare = Compare;

    using node_t      = rb_tree_node<K, V>;

    class iterator
    {

    };

    class const_iterator
    {

    };

    rb_tree()
    {
    }

    node_t*& find(const key_type& k)
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

    node_t* insert(node_t* n)
    {
        if(nullptr == n)
        {
            return nullptr;
        }
    }

private:
    void rotate_left(node_t* n)
    {
    }

    void rotate_right(node_t* n)
    {
    }

private:
    node_t*     m_root       { nullptr };
    std::size_t m_node_count { 0 };
    key_compare m_compare    {};
};

} // namespace ecl

#endif // ECL_STATIC_RB_TREE
