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

const auto m = ecl::create_map<key_type, value_type>
(
    "not found"
    , std::make_pair(E::e1, "e1")
    , std::make_pair(E::e2, "e2")
    , std::make_pair(E::e3, "e3")
);

int main(int, char**, char**)
{
    std::cout << "map elements count: " << m.size() << std::endl;

    std::cout << "range-for interation over map [second elements in pair]: ";
    for(auto& p : m)
    {
        std::cout << p << " ";
    }
    std::cout << std::endl;

    value_type v = nullptr;

    v = m[E::e1];
    std::cout << "map[E::e1]: " << v << std::endl;

    v = m[E::e2];
    std::cout << "map[E::e2]: " << v << std::endl;

    v = m[E::e3];
    std::cout << "map[E::e3]: " << v << std::endl;

    v = m[E::e4];
    std::cout << "map[E::e4]: " << v << std::endl;
}
