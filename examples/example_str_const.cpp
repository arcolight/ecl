#include <ecl/str_const.hpp>

#include <iostream>

using namespace ecl;

int main(int, char**, char**)
{
    constexpr str_const test("test!");
    constexpr str_const test2(test);

    std::cout << test << std::endl;

    std::cout << "Out with range-based \"for\": ";
    for(auto& c: test)
    {
        std::cout << c;
    }
    std::cout << std::endl;

    static_assert(test.size() == 5, "Static assert failed!");
}
