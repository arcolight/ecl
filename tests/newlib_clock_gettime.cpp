#include <ctime>

extern "C" int clock_gettime(clockid_t, struct timespec* ts)
{
    std::clock_t c = std::clock();
    ts->tv_sec  = c / CLOCKS_PER_SEC;
    ts->tv_nsec = (c % CLOCKS_PER_SEC) * (1000000000L / CLOCKS_PER_SEC);
    return 0;
}
