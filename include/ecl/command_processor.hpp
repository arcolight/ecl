#ifndef ECL_COMMAND_PROCESSOR_HPP
#define ECL_COMMAND_PROCESSOR_HPP

#include "command.hpp"

#include <cstring>

namespace ecl
{

template<typename NAME, typename... commands>
class command_processor : public i_command
{
public:
    command_processor(const sized_data* const buf) :
        m_buf(buf)
    {}

    virtual bool init(const uint8_t argc,
                      const uint8_t* const* const argv)                 override
    {
        if(0 == argc) {
            return false;
        }

        m_argc = argc;
        m_argv = argv;

        return true;
    }

    virtual bool dispatch()                                             override
    {
        return call<0, commands...>((const char* const)m_argv[0],
                                    m_argc - 1,
                                    m_argv + 1);
    }

    constexpr static const char* const name()
    {
        return NAME::name();
    }

private:
    template<size_t COUNT, typename cmd, typename... tail>
    bool call(const char* const           nm,
              const uint8_t               argc,
              const uint8_t* const* const argv)                            const
    {
        static_assert((cmd::name() != nullptr), "cmd::name is empty!");

        if(0 == strncmp(cmd::name(), nm, strlen(cmd::name())))
        {
            cmd c(m_buf);

            i_command* p_cmd = static_cast<i_command*>(&c);

            if( ! p_cmd->init(argc, argv) ) {
                const char* const p = "Invalid syntax.";

                for(size_t i = 0; i < strlen(p); ++i) {
                    m_buf->ptr[i] = p[i];
                }

                return false;
            }

            return p_cmd->dispatch();
        }

        return call<COUNT + 1, tail...>(nm, argc, argv);
    }

    template<size_t COUNT>
    bool call(const char* const           nm,
              const uint8_t               argc,
              const uint8_t* const* const argv)                            const
    {
        (void)(nm);
        (void)(argc);
        (void)(argv);

        const char* const p = "Command not found.";

        for(size_t i = 0; i < strlen(p); ++i) {
            m_buf->ptr[i] = p[i];
        }

        return false;
    }

    uint8_t                 m_argc;
    const uint8_t* const*   m_argv;

    const sized_data* const m_buf;
};

} // namespace ecl

#endif // ECL_COMMAND_PROCESSOR_HPP
