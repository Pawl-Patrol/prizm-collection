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

void plotLineAA(Vector p1, Vector p2, color_t color, color_t background, bool bounded)
{
    float dx = fabs(p2.x - p1.x);
    float dy = fabs(p2.y - p1.y);
    int sx = p1.x < p2.x ? 1 : -1;
    int sy = p1.y < p2.y ? 1 : -1;
    float x2, e2, err = dx - dy;
    float ed = dx + dy == 0.0f ? 1.0f : sqrt(dx * dx + dy * dy);
    int col_r, col_g, col_b, bg_r, bg_g, bg_b;
    getRGB(color, &col_r, &col_g, &col_b);
    getRGB(background, &bg_r, &bg_g, &bg_b);
    while (true)
    {
        plot(p1.x, p1.y, getAlpha(col_r, col_g, col_b, bg_r, bg_g, bg_b, fabs(err - dx + dy) / ed), bounded);
        e2 = err;
        x2 = p1.x;
        if (2.0f * e2 >= -dx)
        {
            if ((int)p1.x == (int)p2.x)
            {
                break;
            }
            if (e2 + dy < ed)
            {
                plot(p1.x, p1.y + sy, getAlpha(col_r, col_g, col_b, bg_r, bg_g, bg_b, (e2 + dy) / ed), bounded);
            }
            err -= dy;
            p1.x += sx;
        }
        if (2.0f * e2 <= dy)
        {
            if ((int)p1.y == (int)p2.y)
            {
                break;
            }
            if (dx - e2 < ed)
            {
                plot(x2 + sx, p1.y, getAlpha(col_r, col_g, col_b, bg_r, bg_g, bg_b, (dx - e2) / ed), bounded);
            }
            err += dx;
            p1.y += sy;
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
