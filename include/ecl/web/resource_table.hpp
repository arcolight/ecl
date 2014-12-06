#pragma once

#include <cstring>

#include <ecl/singleton.hpp>

#include <tuple>

namespace ecl
{

namespace web
{

template<typename... RESOURCES>
class resource_table
{
public:
    static const i_resource* lookup(const char* const name)              
    {
        if(nullptr == name)
        {
            return nullptr;
        }

        return lookup_internal<0, RESOURCES...>(name);
    }

private:
    template<size_t COUNT, typename RES, typename... TAIL>
    static const i_resource* lookup_internal(const char* const name)
    {
        if(0 == strncmp(name, RES::name(), strlen(RES::name())))
        {
            return &(std::get<COUNT>(resources_tuple_singleton::instance()));
        }

        return lookup_internal<COUNT + 1, TAIL...>(name);
    }

    template<size_t COUNT>
    static const i_resource* lookup_internal(const char* const name)
    {
        (void)(name);
        static_assert((COUNT == sizeof...(RESOURCES)), 
                      "Variadic template instantiation error!");

        return nullptr;
    }

    typedef ecl::singleton<std::tuple<RESOURCES...>> resources_tuple_singleton;
};

} // namespace web

} // namespace ecl
