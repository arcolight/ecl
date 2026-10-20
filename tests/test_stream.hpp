#ifndef ECL_TEST_STREAM_HPP
#define ECL_TEST_STREAM_HPP

#include <ecl/stream.hpp>

#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE( stream_suite )

constexpr static std::size_t STREAM_SIZE { 64 };

using stream_t = ecl::stream<STREAM_SIZE>;

struct stream_fixture
{
    stream_fixture()
        : flushed()
        , st([this] (const char* const buf, std::size_t size)
             {
                 flushed.append(buf, size);
                 ++flush_count;
             })
    {}

    std::string output()
    {
        st << ecl::end();
        return flushed;
    }

    std::string flushed;
    std::size_t flush_count { 0 };
    stream_t    st;
};

BOOST_FIXTURE_TEST_CASE( empty_case, stream_fixture )
{
    BOOST_CHECK_EQUAL(std::string(st.data()), "");
    BOOST_CHECK_EQUAL(output(), "");
    BOOST_CHECK_EQUAL(flush_count, 1u);
}

BOOST_FIXTURE_TEST_CASE( string_char_bool_case, stream_fixture )
{
    st << "abc" << ' ' << 'x' << true << false;

    BOOST_CHECK_EQUAL(std::string(st.data()), "abc xtruefalse");
    BOOST_CHECK_EQUAL(output(), "abc xtruefalse");
}

BOOST_FIXTURE_TEST_CASE( null_string_case, stream_fixture )
{
    const char* null_str = nullptr;
    st << "a" << null_str << "b";

    BOOST_CHECK_EQUAL(output(), "ab");
}

BOOST_FIXTURE_TEST_CASE( decimal_case, stream_fixture )
{
    st << static_cast<std::int8_t>(-5)    << " "
       << static_cast<std::uint8_t>(200)  << " "
       << static_cast<std::int16_t>(-300) << " "
       << static_cast<std::uint16_t>(0)   << " "
       << static_cast<std::int32_t>(123456789) << " "
       << static_cast<std::uint64_t>(18446744073709551615ull);

    BOOST_CHECK_EQUAL(output(), "-5 200 -300 0 123456789 18446744073709551615");
}

BOOST_FIXTURE_TEST_CASE( limits_case, stream_fixture )
{
    st << std::numeric_limits<std::int8_t>::min()  << " "
       << std::numeric_limits<std::int16_t>::min() << " "
       << std::numeric_limits<std::int32_t>::min() << " "
       << std::numeric_limits<std::int64_t>::min();

    BOOST_CHECK_EQUAL(output(), "-128 -32768 -2147483648 -9223372036854775808");
}

BOOST_FIXTURE_TEST_CASE( bases_case, stream_fixture )
{
    st << ecl::base::h << static_cast<std::uint32_t>(0xDEADBEEF) << " "
       << ecl::base::o << static_cast<std::uint32_t>(8)          << " "
       << ecl::base::b << static_cast<std::uint8_t>(5)           << " "
       << ecl::base::d << static_cast<std::uint32_t>(42);

    BOOST_CHECK_EQUAL(output(), "deadbeef 10 101 42");
}

BOOST_FIXTURE_TEST_CASE( upper_case_case, stream_fixture )
{
    st(ecl::base::h)(ecl::alpha_case::upper) << static_cast<std::uint16_t>(0xBEEF);

    BOOST_CHECK_EQUAL(output(), "BEEF");
}

BOOST_FIXTURE_TEST_CASE( negative_hex_case, stream_fixture )
{
    st << ecl::base::h << static_cast<std::int8_t>(-1) << " " << static_cast<std::int16_t>(-2);

    BOOST_CHECK_EQUAL(output(), "ff fffe");
}

BOOST_FIXTURE_TEST_CASE( base_reset_case, stream_fixture )
{
    st << ecl::base::h << static_cast<std::uint8_t>(255);
    st << ecl::end();

    st << static_cast<std::uint8_t>(255);

    BOOST_CHECK_EQUAL(output(), "ff255");
}

BOOST_FIXTURE_TEST_CASE( float_case, stream_fixture )
{
    st << ecl::width(3) << 1.5 << " " << -2.25 << " " << 0.0;

    BOOST_CHECK_EQUAL(output(), "1.500 -2.250 0.000");
}

BOOST_FIXTURE_TEST_CASE( float_leading_zero_fraction_case, stream_fixture )
{
    st << ecl::width(2) << 1.05f;

    BOOST_CHECK_EQUAL(output(), "1.05");
}

