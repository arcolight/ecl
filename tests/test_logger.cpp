#include <ecl/logger.hpp>
#include "logger_hdr_out_example.hpp"

typedef ecl::logger<2048, lvl, std_header<lvl, lvl_to_string>, std_out> logger_t;

lvl switch_level(lvl l)
{
    switch(l)
    {
        case lvl::debug:
            return lvl::info;
        break;
        case lvl::info:
            return lvl::warning;
        break;
        case lvl::warning:
            return lvl::error;
        break;
        case lvl::error:
            return lvl::critical;
        break;
        case lvl::critical:
            return lvl::debug;
        break;
        default:
            return lvl::debug;
        break;
    }
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    lvl cur_l = lvl::debug;
    logger_t lg(cur_l);

    for(size_t i = 0; i < 128; ++i)
    {
        lg(lvl::always)   << "=====================\n"   << ecl::end();
        lg(lvl::always)   << "Log level -> " << lvl_to_string::to_string(lg.get_level()) << "\n" << ecl::end();
        lg(lvl::always)   << "========START========\n"   << ecl::end();
        lg(lvl::debug)    << "Debug message.\n"          << ecl::end();
        lg(lvl::info)     << "Info message.\n"           << ecl::end();
        lg(lvl::warning)  << "Warning message.\n"        << ecl::end();
        lg(lvl::error)    << "Error message.\n"          << ecl::end();
        lg(lvl::critical) << "Critical message.\n"       << ecl::end();
        lg(lvl::always)   << "=========END=========\n\n" << ecl::end();
        lg.set_level(switch_level(lg.get_level()));
    }

    return 0;
}