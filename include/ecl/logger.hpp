#ifndef ECL_LOGGER_HPP
#define ECL_LOGGER_HPP

#include <ecl/stream.hpp>

namespace ecl
{

template<size_t BUFFER_SIZE, typename LEVELS, typename HEADER, typename... OUT>
class logger
{
private:
    static void stream_out(const char* const buf, size_t size)
    {
        out_all(buf, size);
    }

    typedef ecl::stream<BUFFER_SIZE, &logger::stream_out> stream_t;

public:
    logger(const LEVELS def_level) : m_level(def_level) {}

    logger& operator() (const LEVELS& l)
    {
        m_enabled = false;
        if(l >= m_level)
        {
            m_enabled = true;
            HEADER(l, m_stream);
        }

        return *this;
    }

    template<typename T>
    logger& operator<<(const T& obj)
    {
        if(m_enabled)
        {
            m_stream << obj;
        }

        return *this;
    }

    LEVELS set_level(LEVELS lvl)
    {
        LEVELS prev = m_level;
        m_level = lvl;

        return prev;
    }

    LEVELS get_level()
    {
        return m_level;
    }

private:
    static void out_all(const char* const buf, size_t size)
    {
        out<0, OUT...>(buf, size);
    }

    template<size_t COUNT, typename O, typename... TAIL>
    static void out(const char* const buf, size_t size)
    {
        O::print(buf, size);
        out<COUNT + 1, TAIL...>(buf, size);
    }

    template<size_t COUNT>
    static void out(const char* const buf, size_t size)
    {
        (void)buf;
        (void)size;
    }

    LEVELS   m_level;
    stream_t m_stream {};
    bool     m_enabled { false };
};

} // namespace ecl

#endif // ECL_LOGGER_HPP
