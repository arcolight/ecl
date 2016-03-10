#ifndef ECL_COMMAND_HPP
#define ECL_COMMAND_HPP

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <array>
#include <type_traits>

#include <ecl/singleton.hpp>

#ifndef RECEIVER_CAPACITY
#define RECEIVER_CAPACITY 32
// #pragma message ("[ECL command.hpp] Defaulting to receiver capacity = " )
#endif

#ifndef DEFAULT_INDENT_INCREMENT
#define DEFAULT_INDENT_INCREMENT 4
// #pragma message "[ECL command.hpp] Defaulting to indent increment = 4"
#endif

namespace ecl
{

template<typename ST>
static void print_indent(ST& st, std::size_t indent)                    noexcept
{
    for(std::size_t i = 0; i < indent; ++i)
    {
        st << " ";
    }
}

namespace detail
{

template<typename cmd>
class i_receiver
{
protected:
    virtual ~i_receiver()
    {}

public:
    virtual void receive(cmd&)                                              = 0;
};

template<typename cmd>
class cmd_core
{
private:
    using receiver_t        = detail::i_receiver<cmd>;
    using receiver_ptr_t    = typename std::add_pointer<receiver_t>::type;
    using array_singleton_t = ecl::singleton
    <
        std::array<receiver_ptr_t, RECEIVER_CAPACITY>
    >;

protected:
    virtual ~cmd_core() {}

    void reg(receiver_ptr_t const i)                                   noexcept(
                                         noexcept(array_singleton_t::instance())
                                                                               )
    {
        for(auto &itf: array_singleton_t::instance())
        {
            if(nullptr == itf)
            {
                itf = i;
                return;
            }
        }
    }

    void execute(cmd& c)                                                   const
    {
        std::size_t sz = array_singleton_t::instance().size();
        for(std::size_t i = 0; i < sz; ++i)
        {
            if(nullptr != array_singleton_t::instance()[i])
            {
                array_singleton_t::instance()[i]->receive(c);
            }
        }
    }
};

} // namespace detail

template<typename NAME, typename cmd>
class command : public virtual detail::cmd_core<cmd>
{
public:
    template<typename ST>
    bool dispatch(ST& /* st */)
    {
        this->execute(*static_cast<cmd*>(this));

        return true;
    }

    bool init(const std::size_t argc,
              const char**      argv)
    {
        m_argc = argc;
        m_argv = argv;

        return true;
    }

    constexpr static const char* name()         noexcept(noexcept(NAME::name()))
    {
        return NAME::name();
    }

    template<typename ST>
    static void show_help(ST& st,
                          std::size_t indent,
                          std::size_t indent_inc = DEFAULT_INDENT_INCREMENT)
                                              noexcept(
                                                  noexcept(
                                                      print_indent(st,
                                                                   indent)) &&
                                                  noexcept(st.operator<<("")) &&
                                                  noexcept(name())
                                              )
    {
        (void)indent_inc;
        print_indent(st, indent);
        st << name() << "\n\r";
    }

protected:
    std::size_t  m_argc { 0 };
    const char** m_argv { nullptr };
};

template<typename cmd>
class receiver : public virtual detail::cmd_core<cmd>,
                 public detail::i_receiver<cmd>
{
protected:
    receiver()
    {
        this->detail::cmd_core<cmd>::reg(this);
    }

    virtual ~receiver()                                                 override
    {}
};

} // namespace ecl

#endif // ECL_COMMAND_HPP
