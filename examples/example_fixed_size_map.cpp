#include <iostream>
#include <cstdlib>
#include <string>

#include <ecl/fixed_size_map.hpp>

enum class E
{
      e1
    , e2
    , e3
    , e4
};

using key_type   = E;
using value_type = std::uint32_t;

using map_t      = ecl::map<key_type, value_type, 16>;

auto m = ecl::create_map<key_type, value_type>
(
      std::make_pair(E::e1, 1u)
    , std::make_pair(E::e2, 2u)
    , std::make_pair(E::e3, 4u)
    , std::make_pair(E::e3, 3u) // overwrite E::e3 element
);

int main(int, char**, char**)
{
    map_t m1;

    // m1[E::e1] = "1";

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
