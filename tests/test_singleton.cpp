#include <iostream>

#include <ecl/singleton.hpp>

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

    void hello()                                                       const
    {
        std::cout << "test_class_ns at " << this << std::endl;
    }

protected:
    test_class_ns() {}
};

typedef ecl::singleton<test_class_ns> test_class_ns_singleton;

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
