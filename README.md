ecl
===

[![Build Status](https://scan.coverity.com/projects/8398/badge.svg)](https://scan.coverity.com/projects/8398)
[![CI](https://github.com/arcolight/ecl/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/arcolight/ecl/actions/workflows/ci.yml)
[![codecov.io](https://codecov.io/github/arcolight/ecl/coverage.svg?branch=master)](https://codecov.io/github/arcolight/ecl?branch=master)
[![MIT licensed](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/hyperium/hyper/master/LICENSE)

Very small C++ class set suitable for embedded devices.
C++11-compatible compiler needed. (Tested: gcc>=4.7, clang>=3.5)

### fsm
Generic class for Finite State Machine creating.
Influenced by boost::MSM, but more simple.

### bit_field
Class for serialization and deserialization of bitfield-like structures.
Main problem in bitfields is compiler-specific implementation and endianess problems.
bitfield class architecture independent. Of course, at cost of complexity.

### scope_guard
RAII idiom. Class provides mechanism to define execution of some code on scope exit.

### command processor & command
Simple classes suitable for composing command processors on embedded systems.
Commands can be arranged as tree.

### json
JSON serializer/deserializer.
JSON document specialized as template, than it can be serialized to ecl::stream,
std::stream or any other object that support operator<< and deserialized from C-style string (char *)

### stream
Simple implementation of stream. (iostream is too complex for embedded systems and uses dynamic memory)
Flush function can be passed as constructor arg. It will be called, when stream becomes full.

### singleton
Meyers singleton implementation.

### str_const
Scott Schurr's str_const class.

### circular_buffer
Circular buffer class.

### tree
Binary tree implementation. Can be used with statically allocated nodes, so no memory allocation needed.

### red_black_tree
Red-black tree implementation.

### avl_tree
AVL tree implementation.

### splay_tree
Splay tree implementation.

### map
Simple constant-size map class. Uses RB-tree for element access.

build
===

Targets:

    * all - all examples and tests
    * examples - all examples
    * tests - build tests (Boost.Test required)
    * check - build and run tests
    * check_arm - build tests with arm-none-eabi-g++ and run them in qemu-arm
    * examples_arm - build examples with arm-none-eabi-g++
    * gcov_tests - run tests and generate gcov reports
    * example_fsm
    * example_scope_guard
    * example_cb
    * example_bitfield
    * example_singleton
    * example_stream
    * example_json
    * example_str_const
    * example_tree
    * example_static_map
    * example_command_processor

gcc:

    make all

clang:

    CXX=clang++ make all

arm-none-eabi-gcc (cortex-m4, run with qemu-arm):

    make examples_arm
    make check_arm

tests:

    make check
    CXX=clang++ make check
    make check ADD_FLAGS="-fsanitize=address,undefined,float-cast-overflow"
