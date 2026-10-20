#ifndef ECL_TEST_JSON_HPP
#define ECL_TEST_JSON_HPP

#include <ecl/json.hpp>
#include <ecl/name_type.hpp>
#include <ecl/stream.hpp>

#include <boost/test/unit_test.hpp>

#include <string>
#include <sstream>

BOOST_AUTO_TEST_SUITE( json_suite )

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

using document_ecl_t = object
<
    node < name1  , bool    >,
    node < name2  , int32_t >,
    node < level1 , object
        <
            node < name1  , bool          >,
            node < name3  , string < 12 > >,
            node < level2 , array
                <
                    object
                    <
                          node < ar_item1 , bool          >
                        , node < ar_item2 , int32_t       >
                        , node < ar_item3 , string < 32 > >
                    >
                    , 8
                >
            >
        >
    >,
    node < name5 , bool >
>;

using document_std_t = object
<
    node < name1  , bool    >,
    node < name2  , int32_t >,
    node < level1 , object
        <
            node < name1  , bool >,
            node < name3  , std::string >,
            node < level2 , array
                <
                    object
                    <
                          node< ar_item1 , bool        >
                        , node< ar_item2 , int32_t     >
                        , node< ar_item3 , std::string >
                    >
                    , 8
                >
            >
        >
    >,
    node < name5 , bool >
>;

using floating_point_doc_t = object
<
    node < name1 , float       >,
    node < name2 , double      >,
    node < name3 , long double >
>;

struct json_fixture
{
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
            ++ctr;
        }

        doc.template f<name5>() = true;
    }

    json_fixture()
        : ecl_doc()
        , std_doc()
        , reference()
    {
        fill_document(ecl_doc);
        fill_document(std_doc);
        fill_document(reference);

        BOOST_TEST_MESSAGE( "setup json_fixture" );
    }

    ~json_fixture()
    {
        BOOST_TEST_MESSAGE( "teardown json_fixture" );
    }

    document_ecl_t ecl_doc;
    document_std_t std_doc;

    document_std_t reference;
};

#define ECL_BOOST_CHECK_DOC_FIELD_MESSAGE(field, value) \
    BOOST_CHECK_MESSAGE(field == value, std::boolalpha << field << " = " << value)

BOOST_AUTO_TEST_CASE( string_assign_and_compare_test )
{
    using string4_t  = ecl::json::string < 4  >;
    using string8_t  = ecl::json::string < 8  >;
    using string16_t = ecl::json::string < 16 >;

    string4_t  s4  = "abcd";
    string8_t  s8  = "abcdefgh";
    string16_t s16 = "abcdefghabcdefgh";

    BOOST_CHECK_MESSAGE( s4  == "abcd"             , s4  << " = abcd"             );
    BOOST_CHECK_MESSAGE( s8  == "abcdefgh"         , s8  << " = abcdefgh"         );
    BOOST_CHECK_MESSAGE( s16 == "abcdefghabcdefgh" , s16 << " = abcdefghabcdefgh" );

    s4  = "";
    s8  = "";
    s16 = "";

    BOOST_CHECK_MESSAGE( s4  == "" , s4  << " = \"\"" );
    BOOST_CHECK_MESSAGE( s8  == "" , s8  << " = \"\"" );
    BOOST_CHECK_MESSAGE( s16 == "" , s16 << " = \"\"" );

    s4  = "12345678901234567890";
    s8  = "12345678901234567890";
    s16 = "12345678901234567890";

    BOOST_CHECK_MESSAGE( s4  == "1234"             , s4  << " = 1234"             );
    BOOST_CHECK_MESSAGE( s8  == "12345678"         , s8  << " = 12345678"         );
    BOOST_CHECK_MESSAGE( s16 == "1234567890123456" , s16 << " = 1234567890123456" );

    BOOST_CHECK_MESSAGE( s4  != s8  , s4  << " != " << s8  );
    BOOST_CHECK_MESSAGE( s8  != s16 , s8  << " != " << s16 );
    BOOST_CHECK_MESSAGE( s16 != s4  , s16 << " != " << s4  );

    s4  = "12";
    s8  = "12";
    s16 = "12";

    BOOST_CHECK_MESSAGE( s4  == s8  , s4  << " = " << s8  );
    BOOST_CHECK_MESSAGE( s8  == s16 , s8  << " = " << s16 );
    BOOST_CHECK_MESSAGE( s16 == s4  , s16 << " = " << s4  );
}

BOOST_AUTO_TEST_CASE( string_serialization_test )
{
    using string4_t  = ecl::json::string < 4  >;
    using string16_t = ecl::json::string < 16 >;

    string4_t  s4  = "abcd";
    string16_t s16 = "a\"b\\c/d\n\te";

    std::stringstream ss4;
    s4.serialize(ss4, false, 0, 0);
    BOOST_CHECK_EQUAL(ss4.str(), R"("abcd")");

    std::stringstream ss16;
    s16.serialize(ss16, false, 0, 0);
    BOOST_CHECK_EQUAL(ss16.str(), R"("a\"b\\c\/d\n\te")");
}

