#include <iostream>

#include <ecl/static_map.hpp>

enum class E
{
    e1,
    e2,
    e3,
    e4
};

using map_t = static_map<E, const char*>;

// constexpr map_t::list_t l =
// ;

const map_t m
{
    { E::e1, "e1" },
    { E::e2, "e2" },
    { E::e3, "e3" }
};

int main(int argc, char* argv[], char** env)
{
    std::cout << m[E::e1] << std::endl;
    std::cout << m[E::e2] << std::endl;
    std::cout << m[E::e3] << std::endl;
    std::cout << m[E::e4] << std::endl;
}
