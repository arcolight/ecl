#include <iostream>

#include <ecl/json.hpp>
#include <ecl/stream.hpp>
#include <ecl/name_type.hpp>

ECL_DECL_NAME_TYPE(name1)
ECL_DECL_NAME_TYPE(name2)
ECL_DECL_NAME_TYPE(name3)
ECL_DECL_NAME_TYPE(name4)
ECL_DECL_NAME_TYPE(name5)
ECL_DECL_NAME_TYPE(level1)
ECL_DECL_NAME_TYPE(level2)
ECL_DECL_NAME_TYPE(level3)
ECL_DECL_NAME_TYPE(ar_item1)
ECL_DECL_NAME_TYPE(ar_item2)
ECL_DECL_NAME_TYPE(ar_item3)

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

    return 0;
}
