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
    using base::m_root;
    using base::m_header;
public:
    using typename base::node_t;
    using typename base::pointer;

    pointer insert(pointer n)
    {
        return this->insert_from_root(n);
    }

private:

};

} // namespace tree

} // namespace ecl

#endif // ECL_BALANCED_BINARY_TREE
