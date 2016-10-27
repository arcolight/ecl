ecl
===

[![Build Status](https://scan.coverity.com/projects/8398/badge.svg)](https://scan.coverity.com/projects/8398)
[![Build Status](https://travis-ci.org/simkimsia/UtilityBehaviors.png)](https://travis-ci.org/arcolight/ecl)
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
    * tests - tests
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

arm-none-eabi-gcc:

    CXX=arm-none-eabi-g++ ADD_FLAGS=--specs=rdimon.specs make all
