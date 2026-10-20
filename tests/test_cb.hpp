#ifndef ECL_TEST_CB_HPP
#define ECL_TEST_CB_HPP

#include <ecl/circular_buffer.hpp>

#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <vector>

BOOST_AUTO_TEST_SUITE( circular_buffer_suite )

constexpr static std::size_t buf_size = 16;

using circular_buffer_t = ecl::circular_buffer<std::uint8_t, buf_size, true>;
using value_type_t = circular_buffer_t::value_type;

struct circular_buffer_fixture
{
    circular_buffer_fixture()
        : buf()
    {
        BOOST_TEST_MESSAGE( "setup circular_buffer_fixture" );

        std::size_t count = buf_size;
        for(value_type_t i = 0; i < count; ++i)
        {
            value_type_t value = i * 2;
            buf.push(value);
        }
    }

    ~circular_buffer_fixture()
    {
        BOOST_TEST_MESSAGE( "teardown circular_buffer_fixture" );
    }

    circular_buffer_t buf;
};

BOOST_FIXTURE_TEST_CASE( empty_case, circular_buffer_fixture )
{
    buf.clear();
    BOOST_CHECK(buf.is_empty());
}

void circular_range_check(const circular_buffer_t&         b,
                          const std::vector<value_type_t>& v)
{
    for(std::size_t i = 0; i < b.capacity(); ++i)
    {
        BOOST_CHECK_MESSAGE(b[i] == v[i], +b[i] << " = " << +v[i]);
    }
}

BOOST_FIXTURE_TEST_CASE( push_case, circular_buffer_fixture  )
{
    std::vector<value_type_t> expected(buf_size);
    std::iota(expected.begin(), expected.end(), 0);
    std::transform(expected.begin(), expected.end(), expected.begin(), [] (value_type_t v) { return v * 2; } );
    circular_range_check(buf, expected);

    for(value_type_t i = buf_size; i < buf_size * 2; ++i)
    {
        value_type_t value = i * 2;
        buf.push(value);
    }

    std::vector<value_type_t> expected2(buf_size);
    std::iota(expected2.begin(), expected2.end(), buf_size);
    std::transform(expected2.begin(), expected2.end(), expected2.begin(), [] (value_type_t v) { return v * 2; } );

    circular_range_check(buf, expected2);
}

BOOST_FIXTURE_TEST_CASE( pop_case, circular_buffer_fixture )
{
    for(value_type_t i = 0; i < buf_size; ++i)
    {
        value_type_t value = buf.pop();
        BOOST_CHECK_MESSAGE(value == i * 2, +value << " = " << i * 2);
    }

    buf.clear();
    BOOST_CHECK_MESSAGE(buf.is_empty(), "buffer not empty!");
}

BOOST_FIXTURE_TEST_CASE( iteration_case, circular_buffer_fixture )
{
    for(size_t i = 0; i < buf_size; ++i)
    {
        BOOST_CHECK_MESSAGE(buf[i] == i * 2, +buf[i] << " = " << i * 2);
    }

    value_type_t j = 0;
    for(auto& c : buf)
    {
        BOOST_CHECK_MESSAGE(c == j * 2, +c << " = " << j * 2);
        ++j;
    }
}

BOOST_FIXTURE_TEST_CASE( size_case, circular_buffer_fixture )
{
    BOOST_CHECK_EQUAL(buf.size(), buf_size);
    BOOST_CHECK_EQUAL(circular_buffer_t::capacity(), buf_size);
    BOOST_CHECK(!buf.is_empty());

    buf.pop();
    BOOST_CHECK_EQUAL(buf.size(), buf_size - 1);

    buf.push(0);
    buf.push(0);
    BOOST_CHECK_EQUAL(buf.size(), buf_size);
}

BOOST_FIXTURE_TEST_CASE( front_back_case, circular_buffer_fixture )
{
    BOOST_CHECK_EQUAL(+buf.front(), 0);
    BOOST_CHECK_EQUAL(+buf.back(), (buf_size - 1) * 2);

    buf.push(100);
    BOOST_CHECK_EQUAL(+buf.front(), 2);
    BOOST_CHECK_EQUAL(+buf.back(), 100);
}

