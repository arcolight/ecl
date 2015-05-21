#include <iostream>

#include <string>
#include <limits>

#define ECL_WITH_STD_STRING

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
> document_ecl_t;

typedef object<
    node<name1, bool>,
    node<name2, int32_t>,
    node<level1,
        object<
            node<name1, bool>,
            node<name3, std::string>,
            node<level2, array<
                object<
                    node<ar_item1, bool>,
                    node<ar_item2, int32_t>,
                    node<ar_item3, std::string>
                >, 8>
            >
        >
    >,
    node<name5, bool>
> document_std_t;

template<typename T>
static void fill_document(T& doc)
{
    doc.template f<name1>() = true;
    doc.template f<name2>() = std::numeric_limits<int32_t>::max();
    doc.template f<level1>().template f<name1>() = false;
    doc.template f<level1>().template f<name3>() = "name3 node";

    int32_t ctr = -3;

    for(auto& i: doc.template f<level1>().template f<level2>())
    {
        i.template f<ar_item1>() = true;
        i.template f<ar_item2>() = ctr;
        i.template f<ar_item3>() = "item3";
        ctr++;
    }

    doc.template f<name5>() = true;
}

template<typename T>
static void dump_document(T& doc, std::string& prefix)
{
    std::cout << prefix << "doc<name1>: " << (doc.template f<name1>() ? "true" : "false") << std::endl;
    std::cout << prefix << "doc<name2>: " << doc.template f<name2>() << std::endl;
    std::cout << prefix << "doc<level1><name1>: " << (doc.template f<level1>().template f<name1>() ? "true" : "false") << std::endl;
    std::cout << prefix << "doc<level1><name3>: " << doc.template f<level1>().template f<name3>() << std::endl;

    for(auto& i: doc.template f<level1>().template f<level2>())
    {
        std::cout << prefix << "doc<level1><level2><ar_item1>: " << (i.template f<ar_item1>() ? "true" : "false") << std::endl;
        std::cout << prefix << "doc<level1><level2><ar_item2>: " << i.template f<ar_item2>() << std::endl;
        std::cout << prefix << "doc<level1><level2><ar_item3>: " << i.template f<ar_item3>() << std::endl;
    }

    std::cout << prefix << "doc<name5>: " << (doc.template f<name5>() ? "true" : "false") << std::endl;
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    document_ecl_t doc_ecl;
    document_ecl_t doc_ecl_2;
    document_std_t doc_std;
    document_std_t doc_std_2;

    std::string ecl_prefix = "[ECL] ";
    std::string std_prefix = "[STD] ";

    std::cout << ecl_prefix << "document size: " << document_ecl_t::size() << std::endl;
    std::cout << std_prefix << "document size: " << document_std_t::size() << std::endl;

    ecl::stream<document_ecl_t::size()> st_ecl;
    ecl::stream<document_std_t::size()> st_std;

    fill_document(doc_ecl);
    dump_document(doc_ecl, ecl_prefix);

    fill_document(doc_std);
    dump_document(doc_std, std_prefix);

    doc_ecl.serialize(st_ecl);
    doc_std.serialize(st_std);

    std::cout << ecl_prefix << "Serialized count: " << st_ecl.count() << std::endl;
    std::cout << st_ecl << std::endl;

    std::cout << std_prefix << "Serialized count: " << st_std.count() << std::endl;
    std::cout << st_std << std::endl;

    bool deser_result = doc_ecl_2.deserialize(st_ecl);
    std::cout << ecl_prefix << "deserialization result: " << (deser_result ? "true" : "false") << std::endl;
    dump_document(doc_ecl_2, ecl_prefix);

    deser_result = doc_std_2.deserialize(st_std);
    std::cout << std_prefix << "deserialization result: " << (deser_result ? "true" : "false") << std::endl;
    dump_document(doc_std_2, std_prefix);

    return 0;
}
