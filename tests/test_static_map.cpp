#include <iostream>
#include <cstdlib>

#include <ecl/static_map.hpp>

enum class E
{
    e1,
    e2,
    e3,
    e4
};

const auto m = ecl::create_map<const E, const char*>
(
    std::pair<const E, const char*>(E::e1, "e1"),
    std::pair<const E, const char*>(E::e2, "e2"),
    std::pair<const E, const char*>(E::e3, "e3")
);

int main(int, char**, char**)
{
    // std::cout << "map elements count: " << m.size() << std::endl;

    // std::cout << "range-for interation over map [second elements in pair]: ";
    // // for(auto& p : m)
    // // {
    // //     std::cout << p.second << " ";
    // // }
    // // std::cout << std::endl;

    // std::cout << "map[E::e1]: " << m[E::e1] << std::endl;
    // std::cout << "map[E::e2]: " << m[E::e2] << std::endl;
    // std::cout << "map[E::e3]: " << m[E::e3] << std::endl;
    // std::cout << "map[E::e4]: " << m[E::e4] << std::endl;
}
