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

// BOOST_AUTO_TEST_CASE( string_serialization_test )
// {
//     using string4_t  = ecl::json::string < 4  >;
//     using string8_t  = ecl::json::string < 8  >;
//     using string16_t = ecl::json::string < 16 >;

//     string4_t  s4  = "abcd";
//     string8_t  s8  = "abcdefgh";
//     string16_t s16 = "abcdefghabcdefgh";

//     std::stringstream serialized;
//     serialized << s4;

//     BOOST_CHECK_MESSAGE()

//     serialized.swap(std::stringstream());
// }

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

// BOOST_FIXTURE_TEST_CASE( document_deserialization_case, json_fixture )
// {
//     // floating_point_doc_t doc_f { 0.0, 0.0, 0.0 };
// }

BOOST_AUTO_TEST_SUITE_END()

#endif // ECL_TEST_JSON_HPP
