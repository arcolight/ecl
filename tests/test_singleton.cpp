#include <iostream>

#include <ecl/singleton.hpp>

namespace
{

class test_class_1
{
public:
    virtual ~test_class_1() {}

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
    virtual ~test_class_2() {}

    void hello()
    {
        std::cout << "test_class_2 at " << this << std::endl;
    }
protected:
    test_class_2() {}
};

class test_class_ns
{
public:
    virtual ~test_class_ns() {}

    void hello()                                                           const
    {
        std::cout << "test_class_ns at " << this << std::endl;
    }

protected:
    test_class_ns() {}
};

}

using test_class_1_singleton  = ecl::singleton<test_class_1>;
using test_class_2_singleton  = ecl::singleton<test_class_2>;
using test_class_ns_singleton = ecl::singleton<test_class_ns>;

int main(int argc, char** argv)
{
    (void)(argc);
    (void)(argv);

    test_class_1_singleton::instance().hello();
    test_class_1_singleton::instance().hello();
    test_class_1_singleton::instance().hello();

    test_class_2_singleton::instance().hello();
    test_class_2_singleton::instance().hello();
    test_class_2_singleton::instance().hello();

    test_class_ns_singleton::instance().hello();
    test_class_ns_singleton::instance().hello();
    test_class_ns_singleton::instance().hello();
}
