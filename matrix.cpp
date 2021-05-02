#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/heap.h>

#include "atof.hpp"
#include "ftoa.hpp"
#include "draw.hpp"

#define MATRIX_STATUS_AREA_HEIGHT 24
#define MATRIX_FONT_WIDTH 12
#define MATRIX_FONT_HEIGHT 18

#define MATRIX_FONT_MODE_TRANSPARENT 0x02

#define MATRIX_FIELD_CHARS 7
#define MATRIX_FIELD_SPACING 3

#define MATRIX_INPUT_BUFFER 25
#define MATRIX_INPUT_POS_X 1
#define MATRIX_INPUT_POS_Y 7

#define MATRIX_OFFSET 15
#define MATRIX_OFFSET_X MATRIX_OFFSET + MATRIX_FONT_WIDTH + MATRIX_FIELD_SPACING * 3
#define MATRIX_OFFSET_Y MATRIX_OFFSET + MATRIX_FONT_HEIGHT + MATRIX_FIELD_SPACING

#define FKEY_SOLVE 0x0148
#define FKEY_CLEAR 0x0149

class Matrix
{
    private:
        unsigned char size[2];
        float **vals;
        void drawField(int x, int y, bool selected);
        void getInput(float *result, int *key);
    public:
        Matrix(unsigned char width, unsigned char height, float **values);
        void drawLables(char ordinarX, char ordinarY, color_t color);
        int handle(bool read_only = false, bool inverse = false);
};

Matrix::Matrix(unsigned char width, unsigned char height, float **values)
{
    size[0] = width;
    size[1] = height;
    vals = values;
    for (unsigned char i = 0; i < width * height; i++)
    {
        *vals[i] = 0;
    }
}

void Matrix::drawField(int x, int y, bool selected)
{
    char buffer[MATRIX_FIELD_CHARS + 1];
    float f = *vals[y * size[0] + x];
    ftoa(f, buffer, (f >= 0.0f) + (f >= 1.0f) + (f < pow10(10) && f * pow10(10) > 1.0f));
    int width = MATRIX_FONT_WIDTH * MATRIX_FIELD_CHARS;
    x = MATRIX_OFFSET_X + x * (width + MATRIX_FIELD_SPACING);
    y = MATRIX_OFFSET_Y + y * (MATRIX_FONT_HEIGHT + MATRIX_FIELD_SPACING);
    fillArea(x, MATRIX_STATUS_AREA_HEIGHT + y, width, MATRIX_FONT_HEIGHT, selected ? COLOR_BLACK : COLOR_WHITE);
    PrintMini(&x, &y, buffer, MATRIX_FONT_MODE_TRANSPARENT, x + width, 0, 0, selected ? COLOR_WHITE : COLOR_BLACK, 0, true, 0);
}

void Matrix::getInput(float *result, int *key)
{
    char buffer[MATRIX_INPUT_BUFFER];
    *buffer = '\0';
    int cursor = 0, start = 0;
    while (true)
    {
        if (*key && *key < 30000)
        {
            cursor = EditMBStringChar((unsigned char *)buffer, MATRIX_INPUT_BUFFER, cursor, *key);
            DisplayMBString((unsigned char *)buffer, start, cursor, MATRIX_INPUT_POS_X, MATRIX_INPUT_POS_Y);
        }
        else
        {
            EditMBStringCtrl((unsigned char *)buffer, MATRIX_INPUT_BUFFER, &start, &cursor, key, MATRIX_INPUT_POS_X, MATRIX_INPUT_POS_Y);
        }
        GetKey(key);
        if (*key == KEY_CTRL_EXE || *key == KEY_CTRL_EXIT)
        {
            break;
        }
    }
    DisplayMBString((unsigned char *)"", 0, 0, MATRIX_INPUT_POS_X, MATRIX_INPUT_POS_Y);
    Cursor_SetFlashOff();
    *result = atof(buffer);
}

void Matrix::drawLables(char ordinarX, char ordinarY, color_t color)
{
    char buffer[2] = {ordinarX, '\0'};
    int x = MATRIX_OFFSET_X;
    int y = MATRIX_OFFSET_Y + MATRIX_STATUS_AREA_HEIGHT - MATRIX_FIELD_SPACING;
    int x2 = x, y2 = MATRIX_OFFSET_Y - MATRIX_FIELD_SPACING - MATRIX_FONT_HEIGHT;
    for (int i = 0; i < size[0]; i++)
    {
        plotLineAA(x + MATRIX_FIELD_SPACING, y, x + MATRIX_FIELD_CHARS * MATRIX_FONT_WIDTH - 2 * MATRIX_FIELD_SPACING, y, color, COLOR_WHITE);
        x2 += MATRIX_FONT_WIDTH * (MATRIX_FIELD_CHARS - 1) / 2;
        PrintMini(&x2, &y2, buffer, MATRIX_FONT_MODE_TRANSPARENT, LCD_WIDTH_PX, 0, 0, color, 0, true, 0);
        x += MATRIX_FIELD_CHARS * MATRIX_FONT_WIDTH + MATRIX_FIELD_SPACING;
        x2 = x;
        buffer[0]++;
    }
    x = MATRIX_OFFSET_X - 2 * MATRIX_FIELD_SPACING;
    y = MATRIX_OFFSET_Y + MATRIX_STATUS_AREA_HEIGHT + MATRIX_FIELD_SPACING;
    int height = size[1] * (MATRIX_FONT_HEIGHT + MATRIX_FIELD_SPACING) - 3 * MATRIX_FIELD_SPACING;
    plotLineAA(x, y, x, y + height, color, COLOR_WHITE);
    plotLineAA(x, y, x + MATRIX_FIELD_SPACING, y, color, COLOR_WHITE);
    plotLineAA(x, y + height, x + MATRIX_FIELD_SPACING, y + height, color, COLOR_WHITE);
    y = MATRIX_OFFSET_Y;
    x -= MATRIX_FONT_WIDTH + MATRIX_FIELD_SPACING;
    buffer[0] = ordinarY;
    for (int i = 0; i < size[1]; i++)
    {
        x2 = x;
        y2 = y;
        PrintMini(&x2, &y2, buffer, MATRIX_FONT_MODE_TRANSPARENT, LCD_WIDTH_PX, 0, 0, color, 0, true, 0);
        y += MATRIX_FONT_HEIGHT + MATRIX_FIELD_SPACING;
        buffer[0]++;
    }
}

int Matrix::handle(bool read_only, bool inverse)
{
    for (int x = 0; x < size[0]; x++)
    {
        for (int y = 0; y < size[1]; y++)
        {
            drawField(x, y, x == 0 && y == 0);
        }
    }
    if (!read_only)
    {
        drawFKey(FKEY_SOLVE, 0);
        drawFKey(FKEY_CLEAR, 1);
    }
    int pos[2] = {0, 0}; 
    int key = 0;
    while (true)
    {
        GetKey(&key);
        if (key == KEY_CTRL_EXE || key == KEY_CTRL_F1)
        {
            break;
        }
        else if (key == KEY_CTRL_EXIT)
        {
            break;
        }
        else if (key == KEY_CTRL_LEFT || key == KEY_CTRL_RIGHT)
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
            if (key == KEY_CTRL_F2)
            {
                for (int x = 0; x < size[0]; x++)
                {
                    for (int y = 0; y < size[1]; y++)
                    {
                        *vals[y * size[0] + x] = 0.0f;
                        drawField(x, y, x == pos[0] && y == pos[1]);
                    }
                }
            }
            else
            {
                getInput(vals[pos[1] * size[0] + pos[0]], &key);
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
    }
    return key;
}
