#include <iostream>

#include <ecl/json.hpp>

struct name1    { constexpr static const char* name() { return "name1"; } };
struct name2    { constexpr static const char* name() { return "name2"; } };
struct name3    { constexpr static const char* name() { return "name3"; } };
struct name4    { constexpr static const char* name() { return "name4"; } };
struct name5    { constexpr static const char* name() { return "name5"; } };
struct level1   { constexpr static const char* name() { return "level1"; } };
struct level2   { constexpr static const char* name() { return "level2"; } };
struct level3   { constexpr static const char* name() { return "level3"; } };
struct ar_item1 { constexpr static const char* name() { return "ar_item1"; } };
struct ar_item2 { constexpr static const char* name() { return "ar_item2"; } };
struct ar_item3 { constexpr static const char* name() { return "ar_item3"; } };

using namespace ecl;
using namespace json;

typedef object<
    node<name1, bool>,
    node<name2, int32_t>,
    node<level1,
        object<
            node<name1, bool>,
            node<name3, const char*>,
            node<level2, array<
                object<
                    node<ar_item1, bool>,
                    node<ar_item2, int32_t>,
                    node<ar_item3, const char*>
                >, 8>
            >
        >
    >,
    node<name5, bool>
> document_t;

static uint8_t buffer[2048];

struct serial_out
{
    static bool print(const char* const string) {
        puts((const char* const)string);
        return true;
    }
};

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    for(auto& c : buffer)
    {
        c = 0x00;
    }

    document_t doc;

    bool  val1 = doc.f<name1>();
    bool& val2 = doc.f<name1>();

    doc.f<level1>().f<name3>() = "name3 node";
    doc.f<name2>() = 0;

    doc.f<name1>() = true;
    std::cout << "doc<name1>: " << (doc.f<name1>() ? "true" : "false") << std::endl;

    doc.f<name1>() = false;
    std::cout << "doc<name1>: " << (doc.f<name1>() ? "true" : "false") << std::endl;

    doc.f<level1>().f<name1>() = true;
    std::cout << "doc<level1><name1>: " << (doc.f<level1>().f<name1>() ? "true" : "false") << std::endl;

    int32_t ctr = 0;

    for(auto& i: doc.f<level1>().f<level2>())
    {
        i.f<ar_item1>() = true;
        i.f<ar_item2>() = ctr;
        i.f<ar_item3>() = "item3";
        ctr++;
    }

    for(auto& i: doc.f<level1>().f<level2>())
    {
        std::cout << "doc<level1><level2><ar_item1>: " << (i.f<ar_item1>() ? "true" : "false") << std::endl;
        std::cout << "doc<level1><level2><ar_item2>: " << i.f<ar_item2>() << std::endl;
        std::cout << "doc<level1><level2><ar_item3>: " << i.f<ar_item3>() << std::endl;
    }

    ecl::logger<128, serial_out> log;

    bool result = doc.serialize(log);
    std::cout << "Serialize result: " << (result ? "true" : "false") << std::endl;
    log << ecl::end();

    // std::cout << "Serialized: " << (char*)buffer << std::endl;

    return 0;
}
