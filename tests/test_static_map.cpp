#include <iostream>
#include <cstdlib>
#include <string>

#include <ecl/static_map.hpp>

enum class E
{
      e1
    , e2
    , e3
    , e4
};

using key_type   = E;
using value_type = const char*;

auto m = ecl::create_map<key_type, value_type>
(
    "not found"
    , std::make_pair(E::e1, "e1")
    , std::make_pair(E::e2, "e2")
    , std::make_pair(E::e3, "_e3")
    , std::make_pair(E::e3, "e3") // overwrite E::e3 element
);

int main(int, char**, char**)
{
    std::cout << "map elements count: " << m.size() << std::endl;

    std::cout << "range-for interation over map: ";
    for(auto& p : m)
    {
        std::cout << p << " ";
    }
    std::cout << std::endl;

    std::cout << "map[E::e1]: " << m[E::e1] << std::endl;
    std::cout << "map[E::e2]: " << m[E::e2] << std::endl;
    std::cout << "map[E::e3]: " << m[E::e3] << std::endl;
    std::cout << "map[E::e4]: " << m[E::e4] << std::endl;
}
