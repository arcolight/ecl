#ifndef ECL_TEST_CB_HPP
#define ECL_TEST_CB_HPP

#include <ecl/circular_buffer.hpp>

#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <numeric>
#include <algorithm>

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

BOOST_FIXTURE_TEST_CASE( drop_case, circular_buffer_fixture )
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

BOOST_AUTO_TEST_SUITE_END()

#endif // ECL_TEST_CB_HPP
