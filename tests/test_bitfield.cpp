#include <stdio.h>

#include <iostream>

#include <ecl/bit_field.hpp>

// #include <gtest/gtest.h>

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

namespace numerical_chars {
std::ostream &operator<<(std::ostream &os, char c) {
    return os << (std::is_signed<char>::value ? static_cast<int>(c)
                                              : static_cast<unsigned int>(c));
}

std::ostream &operator<<(std::ostream &os, signed char c) {
    return os << static_cast<int>(c);
}

std::ostream &operator<<(std::ostream &os, unsigned char c) {
    return os << static_cast<unsigned int>(c);
}
}

using namespace numerical_chars;

void dump_bit_field(test_bit_field_t const& bf) {
    std::cout <<
      bf.f0 << " " << bf.f1 << " " << bf.f2  << " " << bf.f3  << " " << 
      bf.f4 << " " << bf.f5 << " " << bf.f6  << " " << bf.f7  << " " << 
      bf.f8 << " " << bf.f9 << " " << bf.f10 << " " << bf.f11 << " " << 
      bf.f12 << std::endl;

    // printf("Fields:\n\r"
    //        "f0:  %u\n\r"
    //        "f1:  %u\n\r"
    //        "f2:  %u\n\r"
    //        "f3:  %u\n\r"
    //        "f4:  %u\n\r"
    //        "f5:  %u\n\r"
    //        "f6:  %u\n\r"
    //        "f7:  %u\n\r"
    //        "f8:  %u\n\r"
    //        "f9:  %lu\n\r"
    //        "f10: %u\n\r"
    //        "f11: %u\n\r"
    //        "f12: %lu\n\r",
    //        bf.f0, bf.f1, bf.f2, bf.f3, bf.f4, bf.f5, bf.f6, bf.f7, bf.f8, bf.f9, 
    //        bf.f10, bf.f11, bf.f12);
}

int main(int argc, char** argv)
{
    (void)(argc);
    (void)(argv);

    test_bit_field_t bf;
    test_bit_field_2_t bf_2;
    // test_bit_field_3_t bf_3;

    size_t bf_size = sizeof(test_bit_field_2_t);
    size_t st_size = sizeof(test_struct_2);

    std::cout << "Bit field test."  << std::endl;
    std::cout << "struct size:    " << st_size << std::endl;
    std::cout << "bit field size: " << bf_size << std::endl;

    bf.f0 =  1;
    bf.f1 =  2;
    bf.f2 =  3;
    bf.f3 =  4;
    bf.f4 =  5;
    bf.f5 =  6;
    bf.f6 =  7;
    bf.f7 =  8;
    bf.f8 =  9;
    bf.f9 =  10;
    bf.f10 = 11;
    bf.f11 = 12;
    bf.f12 = 13;

    dump_bit_field(bf);

    bf.pack();

    std::cout << "Packed" << std::endl;
    dump_bit_field(bf);

    bf.f0 =  0;
    bf.f1 =  0;
    bf.f2 =  0;
    bf.f3 =  0;
    bf.f4 =  0;
    bf.f5 =  0;
    bf.f6 =  0;
    bf.f7 =  0;
    bf.f8 =  0;
    bf.f9 =  0;
    bf.f10 = 0;
    bf.f11 = 0;
    bf.f12 = 0;

    std::cout << "Set all to 0" << std::endl;
    dump_bit_field(bf);

    bf.unpack();

    std::cout << "Unpacked" << std::endl;
    dump_bit_field(bf);
}