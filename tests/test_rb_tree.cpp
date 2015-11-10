#include <ecl/rb_tree.hpp>

#define NODES_COUNT 100000

using tree_t      = ecl::rb_tree<std::size_t, const char*>;
using tree_node_t = typename tree_t::node_t;

static tree_node_t nodes [NODES_COUNT] = {};
static const char* values[NODES_COUNT];

int main(int, char**, char**)
{
    tree_t t;

    for(auto& n : nodes)
    {
        t.insert(&n);
    }
}
