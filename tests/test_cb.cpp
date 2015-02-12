#include <cstdint>
#include <iostream>

#include <ctime>
#include <cstdlib>
#include <limits>

#include <ecl/circular_buffer.hpp>

#define CB_SIZE 16

typedef ecl::circular_buffer<uint8_t, CB_SIZE> circular_buffer_t;

static circular_buffer_t cb;

static void dump(circular_buffer_t& c)
{
    std::cout << " * by index:\n\r[ ";
    for(size_t i = 0; i < CB_SIZE; ++i) {
        std::cout << (uint32_t)c[i] << " ";
    }
    std::cout << "]" << std::endl;

    std::cout << " * by iterator (C++11):" << std::endl << "[ ";

    for(auto e: c) {
        std::cout << (uint32_t)e << " ";
    }
    std::cout << "]" << std::endl;

    std::cout << " * by iterator (C++03):" << std::endl << "[ ";
    for(circular_buffer_t::iterator it = c.begin(); it != c.end(); ++it) {
        std::cout << (uint32_t)*it << " ";
    }
    std::cout << "]" << std::endl;

    std::cout << " * by reverse iterator (C++03):" << std::endl << "[ ";
    auto it  = c.rbegin();
    while(it != c.rend())
    {
        std::cout << (uint32_t)*it << " ";
        ++it;
    }

    // for(circular_buffer_t::reverse_iterator it = (++c.rbegin()); it != (++c.rend()); ++it) {
    //     std::cout << (uint32_t)*it << " ";
    // }
    std::cout << "]" << std::endl << std::endl;
}

int main(int argc, char* argv[])
{
    (void)(argc);
    (void)(argv);

    std::srand(std::time(0));
    int random_variable = std::rand();

    size_t count = random_variable % (CB_SIZE*2) + 1;

    std::cout << "CB size: " << CB_SIZE << std::endl;

    std::cout << " * push " << count << " values [1; " << count << "] <= ";

    for(uint8_t i = 1; i < count + 1; ++i) {
        std::cout << (uint32_t)i << " ";
        cb.push(i);
    }
    std::cout << "|" << std::endl;

    dump(cb);

    std::cout << " * pop" << std::endl;
    for(auto e: cb) {
        (void)(e);
        cb.pop();
    }

    dump(cb);

    std::cout << " * push " << count << " values [2; " << count*2 << "] <= ";
    for(uint8_t i = 1; i < count + 1; ++i) {
        std::cout << (uint32_t)(i*2) << " ";
        cb.push(i*2);
    }
    std::cout << "|" << std::endl;

    dump(cb);

    std::cout << " * increment all" << std::endl;
    for(auto& e: cb) {
        ++e;
    }

    dump(cb);
}
