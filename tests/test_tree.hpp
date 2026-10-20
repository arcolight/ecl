#ifndef ECL_TEST_TREE_HPP
#define ECL_TEST_TREE_HPP

#include <ecl/tree/simple_binary_tree.hpp>
#include <ecl/tree/red_black_tree.hpp>
#include <ecl/tree/avl_tree.hpp>
#include <ecl/tree/splay_tree.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <numeric>
#include <random>
#include <vector>

BOOST_AUTO_TEST_SUITE( tree_suite )

using simple_tree_t = ecl::tree::simple_binary_tree < int, int >;
using rb_tree_t     = ecl::tree::red_black_tree     < int, int >;
using avl_tree_t    = ecl::tree::avl_tree           < int, int >;
using splay_tree_t  = ecl::tree::splay_tree         < int, int >;

using tree_types = boost::mpl::list
<
      simple_tree_t
    , rb_tree_t
    , avl_tree_t
    , splay_tree_t
>;

constexpr static std::size_t   TREE_NODES_COUNT { 64 };
constexpr static unsigned      TREE_SEED        { 12345 };

template<typename N>
std::size_t check_subtree(const N* n, std::vector<int>& keys)
{
    if(nullptr == n)
    {
        return 0;
    }

    if(n->have_left())
    {
        BOOST_CHECK_MESSAGE(n->left->parent == n, "broken parent link at " << n->left->key);
        BOOST_CHECK_MESSAGE(n->left->key < n->key, "BST order " << n->left->key << " < " << n->key);
    }

    std::size_t lh = check_subtree(n->left, keys);
    keys.push_back(n->key);

    if(n->have_right())
    {
        BOOST_CHECK_MESSAGE(n->right->parent == n, "broken parent link at " << n->right->key);
        BOOST_CHECK_MESSAGE(n->key < n->right->key, "BST order " << n->key << " < " << n->right->key);
    }

    std::size_t rh = check_subtree(n->right, keys);

    return std::max(lh, rh) + 1;
}

template<typename T>
void check_balance(const T&)
{
}

template<typename N>
std::size_t black_height(const N* n)
{
    if(nullptr == n)
    {
        return 1;
    }

    if(ecl::tree::is_red(n))
    {
        BOOST_CHECK_MESSAGE(ecl::tree::is_black(n->left) && ecl::tree::is_black(n->right),
                            "red node " << n->key << " has red child");
    }

    std::size_t lh = black_height(n->left);
    std::size_t rh = black_height(n->right);

    BOOST_CHECK_MESSAGE(lh == rh, "black height mismatch at " << n->key << ": " << lh << " != " << rh);

    return lh + (ecl::tree::is_black(n) ? 1 : 0);
}

template<>
void check_balance<rb_tree_t>(const rb_tree_t& t)
{
    if(t.empty())
    {
        return;
    }

    BOOST_CHECK_MESSAGE(ecl::tree::is_black(t.root()), "root must be black");
    black_height(t.root());
}

template<typename N>
int avl_height(const N* n)
{
    if(nullptr == n)
    {
        return 0;
    }

    int lh = avl_height(n->left);
    int rh = avl_height(n->right);

    BOOST_CHECK_MESSAGE(std::abs(rh - lh) <= 1, "AVL balance violated at " << n->key << ": " << lh << " / " << rh);
    BOOST_CHECK_MESSAGE(n->height == std::max(lh, rh) + 1, "stored height mismatch at " << n->key);

    return std::max(lh, rh) + 1;
}

template<>
void check_balance<avl_tree_t>(const avl_tree_t& t)
{
    avl_height(t.root());
}

