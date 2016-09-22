#ifndef ECL_TEST_SINGLETON_HPP
#define ECL_TEST_SINGLETON_HPP

#include <ecl/singleton.hpp>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( singleton_suite )

struct singleton_test_struct_1
{
    const singleton_test_struct_1* get_pointer_to_this()
    {
        return this;
    }

    int x;
};

struct singleton_test_struct_2
{
    const singleton_test_struct_2* get_pointer_to_this()
    {
        return this;
    }

    int x;
    int y;
};

BOOST_AUTO_TEST_CASE( base_case )
{
    singleton_test_struct_1& ptr1_1 = ecl::singleton<singleton_test_struct_1>::instance();
    singleton_test_struct_1& ptr1_2 = ecl::singleton<singleton_test_struct_1>::instance();

    singleton_test_struct_2& ptr2_1 = ecl::singleton<singleton_test_struct_2>::instance();
    singleton_test_struct_2& ptr2_2 = ecl::singleton<singleton_test_struct_2>::instance();

    BOOST_CHECK( ptr1_1.get_pointer_to_this() == ptr1_2.get_pointer_to_this() );
    BOOST_CHECK( ptr2_1.get_pointer_to_this() == ptr2_2.get_pointer_to_this() );
}

BOOST_AUTO_TEST_SUITE_END()

#endif // ECL_TEST_SINGLETON_HPP
