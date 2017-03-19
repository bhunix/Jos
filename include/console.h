#ifndef INCLUDE_CONSOLE_H_
#define INCLUDE_CONSOLE_H_

#include "types.h"
#include "common.h"

typedef
enum real_color {
    rc_black = 0,
    rc_blue = 1,
    rc_green = 2,
    rc_cyan = 3,
    rc_red = 4,
    rc_magenta = 5,
    rc_brown = 6,
    rc_light_grey = 7,
    rc_dark_grey = 8,
    rc_light_blue = 9,
    rc_light_green = 10,
    rc_light_cyan = 11,
    rc_light_red = 12,
    rc_light_magenta = 13,
    rc_light_brown = 14,
    rc_white = 15
} color_t;

//clear console
void console_clear();

//print char
void console_print_char(char c, color_t back, color_t front);

//print str end by '\0' with default color
void console_print_str(char *cstr);

//print str end by '\0' with color
void console_print_str_with_color (char *cstr, color_t back, color_t front);

//print hex
void console_print_hex(uint32_t n, color_t back, color_t front);

//print int
void console_write_dec(uint32_t n, color_t back, color_t front);

#endif /* INCLUDE_CONSOLE_H_ */
