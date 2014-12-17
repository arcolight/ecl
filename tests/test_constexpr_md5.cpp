#include <ecl/constexpr_md5.hpp>

constexpr auto md5_1 = ecl::md5("TEST STRING 1");

int main(int agrc, char* argv[])
{
    (void)argc;
    (void)argv;

    return 0;
}