#ifndef ECL_COMMAND_PROCESSOR_HPP
#define ECL_COMMAND_PROCESSOR_HPP

#include <ecl/command.hpp>

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

    template<typename ST>
    bool dispatch(ST& st)
    {
        return call<ST, commands...>(st,
                                     (const char* const)m_argv[0],
                                     m_argc - 1,
                                     m_argv + 1);
    }

    constexpr static const char* name()
    {
        return NAME::name();
    }

    template<typename ST>
    static void help(ST& st, size_t indent_increment = DEFAULT_INDENT_INCREMENT)
    {
        show_help<ST>(st, 0, indent_increment);
    }

    template<typename ST>
    static void show_help(ST& st, size_t indent, size_t indent_increment = DEFAULT_INDENT_INCREMENT)
    {
        print_indent(st, indent);
        st << name() << "\n\r";

        show_help_internal<ST, commands...>(st, indent + indent_increment, indent_increment);
    }

private:
    template<typename ST, typename cmd, typename... tail>
    bool call(ST& st,
              const char* const  nm,
              const uint8_t      argc,
              const uint8_t**    argv)                                     const
    {
        static_assert((cmd::name() != nullptr), "cmd::name is empty!");

        if((strlen(cmd::name()) == strlen(nm)) && (0 == strcmp(cmd::name(), nm)))
        {
            cmd c;

            if( ! c.init(argc, argv) )
            {
                c.show_help(st, 0);
                return false;
            }

            return c.dispatch(st);
        }

        return call<ST, tail...>(st, nm, argc, argv);
    }

    template<typename ST>
    bool call(ST& st,
              const char* const nm,
              const uint8_t     argc,
              const uint8_t**   argv)                                      const
    {
        (void)(st);
        (void)(nm);
        (void)(argc);
        (void)(argv);

//        help(st);

        return false;
    }

    template<typename ST, typename cmd, typename... tail>
    static void show_help_internal(ST& st, size_t indent, size_t indent_increment)
    {
        cmd::show_help(st, indent, indent_increment);

        show_help_internal<ST, tail...>(st, indent, indent_increment);
    }

    template<typename ST>
    static void show_help_internal(ST& st, size_t indent, size_t indent_increment)
    {
        (void)st;
        (void)indent;
        (void)indent_increment;
    }

    uint8_t         m_argc;
    const uint8_t** m_argv;
};

} // namespace ecl

#endif // ECL_COMMAND_PROCESSOR_HPP
