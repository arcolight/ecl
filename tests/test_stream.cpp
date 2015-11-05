#include <ecl/stream.hpp>

#include <stdio.h>
#include <string.h>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <iostream>

using namespace ecl;

void printf_nums(int8_t  i8,  uint8_t  u8,
                 int16_t i16, uint16_t u16,
                 int32_t i32, uint32_t u32,
                 int64_t i64, uint64_t u64);
void out_nums(int8_t  i8,  uint8_t  u8,
              int16_t i16, uint16_t u16,
              int32_t i32, uint32_t u32,
              int64_t i64, uint64_t u64);

void printf_floats(float_t f, double_t d);
void out_floats(float_t f, double_t d);

void out_bool_char(bool b, char c);

void printf_nums(int8_t  i8,  uint8_t  u8,
                 int16_t i16, uint16_t u16,
                 int32_t i32, uint32_t u32,
                 int64_t i64, uint64_t u64)
{
#ifdef __arm__

    printf("[ PRINTF ]------------------------------");
    printf("----------------------------------------\r\n");

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

    printf("[ PRINTF ]------------------------------");
    printf("----------------------------------------\r\n");

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
    stream<2048> l;

    l << "[ STREAM ]------------------------------"
      << "----------------------------------------" << "\r\n";

    l(base::b)(8)    << "Binary:"           << "\r\n" <<
    "int8_t:  " << i8  << " | " << "uint8_t:  " << u8  << "\r\n" <<
    "int16_t: " << i16 << " | " << "uint16_t: " << u16 << "\r\n" <<
    "int32_t: " << i32 << " | " << "uint32_t: " << u32 << "\r\n" <<
    "int64_t: " << i64 << " | " << "uint64_t: " << u64 << "\r\n";
    std::cout << l << std::endl;
    l << rst();

    l(base::o)(8)  << "Octal:"            << "\r\n" <<
    "int8_t:  " << i8  << " | " << "uint8_t:  " << u8  << "\r\n" <<
    "int16_t: " << i16 << " | " << "uint16_t: " << u16 << "\r\n" <<
    "int32_t: " << i32 << " | " << "uint32_t: " << u32 << "\r\n" <<
    "int64_t: " << i64 << " | " << "uint64_t: " << u64 << "\r\n";
    std::cout << l << std::endl;
    l << rst();

    l(base::d)(8)  << "Decimal:"          << "\r\n" <<
    "int8_t:  " << i8  << " | " << "uint8_t:  " << u8  << "\r\n" <<
    "int16_t: " << i16 << " | " << "uint16_t: " << u16 << "\r\n" <<
    "int32_t: " << i32 << " | " << "uint32_t: " << u32 << "\r\n" <<
    "int64_t: " << i64 << " | " << "uint64_t: " << u64 << "\r\n";
    std::cout << l << std::endl;
    l << rst();

    l(base::h)(8)  << "Hex:"              << "\r\n" <<
    "int8_t:  " << i8  << " | " << "uint8_t:  " << u8  << "\r\n" <<
    "int16_t: " << i16 << " | " << "uint16_t: " << u16 << "\r\n" <<
    "int32_t: " << i32 << " | " << "uint32_t: " << u32 << "\r\n" <<
    "int64_t: " << i64 << " | " << "uint64_t: " << u64 << "\r\n";
    std::cout << l << std::endl;
    l << rst();
}

void printf_floats(float_t f, double_t d)
{
    printf("[ PRINTF ]------------------------------");
    printf("----------------------------------------\r\n");

    printf("float:  %8.8f | double: %8.8f\r\n", f, d);
    printf("float:  %6.6f | double: %6.6f\r\n", f, d);
    printf("float:  %4.4f | double: %4.4f\r\n", f, d);
}

void out_floats(float_t f, double_t d)
{
    stream<2048> l;

    l(8) << "[ STREAM ]------------------------------"
         << "----------------------------------------" << "\r\n" <<

    "float:  " << f << " | " << "double: " << d << "\r\n";

    l(6) <<
    "float:  " << f << " | " << "double: " << d << "\r\n";

    l(4) <<
    "float:  " << f << " | " << "double: " << d << "\r\n";

    std::cout << l << std::endl;
    l << rst();
}

void out_bool_char(bool b, char c)
{
    stream<32> l;

    l << "bool: " << b << " | " << "char: " << c << "\r\n";
    std::cout << l << std::endl;
    l << rst();
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

    float  f = -1.0f;
    double d = -1.0;

    printf_floats(f, d);
    out_floats(f, d);

    f = 1;
    d = 1;

    printf_floats(f, d);
    out_floats(f, d);

    f = 2.7181f;
    d = -2.7181;

    printf_floats(f, d);
    out_floats(f, d);

    f = 0.1f;
    d = -0.2;

    printf_floats(f, d);
    out_floats(f, d);

    f = std::numeric_limits<float>::min();
    d = std::numeric_limits<double>::min();

    printf_floats(f, d);
    out_floats(f, d);

    f = std::numeric_limits<float>::max();
    d = std::numeric_limits<double>::max();

    printf_floats(f, d);
    out_floats(f, d);

    std::srand(static_cast<unsigned>(std::time(0)));
    uint8_t random_variable = static_cast<uint8_t>(std::rand());

    printf("Using random variable: %d\r\n\r\n", random_variable);
    printf("TEST1\r\n");

    int8_t   i8 =  std::numeric_limits<int8_t>::max()   - static_cast<int8_t>(random_variable);
    uint8_t  u8 =  std::numeric_limits<uint8_t>::max()  - static_cast<uint8_t>(random_variable);
    int16_t  i16 = std::numeric_limits<int16_t>::max()  - static_cast<int16_t>(random_variable);
    uint16_t u16 = std::numeric_limits<uint16_t>::max() - static_cast<uint16_t>(random_variable);
    int32_t  i32 = std::numeric_limits<int32_t>::max()  - static_cast<int32_t>(random_variable);
    uint32_t u32 = std::numeric_limits<uint32_t>::max() - static_cast<uint32_t>(random_variable);
    int64_t  i64 = std::numeric_limits<int64_t>::max()  - static_cast<int64_t>(random_variable);
    uint64_t u64 = std::numeric_limits<uint64_t>::max() - static_cast<uint64_t>(random_variable);

    printf_nums(i8, u8, i16, u16, i32, u32, i64, u64);
    out_nums(i8, u8, i16, u16, i32, u32, i64, u64);

    printf("TEST2\r\n");
    i8 =  std::numeric_limits<int8_t>::min()   + static_cast<int8_t>(random_variable);
    u8 =  std::numeric_limits<uint8_t>::min()  + static_cast<uint8_t>(random_variable);
    i16 = std::numeric_limits<int16_t>::min()  + static_cast<int16_t>(random_variable);
    u16 = std::numeric_limits<uint16_t>::min() + static_cast<uint16_t>(random_variable);
    i32 = std::numeric_limits<int32_t>::min()  + static_cast<int32_t>(random_variable);
    u32 = std::numeric_limits<uint32_t>::min() + static_cast<uint32_t>(random_variable);
    i64 = std::numeric_limits<int64_t>::min()  + static_cast<int64_t>(random_variable);
    u64 = std::numeric_limits<uint64_t>::min() + static_cast<uint64_t>(random_variable);

    printf_nums(i8, u8, i16, u16, i32, u32, i64, u64);
    out_nums(i8, u8, i16, u16, i32, u32, i64, u64);

    char c = '!';
    bool b = true;

    out_bool_char(b, c);

    c = '?';
    b = false;

    out_bool_char(b, c);
}
