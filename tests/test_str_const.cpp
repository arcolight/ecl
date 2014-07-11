#include <ecl/str_const.hpp>

#include <iostream>

using namespace ecl;

int main(int argc, char* argv[])
{
    (void)(argc);
    (void)(argv);

    constexpr str_const test("test!");

    std::cout << test << std::endl;

    static_assert(test.size() == 5, "Static assert failed!");
}