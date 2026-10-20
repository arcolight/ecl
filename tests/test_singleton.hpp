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

BOOST_AUTO_TEST_CASE( pointer_case )
{
    singleton_test_struct_1& ref = ecl::singleton<singleton_test_struct_1>::instance();
    singleton_test_struct_1* ptr = ecl::singleton<singleton_test_struct_1>::pointer();

    BOOST_CHECK_EQUAL(&ref, ptr);
    BOOST_CHECK(static_cast<const void*>(ptr) !=
                static_cast<const void*>(ecl::singleton<singleton_test_struct_2>::pointer()));
}

BOOST_AUTO_TEST_CASE( shared_state_case )
{
    ecl::singleton<singleton_test_struct_2>::instance().x = 17;
    ecl::singleton<singleton_test_struct_2>::pointer()->y = 42;

    BOOST_CHECK_EQUAL(ecl::singleton<singleton_test_struct_2>::instance().x, 17);
    BOOST_CHECK_EQUAL(ecl::singleton<singleton_test_struct_2>::instance().y, 42);
}

BOOST_AUTO_TEST_SUITE_END()

#endif // ECL_TEST_SINGLETON_HPP
