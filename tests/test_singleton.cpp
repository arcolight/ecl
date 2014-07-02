#include <iostream>

#include <ecl/singleton.hpp>

#include "singleton_test_header.hpp"

class test_class_1
{
public:
    void hello()
    {
        std::cout << "test_class_1 at " << this << std::endl;
    }
protected:
    test_class_1() {}
};

class test_class_2
{
public:
    void hello()
    {
        std::cout << "test_class_2 at " << this << std::endl;
    }
protected:
    test_class_2() {}
};

int main(int argc, char** argv)
{
    (void)(argc);
    (void)(argv);

    // test_class_2::instance().hello();
    // test_class_2::instance().hello();
    // test_class_2::instance().hello();

    test_class_ns_singleton::instance().hello();
    test_class_ns_singleton::instance().hello();
    test_class_ns_singleton::instance().hello();

    // test_class_ns a;
    // a.hello();
}