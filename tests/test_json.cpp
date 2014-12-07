#include <ecl/json.hpp>

constexpr const char name1[] = "name1";
constexpr const char name2[] = "name2";
constexpr const char name3[] = "name3";
constexpr const char name4[] = "name4";
constexpr const char name5[] = "name5";
constexpr const char level1[] = "level1";
constexpr const char level2[] = "level2";
constexpr const char level3[] = "level3";
constexpr const char ar_item1[] = "ar_item1";
constexpr const char ar_item2[] = "ar_item2";
constexpr const char ar_item3[] = "ar_item3";

typedef ecl::json::object<
    ecl::json::node<name1, bool>,
    ecl::json::node<name2, int32_t>,
    ecl::json::node<level1,
        ecl::json::object<
          ecl::json::node<name3, const char*>,
          ecl::json::node<level2, std::array<
                ecl::json::object<
                    ecl::json::node<ar_item1, bool>,
                    ecl::json::node<ar_item2, int32_t>,
                    ecl::json::node<ar_item3, const char*>
                >, 8>
            >
        >
    >,
    ecl::json::node<name5, bool>
> document_t;

static uint8_t buffer[2048];

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    for(auto& c : buffer)
    {
        c = 0x00;
    }

    document_t doc;

    doc.serialize(buffer, 2048);

    // doc["name1"] = true;

    return 0;
}
