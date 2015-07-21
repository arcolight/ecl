enum lvl
{
    debug,
    info,
    warning,
    error,
    critical,
    always
};

struct lvl_to_string
{
    static const char* to_string(const lvl& l)
    {
        switch(l)
        {
            case lvl::critical:
                return "critical";
            break;
            case lvl::error:
                return "error   ";
            break;
            case lvl::warning:
                return "warning ";
            break;
            case lvl::info:
                return "info    ";
            break;
            case lvl::debug:
                return "debug   ";
            break;
            case lvl::always:
                return "always  ";
            break;
        }
    }
};

struct std_out
{
    static void print(const char* const buf, size_t size)
    {
        (void)size;
        printf("%s", buf);
    }
};

template<typename LEVELS, typename LEVELS_STRINGIFY>
struct std_header
{
    template<typename STREAM>
    std_header(const LEVELS& l, STREAM& st)
    {
        static uint64_t m_counter = 0;
        st << "[" << LEVELS_STRINGIFY::to_string(l) << "]" << " | " << ecl::width(16) << m_counter++ << " :";
    }
};
