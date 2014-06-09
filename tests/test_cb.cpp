#include <stdint.h>
#include <stdio.h>

#include <ctime>
#include <cstdlib>
#include <limits>

#include "cb.hpp"

#define CB_SIZE 16

typedef ecl::cb<uint8_t, CB_SIZE> cb_t;

static cb_t cb(0);

static void dump(cb_t& c)
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
    cb_t::iterator it_begin = c.begin();
    cb_t::iterator it_end = c.end();
    cb_t::iterator it = it_begin;

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