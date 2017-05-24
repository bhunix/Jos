#include "types.h"
#include "console.h"
#include "string.h"
#include "debug.h"

void string_test ()
{
    char s[11] = "hello";
    char s2[10] = "world";
    console_clear();
    console_print_str_with_color (strcat(s, s2), rc_black, rc_green);
    console_print_dec_with_color (strlen(s), rc_black, rc_green);
    console_print_dec_with_color (strcmp(s, s), rc_black, rc_green);
}

void console_print_test ()
{
    console_clear();
    console_print_str_with_color ("hello world\n", rc_black, rc_green);
    console_print_hex (0x1234abc9);
    console_print_char ('\n');
    console_print_dec (12345678);
}

int kern_entry()
{
    //console_print_test ();
    //string_test();
    //console_clear();
    printk("%-10d",1234);
    return 0;
}
