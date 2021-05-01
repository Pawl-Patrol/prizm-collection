#ifndef ATOF_HPP
#define ATOF_HPP

#define SIGN_MINUS '\x99'
#define SIGN_PLUS '\x89'

float pow10(int n)
{
    float ret = 1.0;
    float r = 10.0;
    if (n < 0)
    {
        n = -n;
        r = 0.1;
    }

    while (n)
    {
        if (n & 1)
        {
            ret *= r;
        }
        r *= r;
        n >>= 1;
    }
    return ret;
}

float atof(char *str)
{
    if (str == nullptr || *str == '\0')
    {
        return 0.0;
    }
    int sign = 1;
    float int_part = 0.0;
    float frac_part = 0.0;
    bool has_frac = false;
    bool has_exp = false;

    if (*str == SIGN_MINUS)
    {
        sign = -1;
        str++;
    }
    else if (*str == SIGN_PLUS)
    {
        str++;
    }
    while (*str != '\0')
    {
        if (*str >= '0' && *str <= '9')
        {
            int_part = int_part * 10 + (*str - '0');
        }
        else if (*str == '.')
        {
            has_frac = true;
            str++;
            break;
        }
        else if (*str == 'e' || *str == 'E')
        {
            has_exp = true;
            str++;
            break;
        }
        str++;
    }

    if (has_frac)
    {
        float frac_exp = 0.1;

        while (*str != '\0')
        {
            if (*str >= '0' && *str <= '9')
            {
                frac_part += frac_exp * (*str - '0');
                frac_exp *= 0.1;
            }
            else if (*str == 'e' || *str == 'E')
            {
                has_exp = true;
                str++;
                break;
            }
            str++;
        }
    }

    float exp_part = 1.0;
    if (*str != '\0' && has_exp)
    {
        int exp_sign = 1;
        if (*str == SIGN_MINUS)
        {
            exp_sign = -1;
            str++;
        }
        else if (*str == SIGN_PLUS)
        {
            str++;
        }

        int e = 0;
        while (*str != '\0')
        {
            if (*str >= '0' && *str <= '9')
            {
                e = e * 10 + *str - '0';
            }
            str++;
        }
        exp_part = pow10(exp_sign * e);
    }

    return sign * (int_part + frac_part) * exp_part;
}

#endif
