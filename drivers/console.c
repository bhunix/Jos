#include "console.h"

#define VGA_COLOR_ATTR(back, front) (((back) << 4) | ((front) & 0x0F))
#define VGA_CHAR_MODE(c, back, front) ((c) | ((VGA_COLOR_ATTR(back, front))<<8))
#define CRT_ROWS 25
#define CRT_ROLS 80

//VRAM address is 0xb8000
static uint16_t *g_vram = (uint16_t *)0xB8000;

static uint8_t g_cursor_x = 0;
static uint8_t g_cursor_y = 0;

static void move_cursor ()
{
    uint16_t cursor_pos = g_cursor_y * 80 + g_cursor_x;

    outb(0x3D4, 14);
    outb(0x3D5, cursor_pos >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, cursor_pos);
}

void console_clear()
{
    int i;
    //uint8_t color_attr = VGA_COLOR_ATTR(0, 15);

    //fill console with space, 0x20 is ascii
    //uint16_t space = 0x20 | (color_attr << 8);
    uint16_t space = VGA_CHAR_MODE (0x20, 0, 15);

    for (i = 0; i < 80 * 25; i++) {
        g_vram[i] = space;
    }

    g_cursor_x = 0;
    g_cursor_y = 0;
    move_cursor();
}

static void scroll()
{
    int i;
    //uint8_t color_attr = VGA_COLOR_ATTR(0, 15);

    //fill console with space, 0x20 is ascii
    uint16_t space = VGA_CHAR_MODE (0x20, 0, 15);

    if (g_cursor_y >= 25)
    {
        //move up all line
        for (i = 0; i < 24*80; i++)
        {
            g_vram[i] = g_vram[i+80];
        }

        //clear the last line
        for (i = 24*80; i < 25*80; i++)
        {
            g_vram[i] = space;
        }

        g_cursor_y = 24;
    }
}

void console_print_char (char c, color_t back, color_t front)
{
    uint16_t chart = VGA_CHAR_MODE (c, back, front);

    /* ascii code
     * 0x08 is backspace
     * 0x09 is tab */
    if (c == '\b' && g_cursor_x) {
        g_cursor_x--;
    } else if (c == '\t') {
        g_cursor_x = (g_cursor_x+8) & ~(8-1);
    } else if (c == '\r') {
        g_cursor_x = 0;
    } else if (c == '\n') {
        g_cursor_x = 0;
        g_cursor_y++;
    } else if (c >= ' ') {
        g_vram[g_cursor_y*80 + g_cursor_x] = chart;
        g_cursor_x++;
    }

    if (g_cursor_x >= 80)
    {
        g_cursor_x = 0;
        g_cursor_y++;
    }

    scroll();

    move_cursor();
}

void console_print_str (char *cstr)
{
    while (*cstr)
    {
        console_print_char (*cstr++, rc_black, rc_white);
    }
}

void console_print_str_with_color (char *cstr, color_t back, color_t front)
{
    while (*cstr)
    {
        console_print_char (*cstr++, back, front);
    }
}

void console_print_hex(uint32_t n, color_t back, color_t front)
{
    int i;
    uint8_t hex;
    uint16_t chart;

    for (i = 7; i >= 0; i--)
    {
        hex = (n >> i*4) & 0x0F; //get each hex
        hex += (hex < 9)?0x30:0x57; //translate hex to ascii
        chart = VGA_CHAR_MODE (hex, back, front);
        console_print_char (chart, back, front);
    }
}

#define MAX_INT_CNT 100
void console_print_dec(uint32_t n, color_t back, color_t front)
{
    int ary_dec[MAX_INT_CNT];
    int i, n_count;
    uint16_t chart;
    uint32_t dec = n;

    n_count = 0;
    for (i = 0; i < MAX_INT_CNT && dec > 0; i++)
    {
        ary_dec[i] = dec%10;
        dec /= 10;

        n_count++;
    }

    for (i = n_count-1; i >= 0; i--)
    {
        dec = ary_dec[i] + 0x30; //translate dec to ascii
        chart = VGA_CHAR_MODE (dec, back, front);
        console_print_char (chart, back, front);
    }
}

