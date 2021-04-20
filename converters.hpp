float atof(const char *buffer)
{
    float fact = 1.0f;
    if (*buffer == '-' || *buffer == '\x99')
    {
        buffer++;
        fact = -1.0f;
    }
    float ret = 0.0f;
    for (bool point = false; *buffer; buffer++)
    {
        if (*buffer == '.')
        {
            point = true;
            continue;
        }
        int d = *buffer - '0';
        if (d >= 0 && d <= 9)
        {
            if (point)
            {
                fact /= 10.0f;
            }
            ret = ret * 10.0f + (float)d;
        }
    }
    return ret * fact;
}

void ftoa(float f, unsigned char *buffer, int maxchars)
{
    int m = f ? log10(abs(f)) : 0;
    bool neg = f < 0;
    if (m + 1 > maxchars - neg || -m + 1 > maxchars - neg - 2) // digits > maxchars - sign - '0' - '.'
    {
        float k = 0.1f;
        if (m < 0)
        {
            k = 10.0f;
            m = -m;
        }
        for (int i = 0; i < m; i++)
        {
            f *= k;
        }
        ftoa(f, buffer, maxchars - 2 - (int)log10(m)); // maxchars - 'e' - digits of m
        while (*++buffer != '\0');
        *buffer++ = 'e';
        itoa(m, buffer);
    }
    else
    {
        if (!(int)f && neg)
        {
            *buffer++ = '-';
        }
        itoa((int)f, buffer);
        while (*++buffer != '\0');
        f = abs((f - (int)f));
        int precision = maxchars - (max(m, 0)) - 2 - neg; // maxchars - digits - '.' - sign
        for (int i = 0; i < precision; i++)
        {
            f *= 10.0f;
        }
        int fraction = f + 0.5f;
        if (fraction)
        {
            *buffer++ = '.';
            buffer += precision;
            *buffer-- = '\0';
            char val;
            while (*buffer != '.')
            {
                val = '0' + fraction % 10;
                if (val == '0' && *(buffer + 1) == '\0')
                {
                    val = '\0';
                }
                *buffer-- = val;
                fraction *= 0.1;
            }
        }
    }
}
