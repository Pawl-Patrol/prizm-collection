#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <fxcg/rtc.h>

int RandInt(int lower, int upper)
{
    static int a = 123456789;
    static int b = 362436069;
    static int c = 521288629;
    static int d = RTC_GetTicks();
    int t;

    t = a ^ (a << 11);
    a = b;
    b = c;
    c = d;
    return (d = d ^ (d >> 19) ^ (t ^ (t >> 8))) % upper + lower;
}

bool RandBool()
{
    return RandInt(0, 1);
}

char sign()
{
    return 1 - (RandBool() << 1);
}

#endif
