color_t colorHue(int hue)
{
    hue %= 360;
    unsigned char r, g, b;
    if (hue < 60)
    {
        r = 0b11111;
        g = hue % 60 * 0b111111 / 60;
        b = 0b0;
    }
    else if (hue < 120)
    {
        r = 0b11111 - hue % 60 * 0b11111 / 60;
        g = 0b111111;
        b = 0b0;
    }
    else if (hue < 180)
    {
        r = 0b0;
        g = 0b111111;
        b = hue % 60 * 0b11111 / 60;
    }
    else if (hue < 240)
    {
        r = 0b0;
        g = 0b111111 - hue % 60 * 0b111111 / 60;
        b = 0b11111;
    }
    else if (hue < 300)
    {
        r = hue % 60 * 0b11111 / 60;
        g = 0b0;
        b = 0b11111;
    }
    else
    {
        r = 0b11111;
        g = 0b0;
        b = 0b11111 - hue % 60 * 0b11111 / 60;
    }
    return r << 11 | g << 5 | b;
}
