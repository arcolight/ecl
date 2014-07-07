#include <stdio.h>
#include <string.h>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <limits>

#include <ecl/log.hpp>
#include "test_log.hpp"

using namespace ecl;

template<>
logger<2048, serial_out>::alphabet_char_t
logger<2048, serial_out>::m_s_binary_alphabet[] = {
    { '0', '0' },
    { '1', '1' }
};

template<>
logger<2048, serial_out>::alphabet_char_t 
logger<2048, serial_out>::m_s_octal_alphabet[] = {
    { '0', '0' },
    { '1', '1' },
    { '2', '2' },
    { '3', '3' },
    { '4', '4' },
    { '5', '5' },
    { '6', '6' },
    { '7', '7' }
};

template<>
logger<2048, serial_out>::alphabet_char_t 
logger<2048, serial_out>::m_s_decimal_alphabet[] = {
    { '0', '0' },
    { '1', '1' },
    { '2', '2' },
    { '3', '3' },
    { '4', '4' },
    { '5', '5' },
    { '6', '6' },
    { '7', '7' },
    { '8', '8' },
    { '9', '9' }
};

template<>
logger<2048, serial_out>::alphabet_char_t 
logger<2048, serial_out>::m_s_hex_alphabet[] = {
    { '0', '0' },
    { '1', '1' },
    { '2', '2' },
    { '3', '3' },
    { '4', '4' },
    { '5', '5' },
    { '6', '6' },
    { '7', '7' },
    { '8', '8' },
    { '9', '9' },
    { 'a', 'A' },
    { 'b', 'B' },
    { 'c', 'C' },
    { 'd', 'D' },
    { 'e', 'E' },
    { 'f', 'F' }
};

void printf_nums(int8_t  i8,  uint8_t  u8,
                 int16_t i16, uint16_t u16,
                 int32_t i32, uint32_t u32,
                 int64_t i64, uint64_t u64)
{
#ifdef __arm__

    printf("[ PRINTF ]\r\n");
    printf("Octal:\r\n"
           "int8_t:  %o | uint8_t:  %o \r\n"
           "int16_t: %o | uint16_t: %o \r\n"
           "int32_t: %lo | uint32_t: %lo \r\n"
           "int64_t: %llo | uint64_t: %llo \r\n\r\n",
           i8, u8, i16, u16, i32, u32, i64, u64);

    printf("Decimal:\r\n"
           "int8_t:  %d | uint8_t:  %u \r\n"
           "int16_t: %d | uint16_t: %u \r\n"
           "int32_t: %ld | uint32_t: %lu \r\n"
           "int64_t: %lld | uint64_t: %llu \r\n\r\n",
           i8, u8, i16, u16, i32, u32, i64, u64);

    printf("Hex:\r\n"
           "int8_t:  %x | uint8_t:  %x \r\n"
           "int16_t: %x | uint16_t: %x \r\n"
           "int32_t: %lx | uint32_t: %lx \r\n"
           "int64_t: %llx | uint64_t: %llx \r\n\r\n",
           i8, u8, i16, u16, i32, u32, i64, u64);

#else

    printf("[ PRINTF ]\r\n");
    printf("Octal:\r\n"
           "int8_t:  %o | uint8_t:  %o \r\n"
           "int16_t: %o | uint16_t: %o \r\n"
           "int32_t: %o | uint32_t: %o \r\n"
           "int64_t: %lo | uint64_t: %lo \r\n\r\n",
           i8, u8, i16, u16, i32, u32, i64, u64);

    printf("Decimal:\r\n"
           "int8_t:  %d | uint8_t:  %u \r\n"
           "int16_t: %d | uint16_t: %u \r\n"
           "int32_t: %d | uint32_t: %u \r\n"
           "int64_t: %ld | uint64_t: %lu \r\n\r\n",
           i8, u8, i16, u16, i32, u32, i64, u64);

    printf("Hex:\r\n"
           "int8_t:  %x | uint8_t:  %x \r\n"
           "int16_t: %x | uint16_t: %x \r\n"
           "int32_t: %x | uint32_t: %x \r\n"
           "int64_t: %lx | uint64_t: %lx \r\n\r\n",
           i8, u8, i16, u16, i32, u32, i64, u64);
    
#endif
}