BOOST_FIXTURE_TEST_CASE( float_negative_fraction_case, stream_fixture )
{
    st << ecl::width(1) << -0.5;

    BOOST_CHECK_EQUAL(output(), "-0.5");
}

BOOST_FIXTURE_TEST_CASE( float_rounding_case, stream_fixture )
{
    st << ecl::width(2) << 0.999 << " " << 2.004f << " " << 9.996;

    BOOST_CHECK_EQUAL(output(), "1.00 2.00 10.00");
}

BOOST_FIXTURE_TEST_CASE( float_zero_width_case, stream_fixture )
{
    st << ecl::width(0) << 3.25 << " " << 3.75;

    BOOST_CHECK_EQUAL(output(), "3 4");
}

BOOST_FIXTURE_TEST_CASE( float_raw_hex_case, stream_fixture )
{
    st << ecl::base::h << ecl::width(3) << -1.0f << " " << 1.0 << " "
       << static_cast<std::uint8_t>(10);

    BOOST_CHECK_EQUAL(output(), "bf800000 3ff0000000000000 a");
}

BOOST_FIXTURE_TEST_CASE( float_raw_upper_case, stream_fixture )
{
    st(ecl::base::h)(ecl::alpha_case::upper) << 0.1f;

    BOOST_CHECK_EQUAL(output(), "3DCCCCCD");
}

BOOST_FIXTURE_TEST_CASE( float_raw_oct_bin_case, stream_fixture )
{
    st << ecl::base::o << 1.0f << " " << ecl::base::b << 2.0f;

    BOOST_CHECK_EQUAL(output(), "7740000000 1000000000000000000000000000000");
}

BOOST_FIXTURE_TEST_CASE( float_raw_long_double_case, stream_fixture )
{
    st << ecl::base::h << ecl::width(1) << 1.5L;

    if(sizeof(long double) > sizeof(std::uint64_t))
    {
        BOOST_CHECK_EQUAL(output(), "1.5");
    }
    else
    {
        BOOST_CHECK_EQUAL(output(), "3ff8000000000000");
    }
}

BOOST_FIXTURE_TEST_CASE( float_decimal_after_hex_case, stream_fixture )
{
    st << ecl::base::h << 1.0f << ecl::base::d << " " << ecl::width(1) << 1.0f;

    BOOST_CHECK_EQUAL(output(), "3f800000 1.0");
}

BOOST_FIXTURE_TEST_CASE( reset_case, stream_fixture )
{
    st << "garbage" << ecl::rst() << "ok";

    BOOST_CHECK_EQUAL(output(), "ok");
    BOOST_CHECK_EQUAL(flush_count, 1u);
}

BOOST_FIXTURE_TEST_CASE( overflow_case, stream_fixture )
{
    std::string expected;
    for(std::size_t i = 0; i < STREAM_SIZE * 3 + 5; ++i)
    {
        char c = static_cast<char>('a' + (i % 26));
        expected.push_back(c);
        st << c;
    }

    BOOST_CHECK_EQUAL(flush_count, 3u);
    BOOST_CHECK_EQUAL(output(), expected);
    BOOST_CHECK_EQUAL(flush_count, 4u);
}

BOOST_FIXTURE_TEST_CASE( long_string_overflow_case, stream_fixture )
{
    std::string long_str(STREAM_SIZE * 2 + 10, 'z');
    st << long_str.c_str();

    BOOST_CHECK_EQUAL(output(), long_str);
}

BOOST_FIXTURE_TEST_CASE( binary_case, stream_fixture )
{
    const std::uint8_t data[] { 'a', 0x00, 'b', 0xFF };
    st << data;

    BOOST_CHECK_EQUAL(output(), std::string("a\0b\xFF", 4));
}

BOOST_AUTO_TEST_CASE( no_flush_function_case )
{
    stream_t st;
    st << "abc";
    BOOST_CHECK_EQUAL(std::string(st.data()), "abc");

    st << ecl::end();
    BOOST_CHECK_EQUAL(std::string(st.data()), "");
}

struct streamable
{
    template<typename T>
    T& operator<<(T& st) const
    {
        st << "<streamable>";
        return st;
    }
};

BOOST_FIXTURE_TEST_CASE( custom_type_case, stream_fixture )
{
    st << streamable() << "!";

    BOOST_CHECK_EQUAL(output(), "<streamable>!");
}

BOOST_AUTO_TEST_SUITE_END()

#endif // ECL_TEST_STREAM_HPP
