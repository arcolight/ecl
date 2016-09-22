#include <ecl/scope_guard.hpp>

#include <iostream>

namespace
{
    int testVar      = 0;
    const char *ok   = "OK   ";
    const char *fail = "FAIL ";

    const char* test(int step)
    {
        if ((testVar++) == step)
        {
            return ok;
        }
        return fail;
    }
}

void f(bool);

class A
{
public:
    void f()
    {
        std::cout << test(0) << " A::f enter" << std::endl;

        ecl::scope_guard guard([&]()
        {
            std::cout << test(2) << " A::f guard exec" << std::endl;
        });

        std::cout << test(1) << " A::f generic exit" << std::endl;
    }
};

void f(bool condition)
{
    std::cout << test(0) << "  ::f enter" << std::endl;

    ecl::scope_guard guard([&]()
    {
        std::cout << test(2) << "  ::f guard exec" << std::endl;
    });

    if (condition)
    {
        std::cout << test(1) << "  ::f conditional exit" << std::endl;
        return;
    }

    std::cout << test(1) << "  ::f generic exit" << std::endl;
}

int main(int, char**, char**)
{
    testVar = 0;
    ::f(false);
    std::cout << test(3) << "  ::f exited" << std::endl << std::endl;

    testVar = 0;
    ::f(true);
    std::cout << test(3) << "  ::f exited" << std::endl << std::endl;

    testVar = 0;
    A a;
    a.f();
    std::cout << test(3) << " A::f exited" << std::endl << std::endl;

    return 0;
}
