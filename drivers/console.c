#include "console.h"

#define VGA_COLOR_ATTR(back, front) (((back) << 4) | ((front) & 0x0F))
#define VGA_CHAR_MODE(c, back, front) ((c) | ((VGA_COLOR_ATTR(back, front))<<8))
#define CRT_ROWS 25
#define CRT_ROLS 80
#define CRT_SIZE (CRT_ROWS * CRT_ROLS)

//CGA RAM address is 0xb8000
static uint16_t *crt_buf = (uint16_t *)0xB8000;

static uint8_t crt_x = 0;
static uint8_t crt_y = 0;

static void set_crt_pos ()
{
    uint16_t crt_pos = crt_y * CRT_ROLS + crt_x;

    outb(0x3D4, 14);
    outb(0x3D5, crt_pos >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, crt_pos);
}

void console_clear()
{
    int i;
    uint16_t space = VGA_CHAR_MODE (0x20, 0, 15);

    for (i = 0; i < CRT_SIZE; i++) {
        crt_buf[i] = space;
    }

    crt_x = 0;
    crt_y = 0;
    set_crt_pos();
}

static void console_scroll()
{
    int i;
    uint16_t space = VGA_CHAR_MODE (0x20, 0, 15);

    if (crt_y >= CRT_ROWS)
    {
        //move up all line
        for (i = 0; i < CRT_SIZE - CRT_ROLS; i++)
        {
            crt_buf[i] = crt_buf[i+CRT_ROLS];
        }

        //clear the last line
        for (i = CRT_SIZE-CRT_ROLS; i < CRT_SIZE; i++)
        {
            crt_buf[i] = space;
        }

        crt_y = 24;
    }
}

void console_print_char (char c)
{
    console_print_char_with_color (c, rc_black, rc_white);
}

void console_print_char_with_color (char c, color_t back, color_t front)
{
    uint16_t chart = VGA_CHAR_MODE (c, back, front);

    if (c == '\b' && crt_x) {
        crt_x--;
    } else if (c == '\t') {
        crt_x = (crt_x+8) & ~(8-1);
    } else if (c == '\r') {
        crt_x = 0;
    } else if (c == '\n') {
        crt_x = 0;
        crt_y++;
    } else if (c >= ' ') {
        crt_buf[crt_y*CRT_ROLS + crt_x] = chart;
        crt_x++;
    }

    if (crt_x >= CRT_ROLS)
    {
        crt_x = 0;
        crt_y++;
    }

    console_scroll();

    set_crt_pos();
}

void console_print_dec(uint32_t n)
{
    console_print_dec_with_color(n, rc_black, rc_white);
}

void console_print_hex(uint32_t n)
{
    console_print_hex_with_color(n, rc_black, rc_white);
}

void console_print_str (char *cstr)
{
    while (*cstr)
    {
        console_print_char (*cstr++);
    }
}

void console_print_str_with_color (char *cstr, color_t back, color_t front)
{
    while (*cstr)
    {
        console_print_char_with_color (*cstr++, back, front);
    }
}

void console_print_hex_with_color(uint32_t n, color_t back, color_t front)
{
    int i, zero_flag = 1;
    uint8_t hex;
    uint16_t chart;

    console_print_str_with_color ("0x", back, front);

    for (i = 7; i >= 0; i--)
    {
        hex = (n >> i*4) & 0xF; //get each hex
        if (!hex && zero_flag)
        {
            //clear ahead zero
            continue;
        }
        zero_flag = 0;
        hex += (hex < 10)?'0':('a'-10); //translate hex to ascii
        chart = VGA_CHAR_MODE (hex, back, front);
        console_print_char_with_color (chart, back, front);
    }
}

#define MAX_INT_CNT 50
void console_print_dec_with_color(uint32_t n, color_t back, color_t front)
{
    char c_dec[MAX_INT_CNT];
    int len;
    uint32_t dec = n;

    len = MAX_INT_CNT-1;
    c_dec[len--] = '\0';
    for (; len > 0 && dec > 0; len--){
        c_dec[len] = '0' + dec%10;
        dec /= 10;
    }
    len++;

    console_print_str_with_color (&c_dec[len], back, front);
}

