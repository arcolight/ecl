#include <stdint.h>
#include <stdio.h>

#include <ctime>
#include <cstdlib>
#include <limits>

#include <ecl/circular_buffer.hpp>

#define CB_SIZE 16

typedef ecl::circular_buffer<uint8_t, CB_SIZE> circular_buffer_t;

static circular_buffer_t cb(0);

static void dump(circular_buffer_t& c)
{
    printf(" * by index:\n\r[ ");
    for(size_t i = 0; i < CB_SIZE; ++i) {
        printf("%u ",c[i]);
    }
    printf("]\n\r");

    printf(" * by iterator (C++11):\n\r[ ");
    for(auto e: c) {
        printf("%u ", e);
    }
    printf("]\n\r");

    printf(" * by iterator (C++03):\n\r[ ");
    circular_buffer_t::iterator it_begin = c.begin();
    circular_buffer_t::iterator it_end = c.end();
    circular_buffer_t::iterator it = it_begin;

    for(it = it_begin; it != it_end; ++it) {
        printf("%u ", *it);
    }
    printf("]\n\r\n\r");
}

int main(int argc, char* argv[])
{
    (void)(argc);
    (void)(argv);

    std::srand(std::time(0));
    int random_variable = std::rand();

    size_t count = random_variable % (CB_SIZE*2) + 1;

    printf(" * push %lu values\n\r", count);
    for(uint8_t i = 1; i < count; ++i) {
        cb.push(i);
    }

    dump(cb);

    printf(" * pop\n\r");
    for(auto e: cb) {
        (void)(e);
        cb.pop();
    }

    dump(cb);

    printf(" * push %lu values\n\r", count);
    for(uint8_t i = 1; i < count; ++i) {
        cb.push(i*2);
    }

    dump(cb);

    printf(" * increment all\n\r");
    for(auto& e: cb) {
        ++e;
    }

    dump(cb);
}