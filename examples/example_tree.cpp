#include <ecl/tree/simple_binary_tree.hpp>
#include <ecl/tree/red_black_tree.hpp>
#include <ecl/tree/avl_tree.hpp>
#include <ecl/tree/splay_tree.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

#include <random>
#include <ctime>

#define NODES_COUNT 100

#define TREE_DUMP_SPACE "  "
#define TREE_DUMP_RIGHT " ┌"
#define TREE_DUMP_CON   " │"
#define TREE_DUMP_LEFT  " └"

#define TREE_DUMP_NODE_HAVE_NO    "─╼"
#define TREE_DUMP_NODE_HAVE_LEFT  "─┮"
#define TREE_DUMP_NODE_HAVE_BOTH  "─┾"
#define TREE_DUMP_NODE_HAVE_RIGHT "─┶"

#define TREE_PREFIX       "[BT   ] | "
#define RB_TREE_PREFIX    "[RB   ] | "
#define AVL_TREE_PREFIX   "[AVL  ] | "
#define SPLAY_TREE_PREFIX "[SPLAY] | "

using key_type          = int;
using value_type        = std::string;

using tree_t            = ecl::tree::simple_binary_tree<key_type, value_type>;
using tree_node_t       = typename tree_t::node_t;

using rb_tree_t         = ecl::tree::red_black_tree<key_type, value_type>;
using rb_tree_node_t    = typename rb_tree_t::node_t;

using avl_tree_t        = ecl::tree::avl_tree<key_type, value_type>;
using avl_tree_node_t   = typename avl_tree_t::node_t;

using splay_tree_t      = ecl::tree::splay_tree<key_type, value_type>;
using splay_tree_node_t = typename splay_tree_t::node_t;

static tree_node_t simple_nodes [] =
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

static avl_tree_node_t avl_nodes [] =
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

static splay_tree_node_t splay_nodes [] =
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

template<typename T, std::size_t N>
constexpr static std::size_t array_size(T (&)[N])
{
    return N;
}

template<typename T>
std::size_t node_height(const T* n)
{
    std::size_t height = 0;
    while(nullptr != n)
    {
        ++height;
        n = n->parent;
    }

    return height;
}

template<>
std::size_t node_height<rb_tree_node_t>(const rb_tree_node_t* n)
{
    std::size_t height = 0;
    while(nullptr != n)
    {
        if(is_black(n))
        {
            ++height;
        }
        n = n->parent;
    }

    return height;
}

template<typename T>
void check_tree_properties(const std::string prefix,
                           std::vector<std::size_t>& heights,
                           const T* n)
{
    if(nullptr == n)
    {
        return;
    }

    if(n->is_leaf())
    {
        std::size_t h = node_height(n);
        heights.push_back(h);
        std::cout << prefix << n->key << " : " << h << std::endl;
    }

    check_tree_properties(prefix, heights, n->left);
    check_tree_properties(prefix, heights, n->right);
}

template<>
void check_tree_properties<avl_tree_node_t>(const std::string prefix,
                           std::vector<std::size_t>& heights,
                           const avl_tree_node_t* n)
{
    if(nullptr == n)
    {
        return;
    }

    if(n->is_leaf())
    {
        std::size_t h = node_height(n);
        heights.push_back(h);
        std::cout << prefix << n->key << " : " << h << std::endl;
    }

    if((balance_factor(n) > 1) || (balance_factor(n) < -1))
    {
        std::cout << prefix << "Balance factor error! n: " << n->val << " BF: " << +balance_factor(n) << std::endl;
        exit(1);
    }

    check_tree_properties(prefix, heights, n->left);
    check_tree_properties(prefix, heights, n->right);
}

void print_node_symbol(bool have_left, bool have_right)
{
    if(!have_left && !have_right)
    {
        std::cout << TREE_DUMP_NODE_HAVE_NO;
    }
    else if(have_left && !have_right)
    {
        std::cout << TREE_DUMP_NODE_HAVE_LEFT;
    }
    else if(!have_left && have_right)
    {
        std::cout << TREE_DUMP_NODE_HAVE_RIGHT;
    }
    else
    {
        std::cout << TREE_DUMP_NODE_HAVE_BOTH;
    }
}

template<typename N>
void print_node(N n)
{
    print_node_symbol(n->have_left(), n->have_right());
    std::cout << " " << n->key << std::endl;
}

template<>
void print_node<rb_tree_node_t*>(rb_tree_node_t* n)
{
    print_node_symbol(n->have_left(), n->have_right());
    if(is_black(n))
    {
        std::cout << " B:";
    }
    else if(is_red(n))
    {
        std::cout << " R:";
    }
    std::cout << n->key << std::endl;
}

template<>
void print_node<avl_tree_node_t*>(avl_tree_node_t* n)
{
    print_node_symbol(n->have_left(), n->have_right());
    std::cout << " (" << +ecl::tree::balance_factor(n) << ":" << +ecl::tree::height(n) << ") ";
    std::cout << n->key << std::endl;
}

