#ifndef ECL_TEST_BITFIELD_HPP
#define ECL_TEST_BITFIELD_HPP

#include <ecl/bit_field.hpp>

#include <boost/test/unit_test.hpp>

#include <limits>

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

BOOST_FIXTURE_TEST_CASE ( zero_case, bit_field_fixture )
{
    const std::uint8_t* data = bf.pack();

    for(std::size_t i = 0; i < test_bit_field_t::size; ++i)
    {
        BOOST_CHECK_EQUAL(+data[i], 0);
    }
}

BOOST_FIXTURE_TEST_CASE ( isolation_case, bit_field_fixture )
{
    bf.f4 = std::numeric_limits<std::uint8_t>::max();
    bf.pack();
    bf.unpack();

    BOOST_CHECK_EQUAL(+bf.f3, 0);
    BOOST_CHECK_EQUAL(+bf.f4, (1 << F4_SIZE) - 1);
    BOOST_CHECK_EQUAL(+bf.f5, 0);

    const std::uint8_t* data = bf.pack();
    std::size_t bits = 0;
    for(std::size_t i = 0; i < test_bit_field_t::size; ++i)
    {
        for(std::uint8_t b = data[i]; b != 0; b >>= 1)
        {
            bits += b & 0x01;
        }
    }
    BOOST_CHECK_EQUAL(bits, F4_SIZE);
}

struct small_struct
{
    std::uint8_t  a { 0 };
    std::uint8_t  b { 0 };
    std::uint16_t c { 0 };
};

using small_bit_field_t = ecl::bit_field
<
      small_struct
    , ECL_FIELD( small_struct , a , 3 )
    , ECL_FIELD( small_struct , b , 7 )
    , ECL_FIELD( small_struct , c , 9 )
>;

static const std::uint8_t small_packed[] { 0xB9, 0x87, 0x20 };

BOOST_AUTO_TEST_CASE ( pack_layout_case )
{
    BOOST_REQUIRE_EQUAL(std::size_t { small_bit_field_t::size }, sizeof(small_packed));

    small_bit_field_t sbf;
    sbf.a = 0x05;
    sbf.b = 0x66;
    sbf.c = 0x039;

    const std::uint8_t* data = sbf.pack();

    BOOST_CHECK_EQUAL_COLLECTIONS(data, data + small_bit_field_t::size,
                                  small_packed, small_packed + sizeof(small_packed));
}

BOOST_AUTO_TEST_CASE ( set_data_case )
{
    small_bit_field_t sbf;
    sbf.set_data(small_packed);

    BOOST_CHECK_EQUAL(+sbf.a, 0x05);
    BOOST_CHECK_EQUAL(+sbf.b, 0x66);
    BOOST_CHECK_EQUAL(+sbf.c, 0x039);
}

BOOST_AUTO_TEST_CASE ( truncation_case )
{
    small_bit_field_t sbf;
    sbf.a = 0xFF;
    sbf.b = 0x00;
    sbf.c = 0xFFFF;
    sbf.pack();

    sbf.a = 0;
    sbf.c = 0;
    sbf.unpack();

    BOOST_CHECK_EQUAL(+sbf.a, 0x07);
    BOOST_CHECK_EQUAL(+sbf.b, 0x00);
    BOOST_CHECK_EQUAL(+sbf.c, 0x1FF);
}

BOOST_AUTO_TEST_CASE ( copy_case )
{
    small_bit_field_t src;
    src.set_data(small_packed);

    small_bit_field_t copy(src);
    BOOST_CHECK_EQUAL(+copy.a, 0x05);
    BOOST_CHECK_EQUAL(+copy.b, 0x66);
    BOOST_CHECK_EQUAL(+copy.c, 0x039);

    small_bit_field_t assigned;
    assigned = src;
    BOOST_CHECK_EQUAL(+assigned.a, 0x05);
    BOOST_CHECK_EQUAL(+assigned.b, 0x66);
    BOOST_CHECK_EQUAL(+assigned.c, 0x039);
}

BOOST_AUTO_TEST_SUITE_END()

#endif // ECL_TEST_BITFIELD_HPP
