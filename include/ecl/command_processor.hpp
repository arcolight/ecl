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
    bool init(const std::size_t argc,
              const char**      argv)                                   noexcept
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
                                     m_argv[0],
                                     m_argc - 1,
                                     m_argv + 1);
    }

    constexpr static const char* name()         noexcept(noexcept(NAME::name()))
    {
        return NAME::name();
    }

private:
    template<typename ST, typename cmd, typename... tail>
    bool call(ST& st,
              const char* const nm,
              const std::size_t argc,
              const char**      argv)                                      const
    {
        static_assert((cmd::name() != nullptr), "cmd::name is empty!");

        if( (strlen(cmd::name()) == strlen(nm)) &&
            (0 == strcmp(cmd::name(), nm)) )
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
    bool call(ST& /* st */,
              const char* const /* nm */,
              const std::size_t /* argc */,
              const char** /* argv */)                            const noexcept
    {
//        help(st);

        return false;
    }

    template<typename ST, typename cmd, typename... tail>
    static void show_help_internal(ST& st,
                                   std::size_t indent,
                                   std::size_t indent_increment)       noexcept(
                          noexcept(cmd::show_help(st, indent, indent_increment))
                                                                               )
    {
        cmd::show_help(st, indent, indent_increment);

        show_help_internal<ST, tail...>(st, indent, indent_increment);
    }

    template<typename ST>
    static void show_help_internal(ST& /* st */,
                                   std::size_t /* indent */,
                                   std::size_t /* indent_increment */)  noexcept
    {
    }

    std::size_t  m_argc { 0 };
    const char** m_argv { nullptr };

public:
    template<typename ST>
    static void show_help(ST& st,
                          std::size_t indent,
                          std::size_t indent_inc = DEFAULT_INDENT_INCREMENT)
                        noexcept(
                            noexcept(print_indent(st, indent)) &&
                            noexcept(st.operator<<("")) &&
                            noexcept(show_help_internal<
                                        ST,
                                        commands...
                                     >(st,
                                       indent + indent_inc,
                                       indent_inc)
                            )
                        )
    {
        print_indent(st, indent);
        st << name() << "\n\r";

        show_help_internal<ST, commands...>(st,
                                            indent + indent_inc,
                                            indent_inc);
    }

    template<typename ST>
    static void help(ST& st, std::size_t indent_inc = DEFAULT_INDENT_INCREMENT)
                            noexcept(noexcept(show_help<ST>(st, 0, indent_inc)))
    {
        show_help<ST>(st, 0, indent_inc);
    }
};

} // namespace ecl

#endif // ECL_COMMAND_PROCESSOR_HPP
