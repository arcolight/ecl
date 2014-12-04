#pragma once

#include <cstring>

#include <algorithm>

#define DEFAULT_HEADER_NAME_LEN  32
#define DEFAULT_HEADER_VALUE_LEN 32

namespace ecl
{

template<size_t NAME_MAX_LEN =  DEFAULT_HEADER_NAME_LEN,
         size_t VALUE_MAX_LEN = DEFAULT_HEADER_VALUE_LEN>
struct header
{
    header()
    {
        clear();
    }

    bool parse(const char* const raw, size_t size)
    {
        clear();

        const char* const separator_position = std::strchr(raw, ':');

        if(NULL == separator_position)
        {
            return false;
        }

        size_t name_len = separator_position - raw;
        size_t value_len = size - name_len - 1;

        const char* name_ptr = raw;
        while(*name_ptr == ' ' && name_len != 0)
        {
            ++name_ptr;
            --name_len;
        }

        const char* value_ptr = separator_position + 1;
        while(*value_ptr == ' ' && value_len != 0)
        {
            ++value_ptr;
            --value_len;
        }

        if((0 >= value_len) || (0 >= name_len))
        {
            return false;
        }

        strncpy(name,  name_ptr,  std::min(NAME_MAX_LEN,  name_len));
        strncpy(value, value_ptr, std::min(VALUE_MAX_LEN, value_len));

        name[NAME_MAX_LEN - 1] = 0x00;
        value[VALUE_MAX_LEN - 1] = 0x00;

        return true;
    }

    char name[NAME_MAX_LEN];
    char value[VALUE_MAX_LEN];

    void clear()
    {
        for(auto& c: name)
        {
            c = 0x00;
        }

        for(auto& c: value)
        {
            c = 0x00;
        }
    }
};

} // namespace ecl
