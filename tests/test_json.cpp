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
            node<name3, string<12>>,
            node<level2, array<
                object<
                    node<ar_item1, bool>,
                    node<ar_item2, int32_t>,
                    node<ar_item3, string<32>>
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
    document_t doc_2;

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
//    st << ecl::reset();

    const char* ser_ptr = st;
    bool deser_result = doc_2.deserialize(ser_ptr);
    std::cout << "deserialization result: " << (deser_result ? "true" : "false") << std::endl;

    std::cout << "doc_2<name1>: " << (doc_2.f<name1>() ? "true" : "false") << std::endl;
    std::cout << "doc_2<level1><name1>: " << (doc_2.f<level1>().f<name1>() ? "true" : "false") << std::endl;

    for(auto& i: doc_2.f<level1>().f<level2>())
    {
        std::cout << "doc_2<level1><level2><ar_item1>: " << (i.f<ar_item1>() ? "true" : "false") << std::endl;
        std::cout << "doc_2<level1><level2><ar_item2>: " << i.f<ar_item2>() << std::endl;
        std::cout << "doc_2<level1><level2><ar_item3>: " << i.f<ar_item3>() << std::endl;
    }

    std::cout << "doc_2 size: " << document_t::size() << std::endl;

    ecl::stream<document_t::size()> st2;
    doc_2.serialize(st2);

    std::cout << "Serialized count: " << st2.count() << std::endl;
    std::cout << st2 << std::endl;
    st2 << ecl::reset();

    return 0;
}
