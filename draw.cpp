#include <fxcg/display.h>
#include "math.hpp"

void drawFKey(int id, int pos)
{
    void *bitmap;
    GetFKeyPtr(id, &bitmap);
    FKey_Display(pos, bitmap);
}

void plot(int x, int y, color_t color)
{
    color_t *VRAM = (color_t *)GetVRAMAddress();
    VRAM += y * LCD_WIDTH_PX + x;
    *VRAM = color;
}

void fillArea(unsigned x, unsigned y, unsigned w, unsigned h, color_t color)
{
    for (unsigned i = 0; i < w; i++)
    {
        for (unsigned j = 0; j < h; j++)
        {
            plot(x + i, y + j, color);
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

void plotLineAA(int x0, int y0, int x1, int y1, color_t color, color_t background)
{
    float dx = abs(x1 - x0);
    float dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    float x2, e2, err = dx - dy;
    float ed = dx + dy == 0.0f ? 1.0f : sqrt(dx * dx + dy * dy);
    int col_r, col_g, col_b, bg_r, bg_g, bg_b;
    getRGB(color, &col_r, &col_g, &col_b);
    getRGB(background, &bg_r, &bg_g, &bg_b);
    while (true)
    {
        plot(x0, y0, getAlpha(col_r, col_g, col_b, bg_r, bg_g, bg_b, abs(err - dx + dy) / ed));
        e2 = err;
        x2 = x0;
        if (2.0f * e2 >= -dx)
        {
            if ((int)x0 == (int)x1)
            {
                break;
            }
            if (e2 + dy < ed)
            {
                plot(x0, y0 + sy, getAlpha(col_r, col_g, col_b, bg_r, bg_g, bg_b, (e2 + dy) / ed));
            }
            err -= dy;
            x0 += sx;
        }
        if (2.0f * e2 <= dy)
        {
            if ((int)y0 == (int)y1)
            {
                break;
            }
            if (dx - e2 < ed)
            {
                plot(x2 + sx, y0, getAlpha(col_r, col_g, col_b, bg_r, bg_g, bg_b, (dx - e2) / ed));
            }
            err += dx;
            y0 += sy;
        }
    }
}

void plotCircleAA(int cx, int cy, int r, color_t color, color_t background)
{
    int x = r, y = 0;
    int x2, e2, err = 2 - 2 * r;
    float alpha;
    r = 1 - err;
    int col_r, col_g, col_b, bg_r, bg_g, bg_b;
    getRGB(color, &col_r, &col_g, &col_b);
    getRGB(background, &bg_r, &bg_g, &bg_b);
    while (true)
    {
        color = getAlpha(col_r, col_g, col_b, bg_r, bg_g, bg_b, abs(err + 2.0f * (x + y) - 2) / r);
        plot(cx + x, cy - y, color);
        plot(cx + y, cy + x, color);
        plot(cx - x, cy + y, color);
        plot(cx - y, cy - x, color);
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
                plot(cx + x, cy - y + 1, color);
                plot(cx + y - 1, cy + x, color);
                plot(cx - x, cy + y - 1, color);
                plot(cx - y + 1.0f, cy - x, color);
            }
            err -= --x * 2 - 1;
        }
        if (e2 <= x2--)
        {
            alpha = (1.0f - 2 * y - e2) / r;
            if (alpha < 1.0f)
            {
                color = getAlpha(col_r, col_g, col_b, bg_r, bg_g, bg_b, alpha);
                plot(cx + x2, cy - y, color);
                plot(cx + y, cy + x2, color);
                plot(cx - x2, cy + y, color);
                plot(cx - y, cy - x2, color);
            }
            err -= --y * 2 - 1;
        }
    }
}
