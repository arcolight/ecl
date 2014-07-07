#ifndef BIT_FIELD_TEST_HEADER
#define BIT_FIELD_TEST_HEADER

#include <cstdint>

#include <ecl/bit_field.hpp>

struct test_struct {
    uint8_t  f0;
    uint16_t f1;
    uint32_t f2;
    uint8_t  f3;
    uint8_t  f4;
    uint8_t  f5;
    uint8_t  f6;
    uint8_t  f7;
    uint8_t  f8;
    uint64_t f9;
    uint8_t  f10;
    uint32_t f11;
    uint64_t f12;
};

struct test_struct_2 {
  uint8_t f0;
  uint8_t f1;
};

struct test_struct_3 {
  uint8_t array[10];
};

#define TEST_STRUCT_PACK_SIZE_BYTES 29
#define F0_SIZE                     5
#define F1_SIZE                     11
#define F2_SIZE                     26
#define F3_SIZE                     2
#define F4_SIZE                     7
#define F5_SIZE                     8
#define F6_SIZE                     8
#define F7_SIZE                     1
#define F8_SIZE                     3
#define F9_SIZE                     58
#define F10_SIZE                    4
#define F11_SIZE                    31
#define F12_SIZE                    64

typedef ecl::bit_field<TEST_STRUCT_PACK_SIZE_BYTES, test_struct,
   ECL_FIELD(test_struct, f0,  F0_SIZE),
   ECL_FIELD(test_struct, f1,  F1_SIZE),
   ECL_FIELD(test_struct, f2,  F2_SIZE),
   ECL_FIELD(test_struct, f3,  F3_SIZE),
   ECL_FIELD(test_struct, f4,  F4_SIZE),
   ECL_FIELD(test_struct, f5,  F5_SIZE),
   ECL_FIELD(test_struct, f6,  F6_SIZE),
   ECL_FIELD(test_struct, f7,  F7_SIZE),
   ECL_FIELD(test_struct, f8,  F8_SIZE),
   ECL_FIELD(test_struct, f9,  F9_SIZE),
   ECL_FIELD(test_struct, f10, F10_SIZE),
   ECL_FIELD(test_struct, f11, F11_SIZE),
   ECL_FIELD(test_struct, f12, F12_SIZE)
> test_bit_field_t;

typedef ecl::bit_field<1, test_struct_2,
   ECL_FIELD(test_struct_2, f0, 1),
   ECL_FIELD(test_struct_2, f1, 1)
> test_bit_field_2_t;

typedef ecl::bit_field<10, test_struct_3,
   ECL_FIELD(test_struct_3, array, 80)
> test_bit_field_3_t;

#endif // BIT_FIELD_TEST_HEADER
