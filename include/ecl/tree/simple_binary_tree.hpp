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
class simple_binary_tree : public binary_tree_base<K, V, Compare>
{
};

} // namespace tree

} // namespace ecl

#endif // ECL_SIMPLE_BINARY_TREE
