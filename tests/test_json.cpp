#include <iostream>

#include <ecl/json.hpp>
#include <ecl/stream.hpp>
#include <ecl/constexpr_md5.hpp>

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

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    document_t doc;

    bool  val1 = doc.f<name1>();
    bool& val2 = doc.f<name1>();
    val2 = true;

    std::cout << "val1: " << (val1 ? "true" : "false") << std::endl;
    std::cout << "val2: " << (val2 ? "true" : "false") << std::endl;

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

    doc.f<name5>() = false;

    std::cout << "doc size: " << document_t::size() << std::endl;

    ecl::stream<document_t::size()> st;

    doc.serialize(st);

    std::cout << "Serialized count: " << st.count() << std::endl;
    std::cout << st << std::endl;
    st << ecl::reset();

    // constexpr auto md5_sum = ecl::md5("TEST STRING");

    // std::cout << "MD5: ";
    // for(auto& c: md5_sum)
    // {
    //     std::cout << std::hex << c;
    // }
    // std::cout << std::endl;

    return 0;
}