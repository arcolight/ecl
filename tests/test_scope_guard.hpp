#ifndef ECL_TEST_SCOPE_GUARD_HPP
#define ECL_TEST_SCOPE_GUARD_HPP

#include <ecl/scope_guard.hpp>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( scope_guard_suite )

static int guard_counter = 0;

BOOST_AUTO_TEST_CASE( base_case )
{
    BOOST_CHECK_MESSAGE(guard_counter == 0, "counter error! counter = " << guard_counter << " instead of 0");
    {
        ecl::scope_guard g1([&] () { guard_counter += 1; } );
        BOOST_CHECK_MESSAGE(guard_counter == 0, "counter error! counter = " << guard_counter << " instead of 0");
        {
            BOOST_CHECK_MESSAGE(guard_counter == 0, "counter error! counter = " << guard_counter << " instead of 0");
            ecl::scope_guard g2([&] () { guard_counter += 10; } );
            BOOST_CHECK_MESSAGE(guard_counter == 0, "counter error! counter = " << guard_counter << " instead of 0");
        }
        BOOST_CHECK_MESSAGE(guard_counter == 10, "counter error! counter = " << guard_counter << " instead of 10");
    }
    BOOST_CHECK_MESSAGE(guard_counter == 11, "counter error! counter = " << guard_counter << " instead of 11");
}

BOOST_AUTO_TEST_SUITE_END()

#endif // SCOPE_GUARD
