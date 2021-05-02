#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/system.h>

#include "draw.hpp"

#define MENU_STATUS_AREA_HEIGHT 24
#define MENU_FONT_MINI_HEIGHT 18
#define MENU_FONT_NORMAL_WIDTH 18
#define MENU_FONT_NORMAL_HEIGHT 24

#define MENU_FONT_MODE_TRANSPARENT 0x02

#define MENU_SPACING 8

#define COLOR_HEADER COLOR_BLUE

struct MenuElement
{
    const char *name;
    const int id_or_size;
    const MenuElement *subs = nullptr;
};

void drawElement(int pos, const char *text, bool selected)
{
    int x = 0;
    int y = MENU_FONT_NORMAL_HEIGHT + MENU_SPACING + pos * (MENU_SPACING + MENU_FONT_MINI_HEIGHT);
    fillArea(0, MENU_STATUS_AREA_HEIGHT + y - MENU_SPACING / 2, LCD_WIDTH_PX, MENU_FONT_MINI_HEIGHT + MENU_SPACING, selected ? COLOR_BLACK : COLOR_WHITE);
    PrintMini(&x, &y, text, 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_BLACK, false, 0);
    x = (LCD_WIDTH_PX - x) / 2;
    PrintMini(&x, &y, text, MENU_FONT_MODE_TRANSPARENT, LCD_WIDTH_PX, 0, 0, selected ? COLOR_WHITE : COLOR_BLACK, COLOR_BLACK, true, 0);
}

int getChoice(char *header, const MenuElement *elements, int size)
{
    Bdisp_Fill_VRAM(COLOR_WHITE, 1);
    int x = (LCD_WIDTH_PX - MB_ElementCount(header) * MENU_FONT_NORMAL_WIDTH) / 2;
    PrintCXY(x, 0, header, TEXT_MODE_TRANSPARENT_BACKGROUND, -1, COLOR_HEADER, 0, 1, 0);
    int index = 0;
    for (int i = size - 1; i >= 0; i--)
    {
        drawElement(i, elements[i].name, i == index);
    }
    int key, result;
    while (true)
    {
        GetKey(&key);
        if (key == KEY_CTRL_EXIT)
        {
            result = -1;
            break;
        }
        else if (key == KEY_CTRL_EXE)
        {
            if (elements[index].subs == nullptr)
            {
                result = elements[index].id_or_size;
            }
            else
            {
                result = getChoice((char *)elements[index].name, elements[index].subs, elements[index].id_or_size);
            }
            break;
        }
        else if (key == KEY_CTRL_UP || key == KEY_CTRL_DOWN)
        {
            drawElement(index, elements[index].name, false);
            index += key == KEY_CTRL_UP ? size - 1 : 1;
            index %= size;
            drawElement(index, elements[index].name, true);
        }
    }
    return result;
}
