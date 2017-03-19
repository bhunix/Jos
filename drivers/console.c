#include "console.h"

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
    uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);

    //fill console with space char, 0x20 is ascii
    uint16_t blank = 0x20 | (attribute_byte << 8);

    for (i = 0; i < 80 * 25; i++) {
        g_vram[i] = blank;
    }
    g_cursor_x = 0;
    g_cursor_y = 0;
    move_cursor();
}

static void scroll()
{
    int i;
    uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);

    //fill console with space char, 0x20 is ascii
    uint16_t blank = 0x20 | (attribute_byte << 8);

    if (g_cursor_y >= 25)
    {
        //move all up one line
        for (i = 0; i < 24*80; i++)
        {
            g_vram[i] = g_vram[i+80];
        }
        
        //clear the last line
        for (i = 24*80; i < 25*80; i++)
        {
            g_vram[i] = blank;
        }

        g_cursor_y = 24;
    }
}

void console_print_char (char c, color_t back, color_t front)
{
    uint8_t back_color = back;
    uint8_t front_color = front;

    uint16_t chart = c | (((back_color << 4) | (front_color & 0x0F)) << 8);

    /* ascii code
     * 0x08 is backspace 
     * 0x09 is tab */
    if (c == 0x08 && g_cursor_x) {
        g_cursor_x--;
    } else if (c == 0x09) {
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
    uint8_t back_color = back;
    uint8_t front_color = front;
    uint8_t hex;

    uint16_t chart;
    console_print_str_with_color ("hello world\n", back, front);
    for (i = 0; i < 4; i++)
    {
        console_print_char (i, back, front);
        hex = (n >> i*8) & 0xFF;
        chart = hex | (((back_color << 4) | (front_color & 0x0F)) << 8);
        console_print_char (chart, back, front);
    }
}