void print_indent(std::size_t indent)
{
    for(std::size_t i = 0; i < indent; ++i)
    {
        std::cout << TREE_DUMP_SPACE;
    }
}

template<typename N>
void print(N n, std::vector<std::size_t> connectors = std::vector<std::size_t>(), std::size_t indent = 0)
{
    if(n->have_right())
    {
        if(n->is_left())
        {
            connectors.push_back(indent);
        }
        print(n->right, connectors, indent + 1);
        if(n->is_left())
        {
            connectors.pop_back();
        }
    }

    std::size_t sum_con = 0;
    for(auto& c : connectors)
    {
        print_indent(c - sum_con - 1);
        std::cout << TREE_DUMP_CON;
        sum_con = c;
    }

    if(indent >= sum_con + 1)
    {
        print_indent(indent - sum_con - 1);
    }

    if(n->is_right())
    {
        std::cout << TREE_DUMP_RIGHT;
    }
    else if(n->is_left())
    {
        std::cout << TREE_DUMP_LEFT;
    }

    print_node(n);

    if(n->have_left())
    {
        if(n->is_right())
        {
            connectors.push_back(indent);
        }
        print(n->left, connectors, indent + 1);
        if(n->is_right())
        {
            connectors.pop_back();
        }
    }
}

template<typename T>
bool validate_heights(const std::string,
                      const std::vector<std::size_t>&)
{
    return true;
}

template<>
bool validate_heights<rb_tree_t>(const std::string,
                                 const std::vector<std::size_t>& heights)
{
    if(heights.empty())
    {
        return true;
    }

    auto result = std::minmax_element(heights.begin(), heights.end());
    return *result.first == *result.second;
}

