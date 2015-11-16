#ifndef ECL_BALANCED_BINARY_TREE
#define ECL_BALANCED_BINARY_TREE

#include <ecl/tree/binary_tree.hpp>

namespace ecl
{

namespace tree
{

template<typename K, typename V, typename Compare = std::less<const K>>
class balanced_binary_tree : public binary_tree<K, V, Compare>
{
    using base = binary_tree<K, V, Compare>;
    using typename base::key_compare;
public:
    using typename base::pointer;
    using base::insert;
};

} // namespace tree

} // namespace ecl

#endif // ECL_BALANCED_BINARY_TREE
