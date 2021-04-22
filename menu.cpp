#include <fxcg/display.h>
#include <fxcg/keyboard.h>

const color_t COLOR_BACKGROUND_SELECTED = COLOR_BLACK;
const color_t COLOR_BACKGROUND_UNSELECTED = COLOR_WHITE;
const color_t COLOR_TEXT_SELECTED = COLOR_WHITE;
const color_t COLOR_TEXT_UNSELECTED = COLOR_BLACK;

struct MenuElement
{
    const char *name;
    const int id_or_size;
    const MenuElement *subs = nullptr;
};

void drawElement(int pos, const char *text, bool selected)
{
    int x = 0;
    int y = pos * FONT_HEIGHT;
    color_t *VRAM = (color_t *)GetVRAMAddress();
    VRAM += (y + STATUS_AREA_HEIGHT) * LCD_WIDTH_PX;
    for (int i = 0; i < FONT_HEIGHT * LCD_WIDTH_PX; i++)
    {
        *VRAM++ = selected ? COLOR_BACKGROUND_SELECTED : COLOR_BACKGROUND_UNSELECTED;
    }
    PrintMini(&x, &y, text, 0, LCD_WIDTH_PX, 0, 0, COLOR_BLACK, COLOR_BLACK, false, 0);
    x = (LCD_WIDTH_PX - x) / 2;
    PrintMini(&x, &y, text, 0x02, LCD_WIDTH_PX, 0, 0, selected ? COLOR_TEXT_SELECTED : COLOR_TEXT_UNSELECTED, COLOR_BLACK, true, 0);
}

int getChoice(const MenuElement *elements, int size)
{
    for (int i = 0; i < size; i++)
    {
        drawElement(i, elements[i].name, !i);
    }
    int key, result, index = 0;
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
                result = getChoice(elements[index].subs, elements[index].id_or_size);
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
    color_t *VRAM = (color_t *)GetVRAMAddress();
    VRAM += STATUS_AREA_HEIGHT * LCD_WIDTH_PX;
    for (int i = 0; i < size * FONT_HEIGHT * LCD_WIDTH_PX; i++)
    {
        *VRAM++ = COLOR_BACKGROUND_UNSELECTED;
    }
    return result;
}
