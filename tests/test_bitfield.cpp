#include <stdio.h>

#include <iostream>

#include <ecl/bit_field.hpp>

#include "bit_field_test_header.hpp"

// #include <gtest/gtest.h>

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