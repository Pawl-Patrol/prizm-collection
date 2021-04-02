#define PI 3.141592653589793

float max(float a, float b)
{
    return a > b ? a : b;
}

float min(float a, float b)
{
    return a < b ? a : b;
}

float floor(float x)
{
    return (float)(int)x;
}

float ceil(float x)
{
    return floor(x + 0.5f);
}

float abs(float x)
{
    return max(x, -x);
}

float fmod(float a, int b)
{
    while ((int)a > b)
    {
        a -= (float)b;
    }
    return a;
}

float radians(float degrees)
{
    return degrees * PI / 180;
}

float degrees(float radians)
{
    return radians * 180 / PI;
}

unsigned int factorial(unsigned int x)
{
    if (x == 0)
    {
        return 1;
    }
    for (unsigned int i = x - 1; i > 1; i--)
    {
        x *= i;
    }
    return x;
}

float sqrt(float x, unsigned int iterations)
{
    if (x == 0)
    {
        return 0;
    }
    float guess = x;
    while (iterations--)
    {
        guess -= (guess * guess - x) / (2 * guess);
    }
    return guess;
}

float sin(float x, unsigned int iterations)
{
    if (x == 0)
    {
        return 0;
    }
    float x_sq = x;
    float result = x;
    float sign = 1;
    float fact = 1;
    x *= x;
    for (int n = 1; n < iterations; n++)
    {
        x_sq *= x;
        sign *= -1;
        fact *= 2 * n * (2 * n + 1);
        result += sign * x_sq / fact;
    }
    return result;
}

float cos(float x, unsigned int iterations)
{
    if (x == 0.0)
    {
        return 1;
    }
    x *= x;
    float x_sq = 1;
    float sign = 1;
    float result = 1;
    float fact = 1;
    for (int n = 1; n < iterations; n++)
    {
        x_sq *= x;
        sign *= -1;
        fact *= 2 * n * (2 * n - 1);
        result += sign * x_sq / fact;
    }
    return result;
}

float arcsin(float x, unsigned int iterations)
{
    if (x == 0.0)
    {
        return 0;
    }
    float result = x;
    float mult = x;
    float mfour = 1;
    float fact1 = 1;
    float fact2 = 1;
    x *= x;
    for (int n = 1; n < iterations; n++)
    {
        mfour *= 4;
        fact1 *= 2 * n * (2 * n - 1);
        fact2 *= n;
        mult *= x;
        result += mult * fact1 / (mfour * fact2 * fact2 * (2 * n + 1));
    }
    return result;
}

float arccos(float x, unsigned int iterations)
{
    if (x == 1.0)
    {
        return 0;
    }
    return PI / 2 - arcsin(x, iterations);
}
