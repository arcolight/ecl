#ifndef ECL_SIMPLE_BINARY_TREE
#define ECL_SIMPLE_BINARY_TREE

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
>
struct node : public node_base<K, V, Compare, ecl::tree::node>
{
    // Full namespace is workaround for clang bug
    // about template-template parameters
    //
    // http://stackoverflow.com/questions/17687459/clang-not-accepting-use-of-template-template-parameter-when-using-crtp
    using base = node_base<K, V, Compare, ecl::tree::node>;

    using node_base<K, V, Compare, ecl::tree::node>::node_base;
};

template
<
      typename K
    , typename V
    , template <typename> class Compare = std::less
>
class simple_binary_tree : public binary_tree_base<K, V, Compare, node>
{
};

} // namespace tree

} // namespace ecl

#endif // ECL_SIMPLE_BINARY_TREE
