#include <iostream>

#include <ecl/static_map.hpp>
#include <ecl/str_const.hpp>

enum class test_en
{
    e1,
    e2,
    e3,
    e4,
    e5,
    e6
};

typedef ecl::pair<const test_en, const int> pair_int_t;
typedef ecl::static_map
<
    pair_int_t,
    0,
    ecl::entry<pair_int_t, test_en::e1, 1>,
    ecl::entry<pair_int_t, test_en::e2, 2>,
    ecl::entry<pair_int_t, test_en::e3, 3>,
    ecl::entry<pair_int_t, test_en::e4, 4>,
    ecl::entry<pair_int_t, test_en::e5, 5>
> map_int_t;

// constexpr const char str_dummy[] = "";
// constexpr const char str1[] = "1";
// constexpr const char str2[] = "2";
// constexpr const char str3[] = "3";
// constexpr const char str4[] = "4";
// constexpr const char str5[] = "5";

typedef ecl::pair<const test_en, ecl::str_const&> pair_str_t;
typedef ecl::static_map
<
    pair_str_t,
    ""_const,
    ecl::entry<pair_str_t, test_en::e1, "1"_const>,
    ecl::entry<pair_str_t, test_en::e2, "2"_const>,
    ecl::entry<pair_str_t, test_en::e3, "3"_const>,
    ecl::entry<pair_str_t, test_en::e4, "4"_const>,
    ecl::entry<pair_str_t, test_en::e5, "5"_const>
> map_str_t;

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    static_assert((1 == map_int_t::get<test_en::e1>()), "Entry lookup fail!");
    static_assert((2 == map_int_t::get<test_en::e2>()), "Entry lookup fail!");
    static_assert((3 == map_int_t::get<test_en::e3>()), "Entry lookup fail!");
    static_assert((4 == map_int_t::get<test_en::e4>()), "Entry lookup fail!");
    static_assert((5 == map_int_t::get<test_en::e5>()), "Entry lookup fail!");
    static_assert((0 == map_int_t::get<test_en::e6>()), "Entry lookup fail!");

    static_assert(("1"_const == map_str_t::get<test_en::e1>()), "Entry lookup fail!");
    static_assert(("2"_const == map_str_t::get<test_en::e2>()), "Entry lookup fail!");
    static_assert(("3"_const == map_str_t::get<test_en::e3>()), "Entry lookup fail!");
    static_assert(("4"_const == map_str_t::get<test_en::e4>()), "Entry lookup fail!");
    static_assert(("5"_const == map_str_t::get<test_en::e5>()), "Entry lookup fail!");
    static_assert((""_const == map_str_t::get<test_en::e6>()), "Entry lookup fail!");

    std::cout << "Int map e1: " << map_int_t::get<test_en::e1>() << std::endl;
    std::cout << "Int map e2: " << map_int_t::get<test_en::e2>() << std::endl;
    std::cout << "Int map e3: " << map_int_t::get<test_en::e3>() << std::endl;
    std::cout << "Int map e4: " << map_int_t::get<test_en::e4>() << std::endl;
    std::cout << "Int map e5: " << map_int_t::get<test_en::e5>() << std::endl;
    std::cout << "Int map e6: " << map_int_t::get<test_en::e6>() << std::endl;

    std::cout << "Str map e1: " << map_str_t::get<test_en::e1>() << std::endl;
    std::cout << "Str map e2: " << map_str_t::get<test_en::e2>() << std::endl;
    std::cout << "Str map e3: " << map_str_t::get<test_en::e3>() << std::endl;
    std::cout << "Str map e4: " << map_str_t::get<test_en::e4>() << std::endl;
    std::cout << "Str map e5: " << map_str_t::get<test_en::e5>() << std::endl;
    std::cout << "Str map e6: " << map_str_t::get<test_en::e6>() << std::endl;

    return 0;
}