template<typename T>
void dump_tree(const std::string prefix, const T& tree)
{
    std::cout << prefix << "printing tree." << std::endl;
    if(nullptr != tree.root())
    {
        print(tree.root());
    }
    std::cout << prefix << "done." << std::endl << std::endl;

    std::cout << prefix << "iterating over tree." << std::endl;

    std::cout << prefix << "iterator :         ";
    for(auto& v : tree)
    {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    auto it_end = tree.rend();

    std::cout << prefix << "reverse iterator : ";
    for(auto it = tree.rbegin(); it != it_end; ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << prefix << "done." << std::endl << std::endl;

    std::vector<std::size_t> heights;
    check_tree_properties(prefix, heights, tree.root());

    bool v_res = validate_heights<T>(prefix, heights);

    if(!v_res)
    {
        std::cout << "Validate heights failed!" << std::endl;
        exit(1);
    }
}

template<typename T>
void fill_tree_dynamic(const std::string prefix, T& tree, key_type from, key_type to, std::size_t count)
{
    std::size_t ins_count = 0;

    std::cout << prefix << "inserting dynamicaly created nodes."  << std::endl;
    std::cout << prefix << "nodes key from: " << from  << std::endl;
    std::cout << prefix << "nodes key to:   " << to    << std::endl;
    std::cout << prefix << "nodes count:    " << count << std::endl;

    std::default_random_engine e;
    std::uniform_int_distribution<int> uniform_dist(from, to);

    for(std::size_t i = 0; i < count; ++i)
    {
        int v = uniform_dist(e);
        std::stringstream ss;
        ss << v;
        typename T::node_t::pointer p = new typename T::node_t(v, ss.str());

        std::cout << prefix << "insert (" << v << "; " << ss.str() << ") : ";

        if(tree.insert(p) != p)
        {
            std::cout << "already exists in tree, deleting." << std::endl;
            delete p;
        }
        else
        {
            std::cout << "done." << std::endl;
            ++ins_count;
        }
        dump_tree(prefix, tree);
    }

    std::cout << prefix << "done. inserted: " << ins_count << std::endl << std::endl;
}

template<typename T, std::size_t N>
void fill_tree_static(const std::string prefix, T& tree, typename T::node_t (& nodes)[N])
{
    std::size_t count = 0;

    std::cout << prefix << "inserting staticaly allocated nodes from array." << std::endl;
    std::cout << prefix << "nodes count: " << N << std::endl;

    for(auto& n: nodes)
    {
        std::cout << prefix << "inserting node (" << n.key << "; " << n.val << ") : ";
        if(tree.insert(&n) != &n)
        {
            std::cout << "already exists in tree." << std::endl;
        }
        else
        {
            std::cout << "done." << std::endl;
            ++count;
        }
        dump_tree(prefix, tree);
    }

    std::cout << prefix << "done. inserted: " << count << std::endl << std::endl;
}

template<typename T>
void erase_tree_static(const std::string prefix, T& tree, key_type from, key_type to)
{
    std::size_t count = 0;

    std::cout << prefix << "erasing nodes."   << std::endl;
    std::cout << prefix << "nodes key from: " << from  << std::endl;
    std::cout << prefix << "nodes key to:   " << to    << std::endl;

    for(key_type i = from; i < to; ++i)
    {
        std::cout << prefix << "erasing node " << i << ": ";
        if(nullptr != tree.erase(i))
        {
            ++count;
            std::cout << "done." << std::endl;
        }
        else
        {
            std::cout << "no such node." << std::endl;
        }
        dump_tree(prefix, tree);
    }

    std::cout << prefix << "done. erased: " << count << std::endl;
}

template<typename T>
void erase_tree_dynamic(const std::string prefix, T& tree, key_type from, key_type to)
{
    std::size_t count = 0;

    std::cout << prefix << "erasing nodes."   << std::endl;
    std::cout << prefix << "nodes key from: " << from  << std::endl;
    std::cout << prefix << "nodes key to:   " << to    << std::endl;

    std::srand(std::time(0));
    while(!tree.empty())
    {
        key_type i = std::rand() % (to - from) + from;
        typename T::node_t::pointer p = tree.erase(i);
        if(nullptr != p)
        {
            std::cout << prefix << "erasing node " << i << ": ";
            ++count;
            delete p;
            std::cout << "done." << std::endl;
            dump_tree(prefix, tree);
        }
        else
        {
            // std::cout << "no such node." << std::endl;
        }
    }

    // for(key_type i = from; i < to; ++i)
    // {
    //     std::cout << prefix << "erasing node " << i << ": ";
    //     typename T::node_t::pointer p = tree.erase(i);
    //     if(nullptr != p)
    //     {
    //         ++count;
    //         delete p;
    //         std::cout << "done." << std::endl;
    //     }
    //     else
    //     {
    //         std::cout << "no such node." << std::endl;
    //     }
    //     dump_tree(prefix, tree);
    // }

    std::cout << prefix << "done. erased: " << count << std::endl;
}

template<typename T>
void find_in_tree(const std::string prefix, const T& tree, key_type from, key_type to)
{
    std::size_t count = 0;

    std::cout << prefix << "find nodes."      << std::endl;
    std::cout << prefix << "nodes key from: " << from << std::endl;
    std::cout << prefix << "nodes key to:   " << to   << std::endl;

    for(key_type i = from; i < to; ++i)
    {
        std::cout << prefix << "search for node " << i << ": ";
        if(tree.end() != tree.find(i))
        {
            ++count;
            std::cout << "found." << std::endl;
        }
        else
        {
            std::cout << "no such node." << std::endl;
        }
    }

    std::cout << prefix << "done. found: " << count << std::endl;
}

void find_in_tree(const std::string prefix, splay_tree_t& tree, key_type from, key_type to)
{
    std::size_t count = 0;

    std::cout << prefix << "find nodes."      << std::endl;
    std::cout << prefix << "nodes key from: " << from << std::endl;
    std::cout << prefix << "nodes key to:   " << to   << std::endl;

    for(key_type i = from; i < to; ++i)
    {
        std::cout << prefix << "search for node " << i << ": ";
        if(tree.end() != tree.find(i))
        {
            ++count;
            std::cout << "found." << std::endl;
        }
        else
        {
            std::cout << "no such node." << std::endl;
        }
        dump_tree(prefix, tree);
    }

    std::cout << prefix << "done. found: " << count << std::endl;
}

template<typename T, std::size_t N>
void test_tree(const std::string prefix, typename T::node_t (& nodes)[N], std::size_t dynamic_nodes_count)
{
    T static_tree;
    fill_tree_static(prefix, static_tree, nodes);
    dump_tree(prefix, static_tree);
    find_in_tree(prefix, static_tree, 0, static_cast<key_type>(N) + 1);
    erase_tree_static(prefix, static_tree, 0, static_cast<key_type>(N) + 1);

    T dynamic_tree;
    fill_tree_dynamic(prefix, dynamic_tree, 0, dynamic_nodes_count, dynamic_nodes_count);
    dump_tree(prefix, dynamic_tree);
    find_in_tree(prefix, dynamic_tree, 0, dynamic_nodes_count + 1);
    erase_tree_dynamic(prefix, dynamic_tree, 0, dynamic_nodes_count + 1);
}

int main(int argc, char* argv[], char**)
{
    if(argc > 2)
    {
        std::cout << "Usage: tree_$COMPILER [DYNAMIC_NODES_COUNT]" << std::endl;
        return 0;
    }

    std::size_t dynamic_nodes_count = NODES_COUNT;

    if(argc == 2)
    {
        dynamic_nodes_count = std::max(1, std::stoi(std::string(argv[1])));
    }

    test_tree< tree_t       > ( TREE_PREFIX       , simple_nodes , dynamic_nodes_count);
    test_tree< rb_tree_t    > ( RB_TREE_PREFIX    , rb_nodes     , dynamic_nodes_count);
    test_tree< avl_tree_t   > ( AVL_TREE_PREFIX   , avl_nodes    , dynamic_nodes_count);
    test_tree< splay_tree_t > ( SPLAY_TREE_PREFIX , splay_nodes  , dynamic_nodes_count);
}
