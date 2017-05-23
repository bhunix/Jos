#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#if 0
#define do_div(n, base) ({                                        \
    unsigned long __upper, __low, __high, __mod, __base;        \
    __base = (base);                                            \
    asm("" : "=a" (__low), "=d" (__high) : "A" (n));            \
    __upper = __high;                                            \
    if (__high != 0) {                                            \
        __upper = __high % __base;                                \
        __high = __high / __base;                                \
    }                                                            \
    asm("divl %2" : "=a" (__low), "=d" (__mod)                    \
        : "rm" (__base), "0" (__low), "1" (__upper));            \
    asm("" : "=A" (n) : "a" (__low), "d" (__high));                \
    __mod;                                                        \
 })
#endif

int test (int a, ...)
{
    int i;
    va_list ap;
    va_start (ap, a);
    for (i = 0; i < 4; i++)
    {
        printf("%d\n", *ap);
        va_arg (ap, int);
    }
    va_end (ap);
}

int mAtoi(char *buf)
{
    printf("buf: %s", buf);
    if (strlen(buf) < 2)
    {
        printf("buf: %c", buf[0]);
        return buf[0]-'0';
    }

    return mAtoi(buf+1) * 10 + (buf[0]-'0');
}

int do_div (int n, int base)
{
    return n/base;
}

void putch (int ch, void *ref)
{
    printf("%c", ch);
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

void mPrintfmt (void(*putch)(int ch, void *ref), const char *fmt, va_list args)
{
    void *ref=NULL;
    char ch;
    while (1)
    {
        while ((ch = *fmt++) != '%')
        {
            if (ch == '\0')
            {
                return;
            }
            putch (ch, ref);
        }

        char padc;
        int width, lflag;
    
        switch (ch)
        {
            case '-':
                break;
            case '0':
                break;
            case 1 ... 9:
                break;
            case '#':
                break;
            case 'l':
                lflag++;
                break;
            case 'c':
                {
                    char c;
                    c  = va_arg (args, char);
                    putch(c, ref);
                    break;
                }
            case 's':
                break;
            case 'd':
                {
                    long long n;
                    n = get_int (args, lflag);
                    goto number;
                }
                break;
            case 'u':
                break;
            case 'o':
                break;
        number:

            case 'p':
                break;
            case 'x':
                break;
            default:
                break;
        }
    }
}

void mPrint (char *fmt, ...)
{
    va_list args;

    va_start (args, fmt);
    mPrintfmt (putch, fmt, args);
    va_end (args);
}

int main(int argc, char **argv)
{
    mPrint ("%d\n", 10);
}


