#include "debug.h"
#include "vargs.h"
#include "x86.h"
#define PRINTK_MAX_LEN 100

void putch (int ch, void *putdat)
{
    console_print_char (ch);
}

long long get_int (va_list args, int lflag)
{
    long long n;

    if (lflag >= 2) {
        n = va_arg (args, long long);
    }else if (lflag == 1) {
        n = va_arg (args, long);
    }else {
        n = va_arg (args, int);
    }
    return n;
}

unsigned long long get_uint (va_list args, int lflag)
{
    unsigned long long n;

    if (lflag >= 2) {
        n = va_arg (args, unsigned long long);
    }else if (lflag == 1) {
        n = va_arg (args, unsigned long);
    }else {
        n = va_arg (args, unsigned int);
    }
    return n;
}

void print_num (void(*putch)(int ch, void *putdat), void *putdat, unsigned long long num, int base, int width, int padc)
{
    unsigned long long ret = num;
    unsigned mod = do_div (ret, base);

    if (num >= base){
        print_num (putch, putdat, ret, base, width-1, padc);
    }else {
        while (--width > 0){
            putch(padc, putdat);
        }
    }

    putch ("0123456789abcdef"[mod], putdat);
}

void vsprintfmt (void(*putch)(int ch, void *putdat), void *putdat, const char *fmt, va_list args)
{
    int width, lflag, base;
    unsigned long long num;
    char ch, padc;
    char *str=NULL;

    while (1){
        while ((ch = *fmt++) != '%'){
            if (ch == '\0'){
                return;
            }
            putch (ch, putdat);
        }

        padc = ' ';
        width = lflag = 0;
        base = 10;
    
    reswitch:
        switch (ch = *fmt++)
        {
            case '-':
                padc = '-';
                goto reswitch;
            case '0':
                padc = '0';
                goto reswitch;
            case '1' ... '9':
                for (;;++fmt){
                    width = width*10+(ch-'0');
                    ch = *fmt;
                    if (ch < '0' || ch > '9'){
                        break;
                    }
                }
                goto reswitch;
            case '*':
                width = va_arg (args, int);
                goto reswitch;
            case 'l':
                lflag++;
                goto reswitch;
            case 'c':
                putch (va_arg (args, int), putdat);
                break;
            case 's':
                if ((str = va_arg(args, char*)) == NULL){
                    str = "(null)";
                }
                if (width > 0 && padc != '-'){
                    for (width -= strlen(str); width > 0; width--){
                        putch (padc, putdat);
                    }
                }
                for (;(ch = *str++) != '\0'; width--){
                    if (ch < ' ' || ch > '~'){
                        putch ('?', putdat);
                    }else {
                        putch (ch, putdat);
                    }
                }
                for (; width > 0; width--){
                    putch (' ', putdat);
                }
                break;
            case 'd':
                num = get_int (args, lflag);
                if ((long long)num < 0){
                    putch('-', putdat);
                }
                base = 10;
                goto number;
            case 'u':
                num = get_uint (args, lflag);
                base = 10;
                goto number;
            case 'o':
                num = get_uint (args, lflag);
                base = 8;
                goto number;
            case 'p':
                putch ('0', putdat);
                putch ('x', putdat);
                num = (unsigned long long)(uintptr_t)va_arg(args, void *);
                base = 16;
                goto number;
            case 'x':
                num = get_uint (args, lflag);
                base = 16;
                goto number;
        number:
                print_num (putch, putdat, num, base, width, padc);
                break;

            default:
                putch ('%', putdat);
                break;
        }
    }
}
    
void printk (const char *fmt, ...)
{
    va_list args;

    va_start (args, fmt);
    vsprintfmt (putch, NULL, fmt, args);
    va_end (args);
}

