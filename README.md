ecl
===

Very small C++ class set suitable for embedded devices. 
C++11-compatible compiler needed. (Tested: gcc-4.7, gcc-4.8, clang-3.5)

### FSM
Generic class for Finite State Machine creating.
Influenced by boost::MSM, but more simple.

### bitfield
Class for serialization and deserialization of bitfield-like structures.
Main problem in bitfields is compiler-specific implementation and ending problems.
bitfield class architecture independent. Of course, at cost of complexity.

### command processor & command
Simple classes suitable for composing command processors on embedded systems.
Commands can be arranged as tree.
No demo at now.

### json
JSON serializer.
JSON document specialized as template, than it can be serialized to ecl::stream

### stream
Simple implementation of stream. (iostream is too complex for embedded systems and uses dynamic memory)
Flush function can be specialized as template arg. It would be called, when stream becomes full.

### singleton
Meyers singleton implementation.

### web server
Simple web server with staticaly linked resources.
Simple CGI support.

No gcc-arm-none-eabi demo. For such demo we need FreeRTOS, for example, and lwIP.

tests
===

All tests can be compiled by gcc, clang, gcc-arm-none-eabi. Except web_server(no arm-none-eabi)