BOOST_FIXTURE_TEST_CASE( drop_front_case, circular_buffer_fixture )
{
    buf.drop_front(0);
    BOOST_CHECK_EQUAL(buf.size(), buf_size);

    buf.drop_front(4);
    BOOST_CHECK_EQUAL(buf.size(), buf_size - 4);

    for(std::size_t i = 0; i < buf.size(); ++i)
    {
        BOOST_CHECK_EQUAL(+buf[i], (i + 4) * 2);
    }

    buf.drop_front(buf_size);
    BOOST_CHECK(buf.is_empty());
}

BOOST_FIXTURE_TEST_CASE( drop_back_case, circular_buffer_fixture )
{
    buf.drop_back(0);
    BOOST_CHECK_EQUAL(buf.size(), buf_size);

    buf.drop_back(4);
    BOOST_CHECK_EQUAL(buf.size(), buf_size - 4);
    BOOST_CHECK_EQUAL(+buf.back(), (buf_size - 5) * 2);

    for(std::size_t i = 0; i < buf.size(); ++i)
    {
        BOOST_CHECK_EQUAL(+buf[i], i * 2);
    }

    buf.push(200);
    BOOST_CHECK_EQUAL(buf.size(), buf_size - 3);
    BOOST_CHECK_EQUAL(+buf.back(), 200);

    buf.drop_back(buf_size * 2);
    BOOST_CHECK(buf.is_empty());
}

BOOST_FIXTURE_TEST_CASE( drop_wrapped_case, circular_buffer_fixture )
{
    for(value_type_t i = 0; i < buf_size / 2; ++i)
    {
        buf.push(i + 100);
    }

    buf.drop_front(buf_size / 2 + 2);
    buf.drop_back(2);

    std::vector<value_type_t> expected { 102, 103, 104, 105 };
    BOOST_REQUIRE_EQUAL(buf.size(), expected.size());
    for(std::size_t i = 0; i < expected.size(); ++i)
    {
        BOOST_CHECK_EQUAL(+buf[i], +expected[i]);
    }
}

BOOST_FIXTURE_TEST_CASE( reverse_iteration_case, circular_buffer_fixture )
{
    std::vector<value_type_t> reversed(buf.rbegin(), buf.rend());
    BOOST_REQUIRE_EQUAL(reversed.size(), buf_size);

    for(std::size_t i = 0; i < buf_size; ++i)
    {
        BOOST_CHECK_EQUAL(+reversed[i], (buf_size - i - 1) * 2);
    }
}

BOOST_FIXTURE_TEST_CASE( partial_iteration_case, circular_buffer_fixture )
{
    buf.drop_front(10);

    std::size_t n = 0;
    for(auto c : buf)
    {
        BOOST_CHECK_EQUAL(+c, (n + 10) * 2);
        ++n;
    }
    BOOST_CHECK_EQUAL(n, buf.size());

    const circular_buffer_t& cbuf = buf;
    BOOST_CHECK_EQUAL(std::distance(cbuf.begin(), cbuf.end()), 6);
}

BOOST_AUTO_TEST_CASE( pop_empty_case )
{
    circular_buffer_t b;
    BOOST_CHECK(b.is_empty());
    BOOST_CHECK_EQUAL(+b.pop(), 0);
    BOOST_CHECK(b.is_empty());

    b.push(7);
    BOOST_CHECK_EQUAL(+b.pop(), 7);
    BOOST_CHECK_EQUAL(+b.pop(), 0);
    BOOST_CHECK_EQUAL(b.size(), 0u);
}

BOOST_AUTO_TEST_CASE( erase_elements_case )
{
    ecl::circular_buffer<std::uint8_t, 4, true>  erasing;
    ecl::circular_buffer<std::uint8_t, 4, false> keeping;

    for(std::uint8_t i = 1; i <= 4; ++i)
    {
        erasing.push(i);
        keeping.push(i);
    }

    erasing.clear();
    keeping.clear();

    BOOST_CHECK(erasing.is_empty());
    BOOST_CHECK(keeping.is_empty());

    for(std::size_t i = 0; i < 4; ++i)
    {
        BOOST_CHECK_EQUAL(+erasing[i], 0);
        BOOST_CHECK_EQUAL(+keeping[i], i + 1);
    }
}

BOOST_AUTO_TEST_SUITE_END()

#endif // ECL_TEST_CB_HPP
