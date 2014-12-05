#pragma once

#include <tuple>

#include "i_resource_container.hpp"

namespace ecl
{

namespace web
{

template<typename... RESOURCES>
class resource_table : public i_resource_container
{
public:
    virtual ~resource_table() {}

    virtual const i_resource* lookup(const char* const name)      const override
    {
        if(nullptr == name)
        {
            return nullptr;
        }

        return lookup_internal<0, RESOURCES...>(name);
    }

private:
    template<size_t COUNT, typename RES, typename... TAIL>
    const i_resource* lookup_internal(const char* const name)              const
    {
        if(name == RES::name())
        {
            return &(std::get<COUNT>(m_res));
        }

        return lookup_internal<COUNT + 1, TAIL...>(name);
    }

    template<size_t COUNT>
    const i_resource* lookup_internal(const char* const name)              const
    {
        (void)(name);
        static_assert((COUNT == sizeof...(RESOURCES)), 
                      "Variadic template instantiation error!");

        return nullptr;
    }

    const std::tuple<RESOURCES...> m_res;
};

} // namespace web

} // namespace ecl