BOOST_FIXTURE_TEST_CASE( document_assign_case, json_fixture )
{
    ECL_BOOST_CHECK_DOC_FIELD_MESSAGE(ecl_doc.f< name1 >() , true                                );
    ECL_BOOST_CHECK_DOC_FIELD_MESSAGE(ecl_doc.f< name2 >() , std::numeric_limits<int32_t>::max() );

    auto& l1 = ecl_doc.f< level1 >();
    ECL_BOOST_CHECK_DOC_FIELD_MESSAGE(l1.f< name1 >() , false        );
    ECL_BOOST_CHECK_DOC_FIELD_MESSAGE(l1.f< name3 >() , "name3 node" );

    auto& l2 = l1.f< level2 >();

    int32_t ctr = -3;
    for (auto& e : l2)
    {
        ECL_BOOST_CHECK_DOC_FIELD_MESSAGE(e.f< ar_item1 >() , true    );
        ECL_BOOST_CHECK_DOC_FIELD_MESSAGE(e.f< ar_item2 >() , ctr     );
        ECL_BOOST_CHECK_DOC_FIELD_MESSAGE(e.f< ar_item3 >() , "item3" );
        ++ctr;
    }
}

const static std::string serialized_compact = R"({"name1":true,"name2":2147483647,"level1":{"name1":false,"name3":"name3 node","level2":[{"ar_item1":true,"ar_item2":-3,"ar_item3":"item3"},{"ar_item1":true,"ar_item2":-2,"ar_item3":"item3"},{"ar_item1":true,"ar_item2":-1,"ar_item3":"item3"},{"ar_item1":true,"ar_item2":0,"ar_item3":"item3"},{"ar_item1":true,"ar_item2":1,"ar_item3":"item3"},{"ar_item1":true,"ar_item2":2,"ar_item3":"item3"},{"ar_item1":true,"ar_item2":3,"ar_item3":"item3"},{"ar_item1":true,"ar_item2":4,"ar_item3":"item3"}]},"name5":true})";
const static std::string serialized_beauty  =
R"({
    "name1":true,
    "name2":2147483647,
    "level1":{
        "name1":false,
        "name3":"name3 node",
        "level2":[
            {
                "ar_item1":true,
                "ar_item2":-3,
                "ar_item3":"item3"
            },
            {
                "ar_item1":true,
                "ar_item2":-2,
                "ar_item3":"item3"
            },
            {
                "ar_item1":true,
                "ar_item2":-1,
                "ar_item3":"item3"
            },
            {
                "ar_item1":true,
                "ar_item2":0,
                "ar_item3":"item3"
            },
            {
                "ar_item1":true,
                "ar_item2":1,
                "ar_item3":"item3"
            },
            {
                "ar_item1":true,
                "ar_item2":2,
                "ar_item3":"item3"
            },
            {
                "ar_item1":true,
                "ar_item2":3,
                "ar_item3":"item3"
            },
            {
                "ar_item1":true,
                "ar_item2":4,
                "ar_item3":"item3"
            }
        ]
    },
    "name5":true
})";

BOOST_FIXTURE_TEST_CASE( document_serialization_case, json_fixture )
{
    std::stringstream ss;

    ss << ecl_doc;

    BOOST_CHECK_MESSAGE(ss.str() == serialized_compact, ss.str() << " = " << serialized_compact);
}

BOOST_FIXTURE_TEST_CASE( document_serialization_beautify_case, json_fixture )
{
    std::stringstream ss;

    ecl_doc.serialize(ss, true);

    BOOST_CHECK_MESSAGE(ss.str() == serialized_beauty, ss.str() << " = " << serialized_beauty);
}

template<typename T>
static std::string to_json(T& doc, bool beautify = false)
{
    std::stringstream ss;
    doc.serialize(ss, beautify);
    return ss.str();
}

BOOST_FIXTURE_TEST_CASE( document_deserialization_case, json_fixture )
{
    document_ecl_t doc;
    BOOST_REQUIRE(doc.deserialize(serialized_compact.c_str()));

    BOOST_CHECK_EQUAL(doc.f< name1 >(), true);
    BOOST_CHECK_EQUAL(doc.f< name2 >(), std::numeric_limits<int32_t>::max());
    BOOST_CHECK_EQUAL(doc.f< level1 >().f< name1 >(), false);
    BOOST_CHECK(doc.f< level1 >().f< name3 >() == "name3 node");
    BOOST_CHECK_EQUAL(doc.f< name5 >(), true);

    int32_t ctr = -3;
    for(auto& e : doc.f< level1 >().f< level2 >())
    {
        BOOST_CHECK_EQUAL(e.f< ar_item1 >(), true);
        BOOST_CHECK_EQUAL(e.f< ar_item2 >(), ctr);
        BOOST_CHECK(e.f< ar_item3 >() == "item3");
        ++ctr;
    }
}

