#include "types.h"
#include "console.h"

void console_print_test ()
{
    console_clear();
    console_print_str_with_color ("hello world\n", rc_black, rc_green);
    console_print_hex (0x3456ff, rc_black, rc_green);
    console_print_char ('\n', rc_black, rc_green);
    console_print_dec (12345678, rc_black, rc_green);
}

int kern_entry()
{
    console_print_test ();
    return 0;
}
