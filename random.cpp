#define MODULUS 0x100000000 // 2^32
#define MULTIPLIER 22695477
#define INCREMENT 1

class Random
{
    private:
        int seed;
    public:
        void init(int time);
        void next();
        int int32();
        short int16();
        int range(int min, int max);
        bool boolean();
};

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

int Random::sign()
{
    return 1 - 2 * boolean();
}
