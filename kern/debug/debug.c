#include "debug.h"
#define PRINTK_MAX_LEN 100

void printk (const char *format, ...)
{
    va_list p;
    char str[PRINTK_MAX_LEN];
    int arg;
    int len;

    va_start (p, format);
    len = strlen (format);
    for (i = 0; i < len; i++){
        if (format[i] == '%'){
            if (format[i+1] == 'd'){
                arg = va_arg(p, int);
                while (arg > 0){
                    str[i++] = arg%10;
                    arg /= 10;
                }
            }
        }
        else{
            str[i] = format[i];
            i++;
        }
    }
    va_end (p);
}
    
