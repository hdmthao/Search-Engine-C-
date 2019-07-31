// Implement for time
#include "util.h"

clock_t util::time::timer::t = 0;

bool util::time::timer::SetTime() {
    t = clock();
    return true;
}

float util::time::timer::GetTimeInterval() {
    clock_t cur_time = clock() - t;
    return ((float)cur_time) / CLOCKS_PER_SEC;
}

long long util::time::timer::GetCurrentTime() {
    time_t timer;
    struct tm y2k = {0};

    std::time(&timer);
    y2k = *localtime(&timer);

    y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
     y2k.tm_mon = 0; y2k.tm_mday = 1;
    long long seconds = (long long)(difftime(timer,mktime(&y2k)));
    return seconds;
}