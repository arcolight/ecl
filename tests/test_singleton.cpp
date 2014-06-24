#include <iostream>

#include <ecl/singleton.hpp>

class test_class_1
{
public:
    void hello()
    {
        std::cout << "test_class_1 at " << this << std::endl;
    }
};

class test_class_2 : public ecl::Singleton<test_class_2>
{
public:
    void hello()
    {
        std::cout << "test_class_2 at " << this << std::endl;
    }
};

int main(int argc, char** argv)
{
    (void)(argc);
    (void)(argv);

    ecl::Singleton<test_class_1>::instance().hello();
    ecl::Singleton<test_class_1>::instance().hello();
    ecl::Singleton<test_class_1>::instance().hello();

    test_class_2::instance().hello();
    test_class_2::instance().hello();
    test_class_2::instance().hello();
}