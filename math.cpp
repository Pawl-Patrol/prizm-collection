#define PI 3.141592653589793
#define PI2 6.283185307179586
#define TRIG_ITERATIONS 10

float max(float a, float b)
{
    return a > b ? a : b;
}

float min(float a, float b)
{
    return a < b ? a : b;
}

float abs(float x)
{
    return max(-x, x);
}

float floor(float x)
{
    return int(x);
}

float ceil(float x)
{
    return int(x + 0.5f);
}

float frac(float x)
{
    return x - floor(x);
}

float fmod(float a, float b)
{
    float c = frac(abs(a / b)) * abs(b);
    return (a < 0) ? -c : c;
}

float rad(float deg)
{
    return deg * PI / 180;
}

float deg(float rad)
{
    return rad * 180 / PI;
}

float sqrt(float x)
{
    unsigned int i = *(unsigned int *)&x;
    i += 127 << 23;
    i >>= 1;
    float x2 = *(float *)&i;
    x2 -= (x2 * x2 - x) / (2 * x2);
    return x2;
}

float rsqrt(float x)
{
    float xhalf = 0.5f * x;
    int i = *(int *)&x;
    i = 0x5f375a86 - (i >> 1);
    x = *(float *)&i;
    x = x * (1.5f - xhalf * x * x);
    return x;
}

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

float asin(float x)
{
    float negate = float(x < 0);
    x = abs(x);
    float ret = -0.0187293;
    ret *= x;
    ret += 0.0742610;
    ret *= x;
    ret -= 0.2121144;
    ret *= x;
    ret += 1.5707288;
    ret = PI * 0.5 - sqrt(1.0 - x) * ret;
    return ret - 2 * negate * ret;
}

float acos(float x)
{
    float negate = float(x < 0);
    x = abs(x);
    float ret = -0.0187293;
    ret = ret * x;
    ret = ret + 0.0742610;
    ret = ret * x;
    ret = ret - 0.2121144;
    ret = ret * x;
    ret = ret + 1.5707288;
    ret = ret * sqrt(1.0 - x);
    ret = ret - 2 * negate * ret;
    return negate * PI + ret;
}

float atan2(float y, float x)
{
    float t0, t1, t2, t3, t4;
    t3 = abs(x);
    t1 = abs(y);
    t0 = max(t3, t1);
    t1 = min(t3, t1);
    t3 = 1 / t0;
    t3 = t1 * t3;
    t4 = t3 * t3;
    t0 = -0.013480470;
    t0 = t0 * t4 + 0.057477314;
    t0 = t0 * t4 - 0.121239071;
    t0 = t0 * t4 + 0.195635925;
    t0 = t0 * t4 - 0.332994597;
    t0 = t0 * t4 + 0.999995630;
    t3 = t0 * t3;
    t3 = (abs(y) > abs(x)) ? 1.570796327 - t3 : t3;
    t3 = (x < 0) ? PI - t3 : t3;
    t3 = (y < 0) ? -t3 : t3;
    return t3;
}
