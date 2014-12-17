#pragma once

#include <cstring>

namespace ecl
{

namespace web
{

template<typename... RESOURCES>
class resource_table : public RESOURCES...
{
public:
    template<typename T>
    bool call(T&                    stream,
              const char* const     name,
              int                   argc,
              const char*           argv[])
    {
        if(nullptr == name)
        {
            return false;
        }

        return call_internal<0, T, RESOURCES...>(stream, name, argc, argv);
    }

private:
    template<size_t COUNT, typename T, typename RES, typename... TAIL>
    bool call_internal(T&                    stream,
                       const char* const     name,
                       int                   argc,
                       const char*           argv[])
    {
        if(0 == strncmp(name, RES::name_t::name(), RES::name_t::size()))
        {
            this->RES::template exec<T>(stream, argc, argv);
            return true;
        }

        return call_internal<COUNT + 1, T, TAIL...>(stream, name, argc, argv);
    }

    template<size_t COUNT, typename T>
    bool call_internal(T&                stream,
                       const char* const name,
                       int               argc,
                       const char*       argv[])
    {
        (void)(stream);
        (void)(name);
        (void)(argc);
        (void)(argv);
        static_assert((COUNT == sizeof...(RESOURCES)), 
                      "Variadic template instantiation error!");

        return false;
    }
};

} // namespace web

} // namespace ecl