BOOST_FIXTURE_TEST_CASE( roundtrip_compact_case, json_fixture )
{
    document_ecl_t doc;
    BOOST_REQUIRE(doc.deserialize(to_json(ecl_doc).c_str()));
    BOOST_CHECK_EQUAL(to_json(doc), serialized_compact);
}

BOOST_FIXTURE_TEST_CASE( roundtrip_beautify_case, json_fixture )
{
    document_ecl_t doc;
    BOOST_REQUIRE(doc.deserialize(serialized_beauty.c_str()));
    BOOST_CHECK_EQUAL(to_json(doc, true), serialized_beauty);
}

BOOST_AUTO_TEST_CASE( deserialization_end_pointer_case )
{
    using doc_t = object < node < name1 , int32_t > >;

    const char* json = R"({"name1":-42}tail)";
    const char* end  = nullptr;

    doc_t doc;
    BOOST_REQUIRE(doc.deserialize(json, &end));
    BOOST_CHECK_EQUAL(doc.f< name1 >(), -42);
    BOOST_CHECK_EQUAL(std::string(end), "tail");
}

BOOST_AUTO_TEST_CASE( deserialization_whitespaces_case )
{
    using doc_t = object
    <
        node < name1 , int32_t >,
        node < name2 , bool    >
    >;

    doc_t doc;
    BOOST_REQUIRE(doc.deserialize(" \t{ \"name1\" :\n 7 ,\r\n \"name2\" : true } "));
    BOOST_CHECK_EQUAL(doc.f< name1 >(), 7);
    BOOST_CHECK_EQUAL(doc.f< name2 >(), true);
}

BOOST_AUTO_TEST_CASE( deserialization_invalid_case )
{
    using doc_t = object
    <
        node < name1 , int32_t >,
        node < name2 , bool    >
    >;

    const char* invalid[] =
    {
        "",
        "{",
        "}",
        R"({"name1":1})",
        R"({"name1":1,"name2":true)",
        R"({"name1":1,"name2":yes})",
        R"({"name2":true,"name1":1})",
        R"({"nameX":1,"name2":true})",
        R"({"name1":1 "name2":true})",
        R"({"name1"1,"name2":true})",
        R"(["name1",1])",
    };

    for(const char* json : invalid)
    {
        doc_t doc;
        BOOST_CHECK_MESSAGE(!doc.deserialize(json), "accepted invalid json: " << json);
    }

    doc_t doc;
    BOOST_CHECK(!doc.deserialize(nullptr));
}

BOOST_AUTO_TEST_CASE( deserialization_numbers_case )
{
    using doc_t = object
    <
        node < name1 , int8_t   >,
        node < name2 , uint16_t >,
        node < name3 , int64_t  >,
        node < name4 , uint64_t >
    >;

    doc_t doc;
    BOOST_REQUIRE(doc.deserialize(
        R"({"name1":-128,"name2":65535,"name3":-9223372036854775807,"name4":18446744073709551615})"));

    BOOST_CHECK_EQUAL(+doc.f< name1 >(), -128);
    BOOST_CHECK_EQUAL(doc.f< name2 >(), 65535);
    BOOST_CHECK_EQUAL(doc.f< name3 >(), -9223372036854775807LL);
    BOOST_CHECK_EQUAL(doc.f< name4 >(), 18446744073709551615ULL);
}

BOOST_AUTO_TEST_CASE( deserialization_not_a_number_case )
{
    using doc_t = object < node < name1 , int32_t > >;

    doc_t doc;
    BOOST_CHECK(!doc.deserialize(R"({"name1":})"));
    BOOST_CHECK(!doc.deserialize(R"({"name1":"1"})"));
}

BOOST_AUTO_TEST_CASE( deserialization_floating_point_case )
{
    floating_point_doc_t doc;
    BOOST_REQUIRE(doc.deserialize(R"({"name1":1.5,"name2":0.1,"name3":-2.25})"));

    BOOST_CHECK_EQUAL(doc.f< name1 >(), 1.5f);
    BOOST_CHECK_EQUAL(doc.f< name3 >(), -2.25L);

    BOOST_CHECK_CLOSE(doc.f< name2 >(), 0.1, 1e-12);
}

