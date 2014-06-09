#ifndef ECL_COMMAND_PROCESSOR_HPP
#define ECL_COMMAND_PROCESSOR_HPP

#include "command.hpp"

#include <string.h>

namespace ecl
{

template<typename... commands>
class command_processor : public i_command
{
public:
    command_processor(const uint8_t* const nm,
                      const data_t*  const buf) :
        name(nm),
        m_buf(buf)
    {}

    bool dispatch(const uint8_t argc, const uint8_t** const argv)          const
    {
        if(0 == argc) {
            return false;
        }

        return call<0, commands...>(argv[0], argc - 1, argv + 1);
    }

    virtual size_t run()                                                override
    {
        dispatch(m_argc, m_argv);

        return m_buf->size; // ?
    }

    virtual bool init(const uint8_t argc, const uint8_t** const argv)   override
    {
        m_argc = argc;
        m_argv = argv;

        return true;
    }

    const uint8_t* const name;

private:
    template<size_t COUNT>
    void check()                                                           const
    {}

    template<size_t COUNT, typename cmd, typename... tail>
    bool call(const uint8_t* const  nm,
              const uint8_t         argc,
              const uint8_t** const argv)                                  const
    {
        static_assert((cmd::name != nullptr), "cmd::name is nullptr!");

        if(0 == strncmp((const char*)(cmd::name),
                        (const char*)(nm),
                        strlen((const char*)cmd::name)))
        {
            cmd c(m_buf);

            // check for 'cmd' implements 'i_command' interface.
            // Not really necessary, we don't use polymorphism.
            static_cast<i_command*>(&c);

            if( ! c.init(argc, argv) ) {
                return false;
            }

            c.run();

            return true;
        }

        return call<COUNT + 1, tail...>(nm, argc, argv);
    }

    template<size_t COUNT>
    bool call(const uint8_t* const  nm,
              const uint8_t         argc,
              const uint8_t** const argv)                                  const
    {
        (void)(nm);
        (void)(argc);
        (void)(argv);

        const char* const p = "Command not found";

        for(size_t i = 0; i < strlen(p); ++i) {
            m_buf->ptr[i] = p[i];
        }

        return false;
    }

    uint8_t   m_argc;
    uint8_t** m_argv;

    const data_t* const m_buf;
};

} // namespace ecl

#endif // ECL_COMMAND_PROCESSOR_HPP
