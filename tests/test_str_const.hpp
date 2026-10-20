#ifndef ECL_TEST_STR_CONST_HPP
#define ECL_TEST_STR_CONST_HPP

#include <ecl/str_const.hpp>

#include <boost/test/unit_test.hpp>

#include <string>
#include <sstream>
#include <cstring>

BOOST_AUTO_TEST_SUITE( str_const_suite )

#define STR_CONST_TEST_STRING        "test string!"
#define STR_CONST_TEST_STRING_LENGTH 12

BOOST_AUTO_TEST_CASE( ctor_case )
{
    constexpr ecl::str_const test(STR_CONST_TEST_STRING);

    BOOST_CHECK_MESSAGE(test.size() == STR_CONST_TEST_STRING_LENGTH, test.size() << " = " << STR_CONST_TEST_STRING_LENGTH);
}

BOOST_AUTO_TEST_CASE( copy_case )
{
    constexpr ecl::str_const test(STR_CONST_TEST_STRING);
    constexpr ecl::str_const test_copy(test);

    BOOST_CHECK_MESSAGE(test.size() == test_copy.size(), test_copy.size() << " = " << test.size());
}

BOOST_AUTO_TEST_CASE( zero_length_case )
{
    constexpr ecl::str_const test("");

    BOOST_CHECK_MESSAGE(test.size() == 0, test.size() << " = 0");
}

BOOST_AUTO_TEST_CASE( cast_to_ptr_case )
{
    constexpr ecl::str_const test(STR_CONST_TEST_STRING);

    const char* test_ptr = static_cast<const char*>(test);

    BOOST_CHECK_EQUAL(std::strcmp(test_ptr, STR_CONST_TEST_STRING), 0);
}

BOOST_AUTO_TEST_CASE( ptr_and_size_ctor_case )
{
    constexpr ecl::str_const test(STR_CONST_TEST_STRING, 4);

    BOOST_CHECK_EQUAL(test.size(), 4u);
    BOOST_CHECK_EQUAL(test[std::size_t { 3 }], 't');
    BOOST_CHECK_EQUAL(test[std::size_t { 4 }], 0);
    BOOST_CHECK_EQUAL(std::string(test.begin(), test.end()), "test");
}

BOOST_AUTO_TEST_CASE( compile_time_case )
{
    constexpr ecl::str_const test(STR_CONST_TEST_STRING);

    static_assert(test.size() == STR_CONST_TEST_STRING_LENGTH, "size() must be constexpr");
    static_assert(test[std::size_t { 0 }] == 't', "operator[] must be constexpr");
    static_assert(test[std::size_t { STR_CONST_TEST_STRING_LENGTH }] == 0, "out of bound access must be 0");

    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE( stream_case )
{
    constexpr ecl::str_const test(STR_CONST_TEST_STRING);

    std::stringstream ss;
    test << ss;

    BOOST_CHECK_EQUAL(ss.str(), STR_CONST_TEST_STRING);
}

BOOST_AUTO_TEST_CASE( index_access_case )
{
    constexpr ecl::str_const test(STR_CONST_TEST_STRING);

    for(std::size_t i = 0; i < STR_CONST_TEST_STRING_LENGTH; ++i)
    {
        char expected = std::string(STR_CONST_TEST_STRING)[i];
        BOOST_CHECK_MESSAGE(test[i] == expected, test[i] << " = " << expected);
    }
}

BOOST_AUTO_TEST_CASE( index_access_out_of_bound_case )
{
    constexpr ecl::str_const test(STR_CONST_TEST_STRING);

    for(std::size_t i = STR_CONST_TEST_STRING_LENGTH; i < STR_CONST_TEST_STRING_LENGTH * 2; ++i)
    {
        BOOST_CHECK_MESSAGE(test[i] == 0, test[i] << " = 0");
    }
}

BOOST_AUTO_TEST_CASE ( iteration_case )
{
    constexpr ecl::str_const test(STR_CONST_TEST_STRING);

    std::size_t i = 0;
    for(auto& c: test)
    {
        char expected = std::string(STR_CONST_TEST_STRING)[i];
        BOOST_CHECK_MESSAGE(c == expected, c << " = " << expected);
        ++i;
    }
}

BOOST_AUTO_TEST_SUITE_END()

#endif // ECL_TEST_STR_CONST_HPP
