#define PI 3.141592653589793f
#define TRIG_ITERATIONS 15
#define SQRT_ITERATIONS 5
#define tan(x) sin(x) / cos(x)

struct Vector3
{
    float x, y, z;
};

float _floor(float f) {return (int)f - (f < 0);}
float _ceil(float f) {return (int)f + (f > 0);}

float _max(float a, float b) {return a > b ? a : b;}
float _min(float a, float b) {return a < b ? a : b;}

float _abs(float x) {return _max(-x, x);}
float _frac(float x) {return x - _floor(x);}
float _fmod(float a, float b) {return a - (int)(a / b) * b;}

float _sqrt(float x)
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

float _rsqrt(float x)
{
    float xhalf = 0.5f * x;
    int i = *(int *)&x;
    i = 0x5f375a86 - (i >> 1);
    x = *(float *)&i;
    for (int n = 1; n < SQRT_ITERATIONS; n++)
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

const float radhelper = PI / 180;
float _rad(float x) {return x * radhelper;}
const float deghelper = 180 / PI;
float _deg(float x) {return x * deghelper;}

float _sin(float x)
{
    x = _fmod(x, 2 * PI);
    double x_sq = x;
    double result = x;
    double sign = 1;
    double fact = 1;
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

float _cos(float x)
{
    x = _fmod(x, 2 * PI);
    double x_x = x * x;
    double x_sq = 1;
    double sign = 1;
    double result = 1;
    double fact = 1;
    for (int n = 1; n < TRIG_ITERATIONS; n++)
    {
        x_sq *= x;
        sign *= -1;
        fact *= 2 * n * (2 * n - 1);
        result += sign * x_sq / fact;
    }
    return result;
}

float _asin(float x)
{
    double result = x;
    double mult = x;
    double mfour = 1;
    double fact1 = 1;
    double fact2 = 1;
    x *= x;
    for (int n = 1; n < TRIG_ITERATIONS; n++)
    {
        mfour *= 4;
        fact1 *= 2 * n * (2 * n - 1);
        fact2 *= n;
        mult *= x;
        result += mult * fact1 / (mfour * fact2 * fact2 * (2 * n + 1));
    }
    return result;
}

float _acos(float x)
{
    return PI * 0.5 - _asin(x);
}
