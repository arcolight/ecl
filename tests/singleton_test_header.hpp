#ifndef SINGLETON_HEADER
#define SINGLETON_HEADER

#include <iostream>

#include <ecl/singleton.hpp>

class test_class_ns
{
public:
    void hello()                                                       const
    {
        std::cout << "test_class_ns at " << this << std::endl;
    }

protected:
    test_class_ns() {};
};

typedef ecl::singleton<test_class_ns> test_class_ns_singleton;

#endif // SINGLETON_HEADER