BOOST_AUTO_TEST_CASE( deserialization_partial_array_case )
{
    using doc_t = object
    <
        node < level2 , array < object < node < ar_item2 , int32_t > >, 8 > >
    >;

    doc_t doc;
    BOOST_REQUIRE(doc.deserialize(R"({"level2":[{"ar_item2":1},{"ar_item2":2},{"ar_item2":3}]})"));

    BOOST_CHECK_EQUAL(to_json(doc), R"({"level2":[{"ar_item2":1},{"ar_item2":2},{"ar_item2":3}]})");

    BOOST_REQUIRE(doc.deserialize(R"({"level2":[]})"));
    BOOST_CHECK_EQUAL(to_json(doc), R"({"level2":[]})");
}

BOOST_AUTO_TEST_CASE( deserialization_array_overflow_case )
{
    using doc_t = object
    <
        node < level2 , array < object < node < ar_item2 , int32_t > >, 2 > >
    >;

    doc_t doc;
    BOOST_CHECK(!doc.deserialize(R"({"level2":[{"ar_item2":1},{"ar_item2":2},{"ar_item2":3}]})"));
}

BOOST_AUTO_TEST_CASE( deserialization_string_case )
{
    using doc_t = object < node < name1 , string < 8 > > >;

    doc_t doc;
    BOOST_REQUIRE(doc.deserialize(R"({"name1":"abcdef"})"));
    BOOST_CHECK(doc.f< name1 >() == "abcdef");

    BOOST_REQUIRE(doc.deserialize(R"({"name1":"xy"})"));
    BOOST_CHECK_MESSAGE(doc.f< name1 >() == "xy", doc.f< name1 >() << " = xy");

    BOOST_REQUIRE(doc.deserialize(R"({"name1":""})"));
    BOOST_CHECK_MESSAGE(doc.f< name1 >() == "", doc.f< name1 >() << " = \"\"");
}

BOOST_AUTO_TEST_CASE( deserialization_string_max_size_case )
{
    using doc_t = object < node < name1 , string < 4 > > >;

    doc_t doc;
    BOOST_CHECK(doc.deserialize(R"({"name1":"abcd"})"));
    BOOST_CHECK(doc.f< name1 >() == "abcd");

    BOOST_CHECK(!doc.deserialize(R"({"name1":"abcde"})"));
}

BOOST_AUTO_TEST_CASE( string_escape_roundtrip_case )
{
    using doc_t = object < node < name1 , string < 16 > > >;

    const char* values[] = { "q\"q", "back\\slash", "new\nline", "tab\tx" };

    for(const char* v : values)
    {
        doc_t src;
        src.f< name1 >() = v;

        doc_t dst;
        BOOST_REQUIRE(dst.deserialize(to_json(src).c_str()));
        BOOST_CHECK_MESSAGE(dst.f< name1 >() == v, "roundtrip failed: " << to_json(src) << " -> " << to_json(dst));
    }
}

BOOST_FIXTURE_TEST_CASE( std_document_roundtrip_case, json_fixture )
{
    BOOST_CHECK_EQUAL(to_json(std_doc), serialized_compact);

    document_std_t doc;
    BOOST_REQUIRE(doc.deserialize(serialized_beauty.c_str()));
    BOOST_CHECK_EQUAL(to_json(doc), serialized_compact);
}

BOOST_AUTO_TEST_CASE( std_string_deserialization_case )
{
    using doc_t = object < node < name1 , std::string > >;

    doc_t doc;
    BOOST_REQUIRE(doc.deserialize(R"({"name1":"a long string value"})"));
    BOOST_CHECK_EQUAL(doc.f< name1 >(), "a long string value");

    BOOST_REQUIRE(doc.deserialize(R"({"name1":"xy"})"));
    BOOST_CHECK_EQUAL(doc.f< name1 >(), "xy");

    BOOST_REQUIRE(doc.deserialize(R"({"name1":"q\"b\\n\n"})"));
    BOOST_CHECK_EQUAL(doc.f< name1 >(), "q\"b\\n\n");

    BOOST_CHECK(!doc.deserialize(R"({"name1":"unterminated})"));
    BOOST_CHECK(!doc.deserialize(R"({"name1":"bad \x escape"})"));
}

BOOST_AUTO_TEST_CASE( serialization_to_ecl_stream_case )
{
    using doc_t = object
    <
        node < name1 , int32_t      >,
        node < name2 , bool         >,
        node < name3 , string < 8 > >
    >;

    doc_t doc;
    doc.f< name1 >() = -15;
    doc.f< name2 >() = false;
    doc.f< name3 >() = "str";

    std::string out;
    ecl::stream<128> st([&out] (const char* const buf, std::size_t size) { out.append(buf, size); });

    st << doc << ecl::end();

    BOOST_CHECK_EQUAL(out, R"({"name1":-15,"name2":false,"name3":"str"})");
}

BOOST_AUTO_TEST_SUITE_END()

#endif // ECL_TEST_JSON_HPP
