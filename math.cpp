#define PI 3.141592653589793
#define PI2 6.283185307179586

struct Vector3
{
    float x, y, z;
};


#define floor(x) (int)f - (f < 0)
#define ceil(x) (int)f + (f > 0)
#define max(a, b) a > b ? a : b
#define min(a, b) a < b ? a : b
#define abs(x) max(x, -x)
#define frac(x) x - (int)x

float fmod(float a, float b)
{
    float c = frac(abs(a / b)) * abs(b);
    return (a < 0) ? -c : c;
}

#define SQRT_ITERATIONS 10
#define RSQRT_ITERATIONS 5

float sqrt(float x)
{
    unsigned int i = *(unsigned int *)&x;
    i += 127 << 23;
    i >>= 1;
    float x2 = *(float *)&i;
    for (int n = 1; n < SQRT_ITERATIONS; n++)
    {
        x2 -= (x2 * x2 - x) / (2 * x2);
    }
    return x2;
}

float rsqrt(float x)
{
    float xhalf = 0.5f * x;
    int i = *(int *)&x;
    i = 0x5f375a86 - (i >> 1);
    x = *(float *)&i;
    for (int n = 1; n < RSQRT_ITERATIONS; n++)
    {
        x *= 1.5f - xhalf * x * x;
    }
    return x;
}

float log2(float x)
{
    x = (float)*(long *)&x;
    x *= 1.1920928955078125e-7f;
    return x - 126.94269504f;
}

#define RLD10 0.30103 // 1 / log2(10)

float log10(float x)
{
    return log2(x) * RLD10;
}

#define PI 3.141592653589793f
#define PI2 6.283185307179586f
#define ONEOVERTWOPI 0.159154943091895f
#define PIOVERTWO 1.570796326794896f

#define rad(x) x * PI / 180
#define deg(x) x * 180 / PI

float sin(float x)
{
    x = fmod(x, PI2);
    float x_sq = x;
    float result = x;
    float sign = 1;
    float fact = 1;
    x *= x;
    for (int n = 1; n < TRIG_ITERATIONS; n++)
    {
        x_sq *= x;
        sign *= -1;
        fact *= 2 * n * (2 * n + 1);
        result += sign * x_sq / fact;
    }
    return result;
}

float cos(float x)
{
    x = fmod(x, PI2);
    x *= x;
    float x_sq = 1;
    float sign = 1;
    float result = 1;
    float fact = 1;
    for (int n = 1; n < TRIG_ITERATIONS; n++)
    {
        x_sq *= x;
        sign *= -1;
        fact *= 2 * n * (2 * n - 1);
        result += sign * x_sq / fact;
    }
    return result;
}

#define tan(x) sin(x) / cos(x)
