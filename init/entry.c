#include "types.h"
#include "console.h"

int kern_entry()
{
    console_clear();
    //console_print_str_with_color ("hello world\n", rc_black, rc_green);
    console_print_hex (0xffffffff, rc_black, rc_green);
    return 0;
}
