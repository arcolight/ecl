// #include <ecl/tree/rb_tree.hpp>
#include <iostream>

#include <ecl/tree/balanced_binary_tree.hpp>
#include <ecl/tree/red_black_tree.hpp>

#include <string>

#include <random>

#define NODES_COUNT 10

// using tree_t      = ecl::rb_tree<std::size_t, const char*>;
// using tree_node_t = typename tree_t::node_t;

// static tree_node_t nodes [NODES_COUNT] = {};
// static const char* values[NODES_COUNT];

using tree_t         = ecl::tree::balanced_binary_tree<int, std::string>;
using tree_node_t    = typename tree_t::node_t;

using rb_tree_t      = ecl::tree::red_black_tree<int, std::string>;
using rb_tree_node_t = typename rb_tree_t::node_t;

static tree_node_t nodes [] =
{
    { 6,  "6"  },
    { 17, "17" },
    { 1,  "1"  },
    { 5,  "5"  },
    { 4,  "4"  },
    { 21, "21" },
    { 18, "18" },
    { 16, "16" },
    { 11, "11" },
    { 14, "14" },
    { 15, "15" },
    { 2,  "2"  },
    { 20, "20" },
    { 10, "10" },
    { 23, "23" },
    { 24, "24" },
    { 9,  "9"  },
    { 8,  "8"  },
    { 3,  "3"  },
    { 12, "12" },
    { 13, "13" },
    { 22, "22" },
    { 7,  "7"  },
    { 19, "19" }
};
// static const char* values[NODES_COUNT];

int main(int, char**, char**)
{
    tree_t t1;

    for(auto& n : nodes)
    {
        std::cout << "Inserting node: (" << n.key << "; " << n.val << ")" << std::endl;
        t1.insert(&n);
    }

    tree_t t2;

    std::default_random_engine e1;
    std::uniform_int_distribution<int> uniform_dist(0, 100);

    for(std::size_t i = 0; i < 100; ++i)
    {
        int v = uniform_dist(e1);
        std::cout << "[BT] Inserting node: (" << v << "; " << std::to_string(v) << ")" << std::endl;
        t2.insert(new tree_node_t(v, std::to_string(v)));
    }

    rb_tree_t rb_t;

    for(std::size_t i = 0; i < 100; ++i)
    {
        int v = uniform_dist(e1);
        std::cout << "[RB] Inserting node: (" << v << "; " << std::to_string(v) << ")" << std::endl;
        rb_t.insert(new rb_tree_node_t(v, std::to_string(v)));
    }
    // for(auto& n : t)
    // {

    // }
}
