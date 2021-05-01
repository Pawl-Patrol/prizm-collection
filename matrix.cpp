#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/heap.h>

#include "draw.hpp"
#include "atof.hpp"
#include "ftoa.hpp"

#define STATUS_AREA_HEIGHT 24
#define FONT_WIDTH 12
#define FONT_HEIGHT 18

#define FIELD_CHARS 7
#define FIELD_SPACING 3

#define INPUT_BUFFER 25
#define INPUT_POS_X 1
#define INPUT_POS_Y 7

class Matrix
{
    private:
        int offset[2];
        int size[2];
        float *buffer;
        void drawField(int x, int y, bool selected)
        {
            char buffer[FIELD_CHARS + 1];
            float f = *getFloat(x, y);
            ftoa(f, buffer, (f >= 0.0f) + (f >= 1.0f) + (f < pow10(10) && f * pow10(10) > 1.0f));
            int width = FONT_WIDTH * FIELD_CHARS;
            x = offset[0] + x * (width + FIELD_SPACING);
            y = offset[1] + y * (FONT_HEIGHT + FIELD_SPACING);
            fillArea(x, STATUS_AREA_HEIGHT + y, width, FONT_HEIGHT, selected ? COLOR_BLACK : COLOR_WHITE);
            PrintMini(&x, &y, buffer, 0x02, x + width, 0, 0, selected ? COLOR_WHITE : COLOR_BLACK, 0, true, 0);
        }
        float getInput(const int x, const int y, int *key)
        {
            char buffer[INPUT_BUFFER];
            *buffer = '\0';
            int cursor = 0, start = 0;
            while (true)
            {
                if (*key && *key < 30000)
                {
                    cursor = EditMBStringChar((unsigned char *)buffer, INPUT_BUFFER, cursor, *key);
                    DisplayMBString((unsigned char *)buffer, start, cursor, x, y);
                }
                else
                {
                    EditMBStringCtrl((unsigned char *)buffer, INPUT_BUFFER, &start, &cursor, key, x, y);
                }
                GetKey(key);
                if (*key == KEY_CTRL_EXE || *key == KEY_CTRL_EXIT)
                {
                    break;
                }
            }
            DisplayMBString((unsigned char *)"", 0, 0, x, y);
            Cursor_SetFlashOff();
            return atof(buffer);
        }
    public:
        Matrix(int x, int y, int w, int h)
        {
            offset[0] = x;
            offset[1] = y;
            size[0] = w;
            size[1] = h;
            buffer = (float *)sys_malloc(sizeof(float) * w * h);
            for (int i = 0; i < w * h; i++)
            {
                buffer[i] = 0.0f;
            }
        }
        int handle(bool read_only = false, bool inverse = false)
        {
            for (int x = 0; x < size[0]; x++)
            {
                for (int y = 0; y < size[1]; y++)
                {
                    drawField(x, y, x == 0 && y == 0);
                }
            }
            int pos[2] = {0, 0}; 
            int key = 0;
            while (true)
            {
                GetKey(&key);
                if (key == KEY_CTRL_EXE)
                {
                    break;
                }
                if (key == KEY_CTRL_EXIT)
                {
                    break;
                }
                if (key == KEY_CTRL_LEFT || key == KEY_CTRL_RIGHT)
                {
                    drawField(pos[0], pos[1], false);
                    pos[0] += key == KEY_CTRL_LEFT ? size[0] - 1 : 1;
                    pos[0] %= size[0];
                    drawField(pos[0], pos[1], true);
                }
                else if (key == KEY_CTRL_UP || key == KEY_CTRL_DOWN)
                {
                    drawField(pos[0], pos[1], false);
                    pos[1] += key == KEY_CTRL_UP ? size[1] - 1 : 1;
                    pos[1] %= size[1];
                    drawField(pos[0], pos[1], true);
                }
                else if (!read_only)
                {
                    *getFloat(pos[0], pos[1]) = getInput(INPUT_POS_X, INPUT_POS_Y, &key);
                    if (key != KEY_CTRL_EXIT)
                    {
                        drawField(pos[0], pos[1], false);
                        pos[inverse]++;
                        if (pos[inverse] >= size[inverse])
                        {
                            pos[!inverse]++;
                            pos[0] %= size[0];
                            pos[1] %= size[1];
                        }
                        drawField(pos[0], pos[1], true);
                    }
                }
            }
            return key;
        }
        void drawLables(char cx, char cy, color_t color)
        {
            char buffer[2] = {cx, '\0'};
            int x = offset[0];
            int y = STATUS_AREA_HEIGHT + offset[1] - 2 * FIELD_SPACING;
            int x2, y2 = offset[1] - FONT_HEIGHT - 2 * FIELD_SPACING;
            for (int i = 0; i < size[0]; i++)
            {
                plotLineAA(x + FIELD_SPACING, y, x + FONT_WIDTH * FIELD_CHARS - 2 * FIELD_SPACING, y, color);
                x2 = x + (FONT_WIDTH * (FIELD_CHARS - 1) / 2);
                PrintMini(&x2, &y2, buffer, 0x02, LCD_WIDTH_PX, 0, 0, color, 0, true, 0);
                x += FONT_WIDTH * FIELD_CHARS + FIELD_SPACING;
                buffer[0]++;
            }
            x = offset[0] - 2 * FIELD_SPACING;
            y = STATUS_AREA_HEIGHT + offset[1] + FIELD_SPACING;
            int height = size[1] * (FONT_HEIGHT + FIELD_SPACING) - 3 * FIELD_SPACING;
            plotLineAA(x, y, x, y + height, color);
            plotLineAA(x, y, x + FIELD_SPACING, y, color);
            plotLineAA(x, y + height, x + FIELD_SPACING, y + height, color);
            y = offset[1];
            x -= FONT_WIDTH + FIELD_SPACING;
            buffer[0] = cy;
            for (int i = 0; i < size[1]; i++)
            {
                x2 = x;
                y2 = y;
                PrintMini(&x2, &y2, buffer, 0x02, LCD_WIDTH_PX, 0, 0, color, 0, true, 0);
                y += FONT_HEIGHT + FIELD_SPACING;
                buffer[0]++;
            }
        }
        float *getFloat(int x, int y)
        {
            return &buffer[y * size[0] + x];
        }
};