void out_nums(int8_t  i8,  uint8_t  u8,
              int16_t i16, uint16_t u16,
              int32_t i32, uint32_t u32,
              int64_t i64, uint64_t u64)
{
    logger<2048, serial_out> l;

    l(lvl::info) << "[ LOG ]" << "\r\n" << end();

    l(lvl::info)(base::b)(8)    << "Binary:"           << "\r\n" <<
    "int8_t:  " << i8  << " | " << "uint8_t:  " << u8  << "\r\n" <<
    "int16_t: " << i16 << " | " << "uint16_t: " << u16 << "\r\n" <<
    "int32_t: " << i32 << " | " << "uint32_t: " << u32 << "\r\n" <<
    "int64_t: " << i64 << " | " << "uint64_t: " << u64 << "\r\n" << end();

    l(lvl::info)(base::o)(8)  << "Octal:"            << "\r\n" <<
    "int8_t:  " << i8  << " | " << "uint8_t:  " << u8  << "\r\n" <<
    "int16_t: " << i16 << " | " << "uint16_t: " << u16 << "\r\n" <<
    "int32_t: " << i32 << " | " << "uint32_t: " << u32 << "\r\n" <<
    "int64_t: " << i64 << " | " << "uint64_t: " << u64 << "\r\n" << end();

    l(lvl::info)(base::d)(8)  << "Decimal:"          << "\r\n" <<
    "int8_t:  " << i8  << " | " << "uint8_t:  " << u8  << "\r\n" <<
    "int16_t: " << i16 << " | " << "uint16_t: " << u16 << "\r\n" <<
    "int32_t: " << i32 << " | " << "uint32_t: " << u32 << "\r\n" <<
    "int64_t: " << i64 << " | " << "uint64_t: " << u64 << "\r\n" << end();

    l(lvl::info)(base::h)(8)  << "Hex:"              << "\r\n" <<
    "int8_t:  " << i8  << " | " << "uint8_t:  " << u8  << "\r\n" <<
    "int16_t: " << i16 << " | " << "uint16_t: " << u16 << "\r\n" <<
    "int32_t: " << i32 << " | " << "uint32_t: " << u32 << "\r\n" <<
    "int64_t: " << i64 << " | " << "uint64_t: " << u64 << "\r\n" << end();
}

void printf_floats(float_t f, double_t d)
{
    printf("[ PRINTF ]\r\n");

    printf("float:  %8.8f | double: %8.8f\r\n", f, d);
    printf("float:  %6.6f | double: %6.6f\r\n", f, d);
    printf("float:  %4.4f | double: %4.4f\r\n", f, d);
}

void out_floats(float_t f, double_t d)
{
    logger<2048, serial_out> l;

    l(lvl::info)(8) << "[ LOG ]" << "\r\n" <<
    "float:  " << f << " | " << "double: " << d << "\r\n";

    l(lvl::info)(6) << 
    "float:  " << f << " | " << "double: " << d << "\r\n";

    l(lvl::info)(4) << 
    "float:  " << f << " | " << "double: " << d << "\r\n";

    l << end();
}

void out_bool_char(bool b, char c)
{
    logger<32, serial_out> l;

    l << "bool: " << b << " | " << "char: " << c << "\r\n" << end();
}

int main(int argc, char** argv)
{
    (void)(argc);
    (void)(argv);

#ifdef __arm__

    printf("sizeof(float):       %u\r\n"
           "sizeof(double):      %u\r\n"
           "sizeof(long double): %u\r\n", sizeof(float),
                                          sizeof(double),
                                          sizeof(long double));

#else

    printf("sizeof(float):       %lu\r\n"
           "sizeof(double):      %lu\r\n"
           "sizeof(long double): %lu\r\n", sizeof(float),
                                           sizeof(double),
                                           sizeof(long double));

#endif

    float  f = -1;
    double d = -1;

    printf_floats(f,d);
    out_floats(f, d);

    f = 1;
    d = 1;

    printf_floats(f,d);
    out_floats(f, d);

    f = 2.7181;
    d = 2.7181;

    printf_floats(f,d);
    out_floats(f, d);

    f = std::numeric_limits<float>::min();
    d = std::numeric_limits<double>::min();

    printf_floats(f,d);
    out_floats(f, d);

    f = std::numeric_limits<float>::max();
    d = std::numeric_limits<double>::max();

    printf_floats(f,d);
    out_floats(f, d);

    std::srand(std::time(0));
    int random_variable = std::rand();
    (void)(random_variable);

    printf("Using random variable: %d\r\n\r\n", random_variable);
    printf("TEST1\r\n");

    int8_t   i8 =  std::numeric_limits<int8_t>::max() - random_variable;
    uint8_t  u8 =  std::numeric_limits<uint8_t>::max() - random_variable;
    int16_t  i16 = std::numeric_limits<int16_t>::max() - random_variable;
    uint16_t u16 = std::numeric_limits<uint16_t>::max() - random_variable;
    int32_t  i32 = std::numeric_limits<int32_t>::max() - random_variable;
    uint32_t u32 = std::numeric_limits<uint32_t>::max() - random_variable;
    int64_t  i64 = std::numeric_limits<int64_t>::max() - random_variable;
    uint64_t u64 = std::numeric_limits<uint64_t>::max() - random_variable;

    printf_nums(i8, u8, i16, u16, i32, u32, i64, u64);
    out_nums(i8, u8, i16, u16, i32, u32, i64, u64);

    printf("TEST2\r\n");
    i8 =  std::numeric_limits<int8_t>::min() + random_variable;
    u8 =  std::numeric_limits<uint8_t>::min() + random_variable;
    i16 = std::numeric_limits<int16_t>::min() + random_variable;
    u16 = std::numeric_limits<uint16_t>::min() + random_variable;
    i32 = std::numeric_limits<int32_t>::min() + random_variable;
    u32 = std::numeric_limits<uint32_t>::min() + random_variable;
    i64 = std::numeric_limits<int64_t>::min() + random_variable;
    u64 = std::numeric_limits<uint64_t>::min() + random_variable;

    printf_nums(i8, u8, i16, u16, i32, u32, i64, u64);
    out_nums(i8, u8, i16, u16, i32, u32, i64, u64);

    char c = '!';
    bool b = true;

    out_bool_char(b, c);

    c = '?';
    b = false;

    out_bool_char(b, c);
}
