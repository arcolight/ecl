#ifndef ECL_COMMAND_HPP
#define ECL_COMMAND_HPP

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <array>

#include "singleton.hpp"

#ifndef RECEIVER_CAPACITY
#define RECEIVER_CAPACITY 32
#endif

#ifndef DEFAULT_INDENT_INCREMENT
#define DEFAULT_INDENT_INCREMENT 4
#endif

namespace ecl
{

template<typename ST>
static void print_indent(ST& st, size_t indent)
{
    for(size_t i = 0; i < indent; ++i)
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
    virtual ~i_receiver() {}

public:
    virtual void receive(cmd&)                                              = 0;
};

template<typename cmd>
class cmd_core
{
protected:
    void reg(detail::i_receiver<cmd>* const i)
    {
        for(auto &itf: array_singleton::instance())
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
        std::size_t sz = array_singleton::instance().size();
        for(std::size_t i = 0; i < sz; ++i)
        {
            if(nullptr != array_singleton::instance()[i])
            {
                array_singleton::instance()[i]->receive(c);
            }
        }
    }

private:
    typedef ecl::singleton<
                std::array<detail::i_receiver<cmd>*, RECEIVER_CAPACITY> 
            > array_singleton;
};

} // namespace detail

template<typename NAME, typename cmd>
class command : public virtual detail::cmd_core<cmd>
{
public:
    bool dispatch()
    {
        this->execute(*static_cast<cmd*>(this));

        return true;
    }

    bool init(const uint8_t   argc,
              const uint8_t** argv)
    {
        m_argc = argc;
        m_argv = argv;

        return true;
    }

    constexpr static const char* name()
    {
        return NAME::name();
    }

    template<typename ST>
    static void show_help(ST& st, size_t indent, size_t indent_increment = DEFAULT_INDENT_INCREMENT)
    {
        (void)indent_increment;
        for(size_t i = 0; i < indent; ++i)
        {
            st << " ";
        }

        st << name() << "\n\r";
    }

protected:
    uint8_t         m_argc;
    const uint8_t** m_argv;
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

    virtual ~receiver() {}
};

} // namespace ecl

#endif // ECL_COMMAND_HPP
