#include <fxcg/rtc.h>

#define MODULUS 0x100000000 // 2^32
#define MULTIPLIER 22695477
#define INCREMENT 1

int seed = RTC_GetTicks();

void next()
{
    seed = (MULTIPLIER * seed + INCREMENT) % MODULUS;
}

int int32()
{
    next();
    return seed;
}

short int16()
{
    next();
    return seed >> 16;
}

int range(int min, int max)
{
    return min + (unsigned short)int16() % (max + 1 - min);
}

bool boolean()
{
    return range(0, 1);
}

int sign()
{
    return 1 - 2 * boolean();
}