template<typename T>
void check_tree(const T& t, std::vector<int> expected)
{
    std::sort(expected.begin(), expected.end());

    BOOST_REQUIRE_EQUAL(t.count(), expected.size());
    BOOST_CHECK_EQUAL(t.empty(), expected.empty());

    if(expected.empty())
    {
        BOOST_CHECK(nullptr == t.root());
        BOOST_CHECK(t.begin() == t.end());
        return;
    }

    BOOST_REQUIRE(nullptr != t.root());
    BOOST_CHECK(nullptr == t.root()->parent);

    std::vector<int> keys;
    check_subtree(t.root(), keys);
    BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(), expected.begin(), expected.end());

    BOOST_CHECK_EQUAL(t.min()->key, expected.front());
    BOOST_CHECK_EQUAL(t.max()->key, expected.back());

    std::vector<int> forward;
    for(auto& p : t)
    {
        forward.push_back(p.first);
        BOOST_CHECK_EQUAL(p.second, p.first * 10);
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(forward.begin(), forward.end(), expected.begin(), expected.end());

    std::vector<int> backward;
    for(auto it = t.rbegin(); it != t.rend(); ++it)
    {
        backward.push_back((*it).first);
    }
    std::reverse(backward.begin(), backward.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(backward.begin(), backward.end(), expected.begin(), expected.end());

    check_balance(t);
}

template<typename T>
struct tree_fixture
{
    using node_t = typename T::node_t;

    tree_fixture()
        : rng(TREE_SEED)
        , nodes()
        , tree()
    {
        std::vector<int> order(TREE_NODES_COUNT);
        std::iota(order.begin(), order.end(), 0);
        std::shuffle(order.begin(), order.end(), rng);

        nodes.reserve(TREE_NODES_COUNT);
        for(int k : order)
        {
            nodes.emplace_back(k * 2, k * 2 * 10);
        }
    }

    std::vector<int> keys() const
    {
        std::vector<int> result;
        for(auto& n : nodes)
        {
            result.push_back(n.key);
        }
        return result;
    }

    std::mt19937        rng;
    std::vector<node_t> nodes;
    T                   tree;
};

BOOST_AUTO_TEST_CASE_TEMPLATE( empty_case, T, tree_types )
{
    T t;

    check_tree(t, {});
    BOOST_CHECK(t.find(0) == t.end());
    BOOST_CHECK(nullptr == t.erase(0).first);
    BOOST_CHECK(nullptr == t.min());
    BOOST_CHECK(nullptr == t.max());
}

BOOST_AUTO_TEST_CASE_TEMPLATE( insert_case, T, tree_types )
{
    tree_fixture<T> f;
    std::vector<int> inserted;

    for(auto& n : f.nodes)
    {
        auto it = f.tree.insert(&n);
        BOOST_CHECK(it == &n);
        inserted.push_back(n.key);
        check_tree(f.tree, inserted);
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE( sorted_insert_case, T, tree_types )
{
    std::vector<typename T::node_t> nodes;
    nodes.reserve(TREE_NODES_COUNT);
    for(int k = 0; k < static_cast<int>(TREE_NODES_COUNT); ++k)
    {
        nodes.emplace_back(k, k * 10);
    }

    T t;
    std::vector<int> inserted;
    for(auto& n : nodes)
    {
        t.insert(&n);
        inserted.push_back(n.key);
    }

    check_tree(t, inserted);
}

BOOST_AUTO_TEST_CASE_TEMPLATE( find_case, T, tree_types )
{
    tree_fixture<T> f;
    for(auto& n : f.nodes)
    {
        f.tree.insert(&n);
    }

    for(int k = -1; k <= static_cast<int>(TREE_NODES_COUNT * 2); ++k)
    {
        auto it = f.tree.find(k);
        if((k >= 0) && (k % 2 == 0) && (k < static_cast<int>(TREE_NODES_COUNT * 2)))
        {
            BOOST_REQUIRE_MESSAGE(it != f.tree.end(), "key " << k << " not found");
            BOOST_CHECK_EQUAL(it->first,  k);
            BOOST_CHECK_EQUAL(it->second, k * 10);
        }
        else
        {
            BOOST_CHECK_MESSAGE(it == f.tree.end(), "absent key " << k << " found");
        }
    }

    check_tree(f.tree, f.keys());
}

BOOST_AUTO_TEST_CASE_TEMPLATE( duplicate_insert_case, T, tree_types )
{
    tree_fixture<T> f;
    for(auto& n : f.nodes)
    {
        f.tree.insert(&n);
    }

    std::vector<typename T::node_t> dups;
    dups.reserve(TREE_NODES_COUNT);
    for(auto& n : f.nodes)
    {
        dups.emplace_back(n.key, -1);
    }

    for(auto& d : dups)
    {
        auto it = f.tree.insert(&d, false);
        BOOST_CHECK(it != &d);
        BOOST_CHECK_EQUAL(it->first, d.key);
        BOOST_CHECK_MESSAGE(it->second == d.key * 10,
                            "key " << d.key << " updated with allow_update = false");
    }

    check_tree(f.tree, f.keys());

    for(auto& d : dups)
    {
        auto it = f.tree.insert(&d, true);
        BOOST_CHECK(it != &d);
        BOOST_CHECK_EQUAL(it->second, -1);
    }

    BOOST_CHECK_EQUAL(f.tree.count(), TREE_NODES_COUNT);
}

BOOST_AUTO_TEST_CASE_TEMPLATE( erase_case, T, tree_types )
{
    tree_fixture<T> f;
    for(auto& n : f.nodes)
    {
        f.tree.insert(&n);
    }

    std::vector<int> remaining = f.keys();
    std::vector<int> to_erase  = remaining;
    std::shuffle(to_erase.begin(), to_erase.end(), f.rng);

    for(int k : to_erase)
    {
        BOOST_CHECK(nullptr == f.tree.erase(k + 1).first);

        auto r = f.tree.erase(k);
        BOOST_REQUIRE_MESSAGE(nullptr != r.first, "key " << k << " was not erased");
        BOOST_CHECK_EQUAL(r.first->key, k);
        BOOST_CHECK(r.first->alone());

        remaining.erase(std::find(remaining.begin(), remaining.end(), k));
        check_tree(f.tree, remaining);

        BOOST_CHECK(f.tree.find(k) == f.tree.end());
        BOOST_CHECK(nullptr == f.tree.erase(k).first);
    }

    BOOST_CHECK(f.tree.empty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE( reuse_nodes_case, T, tree_types )
{
    tree_fixture<T> f;

    for(std::size_t round = 0; round < 3; ++round)
    {
        for(auto& n : f.nodes)
        {
            f.tree.insert(&n);
        }
        check_tree(f.tree, f.keys());

        for(auto& n : f.nodes)
        {
            f.tree.erase(n.key);
        }
        check_tree(f.tree, {});
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE( iterator_case, T, tree_types )
{
    tree_fixture<T> f;
    for(auto& n : f.nodes)
    {
        f.tree.insert(&n);
    }

    BOOST_CHECK_EQUAL(static_cast<std::size_t>(std::distance(f.tree.begin(), f.tree.end())),
                      TREE_NODES_COUNT);

    for(auto p : f.tree)
    {
        p.second += 1;
    }

    for(auto& n : f.nodes)
    {
        BOOST_CHECK_EQUAL(n.val, n.key * 10 + 1);
    }

    auto it = f.tree.end();
    --it;
    BOOST_CHECK_EQUAL(it->first, static_cast<int>((TREE_NODES_COUNT - 1) * 2));
}

BOOST_AUTO_TEST_SUITE_END()

#endif // ECL_TEST_TREE_HPP
