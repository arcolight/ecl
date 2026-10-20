#ifndef ECL_TEST_MAP_HPP
#define ECL_TEST_MAP_HPP

#include <ecl/map.hpp>

#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <utility>
#include <vector>

BOOST_AUTO_TEST_SUITE( map_suite )

constexpr static std::size_t MAP_SIZE { 8 };

using map_t = ecl::map<int, std::uint32_t, MAP_SIZE>;

template<typename M>
std::vector<int> map_keys(const M& m)
{
    std::vector<int> keys;
    for(auto& p : m)
    {
        keys.push_back(p.first);
    }
    return keys;
}

BOOST_AUTO_TEST_CASE( empty_case )
{
    map_t m;

    BOOST_CHECK(m.empty());
    BOOST_CHECK_EQUAL(m.size(), 0u);
    BOOST_CHECK_EQUAL(m.max_size(), MAP_SIZE);
    BOOST_CHECK(m.begin() == m.end());
    BOOST_CHECK(m.find(1) == m.end());
    BOOST_CHECK_EQUAL(m.count(1), 0u);
    BOOST_CHECK_EQUAL(m.erase(1), 0u);
}

BOOST_AUTO_TEST_CASE( insert_case )
{
    map_t m;

    auto r1 = m.insert({ 5, 50 });
    BOOST_CHECK(r1.second);
    BOOST_CHECK_EQUAL(r1.first->first,  5);
    BOOST_CHECK_EQUAL(r1.first->second, 50u);

    auto r2 = m.insert({ 5, 55 });
    BOOST_CHECK(!r2.second);
    BOOST_CHECK_EQUAL(r2.first->second, 50u);
    BOOST_CHECK_EQUAL(m.size(), 1u);

    m.insert({ { 3, 30 }, { 7, 70 }, { 1, 10 } });
    BOOST_CHECK_EQUAL(m.size(), 4u);

    const std::vector<int> expected { 1, 3, 5, 7 };
    const std::vector<int> keys = map_keys(m);
    BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE( initializer_list_case )
{
    map_t m { { 4, 40 }, { 2, 20 }, { 6, 60 } };

    BOOST_CHECK_EQUAL(m.size(), 3u);
    BOOST_CHECK_EQUAL(m.at(2), 20u);
    BOOST_CHECK_EQUAL(m.at(4), 40u);
    BOOST_CHECK_EQUAL(m.at(6), 60u);
}

BOOST_AUTO_TEST_CASE( capacity_case )
{
    map_t m;

    for(int i = 0; i < static_cast<int>(MAP_SIZE); ++i)
    {
        BOOST_CHECK(m.insert({ i, static_cast<std::uint32_t>(i) }).second);
    }

    BOOST_CHECK_EQUAL(m.size(), MAP_SIZE);

    auto r = m.insert({ 100, 100 });
    BOOST_CHECK(!r.second);
    BOOST_CHECK(r.first == m.end());
    BOOST_CHECK_EQUAL(m.size(), MAP_SIZE);
    BOOST_CHECK(m.find(100) == m.end());

    BOOST_CHECK_EQUAL(m.erase(3), 1u);
    BOOST_CHECK(m.insert({ 100, 100 }).second);
    BOOST_CHECK_EQUAL(m.size(), MAP_SIZE);
    BOOST_CHECK_EQUAL(m.at(100), 100u);
}

BOOST_AUTO_TEST_CASE( subscript_case )
{
    map_t m;

    m[3] = 30;
    m[1] = 10;
    m[2] = 20;

    BOOST_CHECK_EQUAL(m.size(), 3u);
    BOOST_CHECK_EQUAL(m[1], 10u);
    BOOST_CHECK_EQUAL(m[2], 20u);
    BOOST_CHECK_EQUAL(m[3], 30u);

    m[2] = 22;
    BOOST_CHECK_EQUAL(m.size(), 3u);
    BOOST_CHECK_EQUAL(m.at(2), 22u);

    BOOST_CHECK_EQUAL(m[4], 0u);
    BOOST_CHECK_EQUAL(m.size(), 4u);
}

BOOST_AUTO_TEST_CASE( at_not_found_case )
{
    map_t m { { 1, 10 } };

    BOOST_CHECK_EQUAL(m.at(2), 0u);
    m.at(2) = 5;
    BOOST_CHECK_EQUAL(m.count(2), 0u);
    BOOST_CHECK_EQUAL(m.at(2), 0u);
    BOOST_CHECK_EQUAL(m.size(), 1u);
}

BOOST_AUTO_TEST_CASE( erase_by_key_case )
{
    map_t m { { 1, 10 }, { 2, 20 }, { 3, 30 }, { 4, 40 } };

    BOOST_CHECK_EQUAL(m.erase(2), 1u);
    BOOST_CHECK_EQUAL(m.erase(2), 0u);
    BOOST_CHECK_EQUAL(m.erase(9), 0u);
    BOOST_CHECK_EQUAL(m.size(), 3u);

    const std::vector<int> expected { 1, 3, 4 };
    const std::vector<int> keys = map_keys(m);
    BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE( erase_by_iterator_case )
{
    map_t m { { 1, 10 }, { 2, 20 }, { 3, 30 }, { 4, 40 } };

    map_t::const_iterator it = m.find(2);
    it = m.erase(it);
    BOOST_REQUIRE(it != m.end());
    BOOST_CHECK_EQUAL(it->first, 3);
    BOOST_CHECK_EQUAL(m.size(), 3u);

    it = m.begin();
    while(it != m.end())
    {
        it = m.erase(it);
    }

    BOOST_CHECK(m.empty());
}

BOOST_AUTO_TEST_CASE( erase_range_case )
{
    map_t m { { 1, 10 }, { 2, 20 }, { 3, 30 }, { 4, 40 } };

    m.erase(m.begin(), m.end());
    BOOST_CHECK(m.empty());

    for(int i = 0; i < static_cast<int>(MAP_SIZE); ++i)
    {
        BOOST_CHECK(m.insert({ i, 0 }).second);
    }
}

BOOST_AUTO_TEST_CASE( clear_case )
{
    map_t m { { 1, 10 }, { 2, 20 }, { 3, 30 } };

    m.clear();
    BOOST_CHECK(m.empty());
    BOOST_CHECK(m.find(1) == m.end());

    for(int i = 0; i < static_cast<int>(MAP_SIZE); ++i)
    {
        BOOST_CHECK(m.insert({ i, 0 }).second);
    }
    BOOST_CHECK_EQUAL(m.size(), MAP_SIZE);
}

BOOST_AUTO_TEST_CASE( stress_case )
{
    map_t m;
    std::vector<std::pair<int, std::uint32_t>> model;

    std::uint32_t seed = 1;
    auto next = [&seed] () { seed = seed * 1103515245u + 12345u; return (seed >> 16) & 0x7FFF; };

    for(int step = 0; step < 2000; ++step)
    {
        int key = static_cast<int>(next() % 16);
        auto pos = std::find_if(model.begin(), model.end(),
                                [key] (const std::pair<int, std::uint32_t>& p) { return p.first == key; });

        if(next() % 2)
        {
            auto r = m.insert({ key, static_cast<std::uint32_t>(step) });
            bool expect_insert = (pos == model.end()) && (model.size() < MAP_SIZE);
            BOOST_REQUIRE_EQUAL(r.second, expect_insert);
            if(expect_insert)
            {
                model.push_back({ key, static_cast<std::uint32_t>(step) });
            }
        }
        else
        {
            std::size_t expected = (pos == model.end()) ? 0 : 1;
            BOOST_REQUIRE_EQUAL(m.erase(key), expected);
            if(pos != model.end())
            {
                model.erase(pos);
            }
        }

        BOOST_REQUIRE_EQUAL(m.size(), model.size());
    }

    std::sort(model.begin(), model.end());
    auto it = m.begin();
    for(auto& p : model)
    {
        BOOST_REQUIRE(it != m.end());
        BOOST_CHECK_EQUAL(it->first,  p.first);
        BOOST_CHECK_EQUAL(it->second, p.second);
        ++it;
    }
    BOOST_CHECK(it == m.end());
}

BOOST_AUTO_TEST_CASE( reverse_iteration_case )
{
    map_t m { { 1, 10 }, { 2, 20 }, { 3, 30 } };

    std::vector<int> keys;
    for(auto it = m.rbegin(); it != m.rend(); ++it)
    {
        keys.push_back((*it).first);
    }

    const std::vector<int> expected { 3, 2, 1 };
    BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE( create_map_case )
{
    const auto m = ecl::create_map<int, std::uint32_t>
    (
          std::make_pair(3, 30u)
        , std::make_pair(1, 10u)
        , std::make_pair(2, 20u)
    );

    BOOST_CHECK_EQUAL(m.size(), 3u);
    BOOST_CHECK_EQUAL(m.max_size(), 3u);
    BOOST_CHECK_EQUAL(m.at(1), 10u);
    BOOST_CHECK_EQUAL(m.at(2), 20u);
    BOOST_CHECK_EQUAL(m.at(3), 30u);
    BOOST_CHECK_EQUAL(m[3], 30u);

    const std::vector<int> expected { 1, 2, 3 };
    const std::vector<int> keys = map_keys(m);
    BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(), expected.begin(), expected.end());
}

struct c_str_less
{
    template<typename T>
    struct cmp
    {
        bool operator()(const T& a, const T& b) const
        {
            return std::strcmp(a, b) < 0;
        }
    };
};

BOOST_AUTO_TEST_CASE( custom_compare_case )
{
    ecl::map<const char*, int, 4, c_str_less::cmp> m;

    char k1[] = "key";
    char k2[] = "key";

    BOOST_CHECK(m.insert({ k1, 1 }).second);
    BOOST_CHECK(!m.insert({ k2, 2 }).second);
    BOOST_CHECK_EQUAL(m.size(), 1u);
    BOOST_CHECK_EQUAL(m.at("key"), 1);
}

BOOST_AUTO_TEST_SUITE_END()

#endif // ECL_TEST_MAP_HPP
