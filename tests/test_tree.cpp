// #include <ecl/tree/rb_tree.hpp>
#include <iostream>

#include <ecl/tree/balanced_binary_tree.hpp>
#include <ecl/tree/red_black_tree.hpp>

#include <string>

#include <random>

#define NODES_COUNT 20

using tree_t         = ecl::tree::balanced_binary_tree<int, std::string>;
using tree_node_t    = typename tree_t::node_t;

using rb_tree_t      = ecl::tree::red_black_tree<int, std::string/*, ecl::tree::pointer_type::SHARED*/>;
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

static rb_tree_node_t rb_nodes [] =
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

void print_indent(std::size_t indent)
{
    for(std::size_t i = 0; i < indent; ++i)
    {
        std::cout << "      ";
    }
}

template<typename N>
void print(const N* n, std::size_t indent = 0)
{
    if(nullptr != n->right)
    {
        print(n->right, indent + 1);
    }

    print_indent(indent);
    if(n->color == ecl::tree::node_color::BLACK)
    {
        std::cout << "[B] " << n->key << std::endl;
    }
    else
    {
        std::cout << "[R] " << n->key << std::endl;
    }

    if(nullptr != n->left)
    {
        print(n->left, indent + 1);
    }
}


int main(int, char**, char**)
{
    std::default_random_engine e1;
    std::uniform_int_distribution<int> uniform_dist(0, NODES_COUNT);

    // tree_t t1;

    // for(auto& n : nodes)
    // {
    //     std::cout << "[BT] Inserting node: (" << n.key << "; " << n.val << ")" << std::endl;
    //     t1.insert(&n);
    // }

    // tree_t t2;

    // for(std::size_t i = 0; i < NODES_COUNT; ++i)
    // {
    //     int v = uniform_dist(e1);
    //     std::cout << "[BT] Inserting node: (" << v << "; " << std::to_string(v) << ")" << std::endl;
    //     tree_node_t::pointer p = new tree_node_t(v, std::to_string(v));
    //     std::cout << "Pointer = " << std::hex << p << std::dec << std::endl;
    //     if(t2.insert( p) != p)
    //     {
    //         std::cout << "[BT] Deleting node: (" << v << "; " << std::to_string(v) << ")" << std::endl;
    //         delete p;
    //     }

    //     std::cout << "find: " << v << std::endl << *(t2.find(v)) << std::endl;
    // }

    // rb_tree_t rb_t1;

    // for(auto& n : rb_nodes)
    // {
    //     std::cout << "[RB] Inserting node: (" << n.key << "; " << n.val << ")" << std::endl;
    //     rb_t1.insert(&n);
    // }

    rb_tree_t rb_t2;

    for(std::size_t i = 0; i < NODES_COUNT; ++i)
    {
        int v = uniform_dist(e1);
        std::cout << "[RB] Inserting node: (" << v << "; " << std::to_string(v) << ")" << std::endl;
        // rb_t2.insert(std::make_shared<rb_tree_node_t>(v, std::to_string(v)));

        rb_tree_node_t::pointer p = new rb_tree_node_t(v, std::to_string(v));
        // std::cout << "Pointer = " << std::hex << p << std::dec << std::endl;
        if(rb_t2.insert(p) != p)
        {
            std::cout << "[RB] Deleting node: (" << v << "; " << std::to_string(v) << ")" << std::endl;
            delete p;
        }

        print(rb_t2.root());
    }

    for(auto& v : rb_t2)
    {
        std::cout << "[RB iterator] : " << v << std::endl;
    }

    auto rb_it_begin = rb_t2.rbegin();
    auto rb_it_end = rb_t2.rend();

    for(auto rb_it = rb_t2.rbegin(); rb_it != rb_it_end; ++rb_it)
    {
        std::cout << "[RB reverse iterator] : " << *rb_it << std::endl;
    }
}
