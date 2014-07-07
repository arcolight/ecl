#include <stdio.h>

struct serial_out
{
    static bool print(const char* const string) {
        puts((const char* const)string);
        return true;
    }
};
