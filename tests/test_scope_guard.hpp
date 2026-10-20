#ifndef ECL_TEST_SCOPE_GUARD_HPP
#define ECL_TEST_SCOPE_GUARD_HPP

#include <ecl/scope_guard.hpp>

#include <boost/test/unit_test.hpp>

#include <string>

BOOST_AUTO_TEST_SUITE( scope_guard_suite )

BOOST_AUTO_TEST_CASE( base_case )
{
    int guard_counter = 0;

    {
        ecl::scope_guard g1([&] () { guard_counter += 1; } );
        BOOST_CHECK_EQUAL(guard_counter, 0);
        {
            BOOST_CHECK_EQUAL(guard_counter, 0);
            ecl::scope_guard g2([&] () { guard_counter += 10; } );
            BOOST_CHECK_EQUAL(guard_counter, 0);
        }
        BOOST_CHECK_EQUAL(guard_counter, 10);
    }
    BOOST_CHECK_EQUAL(guard_counter, 11);
}

BOOST_AUTO_TEST_CASE( reverse_order_case )
{
    std::string order;

    {
        ecl::scope_guard g1([&] () { order += "1"; } );
        ecl::scope_guard g2([&] () { order += "2"; } );
        ecl::scope_guard g3([&] () { order += "3"; } );
    }

    BOOST_CHECK_EQUAL(order, "321");
}

static void early_return(int& counter, bool leave)
{
    ecl::scope_guard g([&] () { ++counter; } );

    if(leave)
    {
        return;
    }

    counter += 100;
}

BOOST_AUTO_TEST_CASE( early_return_case )
{
    int counter = 0;

    early_return(counter, true);
    BOOST_CHECK_EQUAL(counter, 1);

    early_return(counter, false);
    BOOST_CHECK_EQUAL(counter, 102);
}

BOOST_AUTO_TEST_CASE( exception_case )
{
    int counter = 0;

    try
    {
        ecl::scope_guard g([&] () { ++counter; } );
        throw 42;
    }
    catch(int)
    {
        BOOST_CHECK_EQUAL(counter, 1);
    }

    BOOST_CHECK_EQUAL(counter, 1);
}

BOOST_AUTO_TEST_SUITE_END()

#endif // ECL_TEST_SCOPE_GUARD_HPP
