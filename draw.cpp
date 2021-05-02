#include <fxcg/display.h>

void plot(int x, int y, color_t color, bool bounded)
{
    if (bounded)
    {
        x = (x + LCD_WIDTH_PX) % LCD_WIDTH_PX;
        y = (y + LCD_HEIGHT_PX) % LCD_HEIGHT_PX;
    }
    else if (x < 0 || x >= LCD_WIDTH_PX || y < 0 || y >= LCD_HEIGHT_PX)
    {
        return;
    }
    color_t *VRAM = (color_t *)GetVRAMAddress();
    VRAM += y * LCD_WIDTH_PX + x;
    *VRAM = color;
}

void fillArea(unsigned x, unsigned y, unsigned w, unsigned h, color_t color, bool bounded)
{
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            plot(x + i, y + h, color, bounded);
        }
    }
}

void getRGB(color_t color, int *r, int *g, int *b)
{
    *r = color >> 11;
    *g = color >> 5 & 0b111111;
    *b = color & 0b11111;
}

color_t getAlpha(int col_r, int col_g, int col_b, int bg_r, int bg_g, int bg_b, float alpha)
{
    col_r = col_r - (alpha * col_r - alpha * bg_r);
    col_g = col_g - (alpha * col_g - alpha * bg_g);
    col_b = col_b - (alpha * col_b - alpha * bg_b);
    return col_r << 11 | col_g << 5 | col_b;
}


void plotLineAA(int x0, int y0, int x1, int y1, color_t color)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int x2, e2, err = dx - dy;
    int ed = dx + dy == 0 ? 1 : sqrt((float)dx * dx + (float)dy * dy);
    int r, g, b;
    getRGB(color, &r, &g, &b);
    while (true)
    {
        plot(x0, y0, getColorAlpha(r, g, b, (float)abs(err - dx + dy) / ed));
        e2 = err;
        x2 = x0;
        if (2 * e2 >= -dx)
        {
            if (x0 == x1)
                break;
            if (e2 + dy < ed)
                plot(x0, y0 + sy, getColorAlpha(r, g, b, (float)(e2 + dy) / ed));
            err -= dy;
            x0 += sx;
        }
        if (2 * e2 <= dy)
        {
            if (y0 == y1)
                break;
            if (dx - e2 < ed)
                plot(x2 + sx, y0, getColorAlpha(r, g, b, (float)(dx - e2) / ed));
            err += dx;
            y0 += sy;
        }
    }
}

void plotCircleAA(Vector center, int r, color_t color, color_t background, bool bounded)
{
    int x = r, y = 0;
    int x2, e2, err = 2 - 2 * r;
    float alpha;
    int col_r, col_g, col_b, bg_r, bg_g, bg_b;
    r = 1 - err;
    getRGB(color, &col_r, &col_g, &col_b);
    getRGB(background, &bg_r, &bg_g, &bg_b);
    while (true)
    {
        color = getAlpha(col_r, col_g, col_b, bg_r, bg_g, bg_b, fabs(err + 2.0f * (x + y) - 2) / r);
        plot(center.x + x, center.y - y, color, bounded);
        plot(center.x + y, center.y + x, color, bounded);
        plot(center.x - x, center.y + y, color, bounded);
        plot(center.x - y, center.y - x, color, bounded);
        if (x == 0)
        {
            break;
        }
        e2 = err;
        x2 = x;
        if (err > y)
        {
            alpha = (err + 2.0f * x - 1) / r;
            if (alpha < 1.0f)
            {
                color = getAlpha(col_r, col_g, col_b, bg_r, bg_g, bg_b, alpha);
                plot(center.x + x, center.y - y + 1, color, bounded);
                plot(center.x + y - 1, center.y + x, color, bounded);
                plot(center.x - x, center.y + y - 1, color, bounded);
                plot(center.x - y + 1.0f, center.y - x, color, bounded);
            }
            err -= --x * 2 - 1;
        }
        if (e2 <= x2--)
        {
            alpha = (1.0f - 2 * y - e2) / r;
            if (alpha < 1.0f)
            {
                color = getAlpha(col_r, col_g, col_b, bg_r, bg_g, bg_b, alpha);
                plot(center.x + x2, center.y - y, color, bounded);
                plot(center.x + y, center.y + x2, color, bounded);
                plot(center.x - x2, center.y + y, color, bounded);
                plot(center.x - y, center.y - x2, color, bounded);
            }
            err -= --y * 2 - 1;
        }
    }
}
