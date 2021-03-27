#include "random.hpp"

void Random::init(int time)
{
    seed = time;
}

void Random::next()
{
    seed = (MULTIPLIER * seed + INCREMENT) % MODULUS;
}

int Random::int32()
{
    next();
    return seed;
}

short Random::int16()
{
    next();
    return seed >> 16;
}

int Random::range(int min, int max)
{
    return min + (unsigned short)int16() % (max + 1 - min);
}

bool Random::boolean()
{
    return range(0, 1);
}