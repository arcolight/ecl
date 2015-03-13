#ifndef ECL_COMMAND_PROCESSOR_HPP
#define ECL_COMMAND_PROCESSOR_HPP

#include "command.hpp"

#include <cstring>

namespace ecl
{

template<typename NAME, typename... commands>
class command_processor
{
public:
    bool init(const uint8_t   argc,
              const uint8_t** argv)
    {
        if(0 == argc)
        {
            return false;
        }

        m_argc = argc;
        m_argv = argv;

        return true;
    }

    bool dispatch()
    {
        return call<0, commands...>((const char* const)m_argv[0],
                                    m_argc - 1,
                                    m_argv + 1);
    }

    constexpr static const char* name()
    {
        return NAME::name();
    }

private:
    template<size_t COUNT, typename cmd, typename... tail>
    bool call(const char* const  nm,
              const uint8_t      argc,
              const uint8_t**    argv)                                     const
    {
        static_assert((cmd::name() != nullptr), "cmd::name is empty!");

        if(0 == strncmp(cmd::name(), nm, strlen(cmd::name())))
        {
            cmd c;

            if( ! c.init(argc, argv) )
            {
                return false;
            }

            return c.dispatch();
        }

        return call<COUNT + 1, tail...>(nm, argc, argv);
    }

    template<size_t COUNT>
    bool call(const char* const nm,
              const uint8_t     argc,
              const uint8_t**   argv)                                      const
    {
        (void)(nm);
        (void)(argc);
        (void)(argv);

        return false;
    }

    uint8_t         m_argc;
    const uint8_t** m_argv;
};

} // namespace ecl

#endif // ECL_COMMAND_PROCESSOR_HPP
