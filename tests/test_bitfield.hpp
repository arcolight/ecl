#ifndef ECL_TEST_BITFIELD_HPP
#define ECL_TEST_BITFIELD_HPP

#include <ecl/bit_field.hpp>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( bit_field_suite )

struct test_struct
{
    std::uint8_t  f0  { 0 };
    std::uint16_t f1  { 0 };
    std::uint32_t f2  { 0 };
    std::uint8_t  f3  { 0 };
    std::uint8_t  f4  { 0 };
    std::uint8_t  f5  { 0 };
    std::uint8_t  f6  { 0 };
    std::uint8_t  f7  { 0 };
    std::uint8_t  f8  { 0 };
    std::uint64_t f9  { 0 };
    std::uint8_t  f10 { 0 };
    std::uint32_t f11 { 0 };
    std::uint64_t f12 { 0 };
};

constexpr static std::size_t TEST_STRUCT_PACK_SIZE_BYTES { 29 };

constexpr static std::size_t F0_SIZE                     { 5  };
constexpr static std::size_t F1_SIZE                     { 11 };
constexpr static std::size_t F2_SIZE                     { 26 };
constexpr static std::size_t F3_SIZE                     { 2  };
constexpr static std::size_t F4_SIZE                     { 7  };
constexpr static std::size_t F5_SIZE                     { 8  };
constexpr static std::size_t F6_SIZE                     { 8  };
constexpr static std::size_t F7_SIZE                     { 1  };
constexpr static std::size_t F8_SIZE                     { 3  };
constexpr static std::size_t F9_SIZE                     { 58 };
constexpr static std::size_t F10_SIZE                    { 4  };
constexpr static std::size_t F11_SIZE                    { 31 };
constexpr static std::size_t F12_SIZE                    { 64 };

using test_bit_field_t = ecl::bit_field
<
      test_struct
    , ECL_FIELD( test_struct , f0  , F0_SIZE  )
    , ECL_FIELD( test_struct , f1  , F1_SIZE  )
    , ECL_FIELD( test_struct , f2  , F2_SIZE  )
    , ECL_FIELD( test_struct , f3  , F3_SIZE  )
    , ECL_FIELD( test_struct , f4  , F4_SIZE  )
    , ECL_FIELD( test_struct , f5  , F5_SIZE  )
    , ECL_FIELD( test_struct , f6  , F6_SIZE  )
    , ECL_FIELD( test_struct , f7  , F7_SIZE  )
    , ECL_FIELD( test_struct , f8  , F8_SIZE  )
    , ECL_FIELD( test_struct , f9  , F9_SIZE  )
    , ECL_FIELD( test_struct , f10 , F10_SIZE )
    , ECL_FIELD( test_struct , f11 , F11_SIZE )
    , ECL_FIELD( test_struct , f12 , F12_SIZE )
>;

void dump_bit_field(test_bit_field_t const& bf)
{
    BOOST_TEST_MESSAGE(
       +bf.f0  << " " << +bf.f1 << " " << +bf.f2  << " " << +bf.f3  << " " <<
       +bf.f4  << " " << +bf.f5 << " " << +bf.f6  << " " << +bf.f7  << " " <<
       +bf.f8  << " " << +bf.f9 << " " << +bf.f10 << " " << +bf.f11 << " " <<
       +bf.f12);
}

struct bit_field_fixture
{
    bit_field_fixture()
        : bf()
    {
        BOOST_TEST_MESSAGE( "setup bit_field_fixture" );
    }

    ~bit_field_fixture()
    {
        BOOST_TEST_MESSAGE( "teardown bit_field_fixture" );
    }

    test_bit_field_t bf;
};

#define ECL_BOOST_CHECK_MESSAGE_BF_FIELD(field, value) \
    BOOST_CHECK_MESSAGE(field == value, std::hex << +field << " = " << +value)

template<typename T>
void check_field(test_bit_field_t& bf, T& f, std::size_t field_size)
{
    T mask = 0x0;

    for(std::size_t i = 0; i < field_size; ++i)
    {
        mask |= 1ULL << i;
    }

    f = std::numeric_limits<T>::max();
    bf.pack();
    bf.unpack();

    ECL_BOOST_CHECK_MESSAGE_BF_FIELD(f, mask);
}

BOOST_FIXTURE_TEST_CASE ( size_case, bit_field_fixture )
{
    BOOST_CHECK_MESSAGE(TEST_STRUCT_PACK_SIZE_BYTES == bf.size, "Static size calculation mismatch!");
    dump_bit_field(bf);
}

BOOST_FIXTURE_TEST_CASE ( fill_case, bit_field_fixture )
{
    check_field( bf , bf.f0  , F0_SIZE  );
    check_field( bf , bf.f1  , F1_SIZE  );
    check_field( bf , bf.f2  , F2_SIZE  );
    check_field( bf , bf.f3  , F3_SIZE  );
    check_field( bf , bf.f4  , F4_SIZE  );
    check_field( bf , bf.f5  , F5_SIZE  );
    check_field( bf , bf.f6  , F6_SIZE  );
    check_field( bf , bf.f7  , F7_SIZE  );
    check_field( bf , bf.f8  , F8_SIZE  );
    check_field( bf , bf.f9  , F9_SIZE  );
    check_field( bf , bf.f10 , F10_SIZE );
    check_field( bf , bf.f11 , F11_SIZE );
    check_field( bf , bf.f12 , F12_SIZE );

    dump_bit_field(bf);
}

BOOST_AUTO_TEST_SUITE_END()

#endif // ECL_TEST_BITFIELD_HPP
