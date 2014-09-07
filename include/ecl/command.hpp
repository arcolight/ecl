#ifndef ECL_COMMAND_HPP
#define ECL_COMMAND_HPP

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <array>

#include "sized_data.h"
#include "singleton.hpp"

#ifndef RECEIVER_CAPACITY
#define RECEIVER_CAPACITY 32
#endif

namespace ecl
{

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
        for(auto itf: array_singleton::instance())
        {
            if(nullptr != itf)
            {
                itf->receive(c);
            }
        }
    }

private:
    typedef ecl::singleton<
                std::array<detail::i_receiver<cmd>*, RECEIVER_CAPACITY> 
                > array_singleton;
};

} // namespace detail

class i_command
{
public:
    virtual bool init(const uint8_t               argc,
                      const uint8_t* const* const argv)                     = 0;

    virtual bool dispatch()                                                 = 0;

    virtual size_t append(const sized_data&)                                = 0;
    virtual size_t append(const char* const)                                = 0;
};

template<typename NAME, typename cmd>
class command : public virtual detail::cmd_core<cmd>,
                public i_command
{
public:
    explicit command(const sized_data* const buf) :
        m_buf(buf),
        m_size(0)
    {}

    virtual bool dispatch()                                             override
    {
        this->execute(*static_cast<cmd*>(this));

        return true;
    }

    virtual bool init(const uint8_t               argc,
                      const uint8_t* const* const argv)                 override
    {
        (void)(argc);
        (void)(argv);
        return true;
    }

    virtual size_t append(const sized_data& d)                          override
    {
        if(nullptr == m_buf)
        {
            return 0;
        }

        if( (nullptr == m_buf->ptr) || (nullptr == d.ptr) || (0 == d.size) )
        {
            return 0;
        }

        size_t remain = m_buf->size - m_size;
        size_t size_safe = (d.size < remain) ? d.size : remain;
        for(size_t i = 0; i < size_safe; ++i)
        {
            m_buf->ptr[m_size] = d.ptr[i];
            ++m_size;
        }

        return size_safe;
    }

    virtual size_t append(const char* const str)                        override
    {
        sized_data d = {
            (uint8_t* const)str,
            strlen(str)
        };

        return append(d);
    }

    constexpr static const char* const name()
    {
        return NAME::name();
    }

protected:
    uint8_t                 m_argc;
    const uint8_t**         m_argv;

    const sized_data* const m_buf;
    size_t                  m_size;
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
