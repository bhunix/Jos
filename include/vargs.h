#ifndef __VARGS_H__
#define __VARGS_H__
typedef __builtin_va_list va_list;
#define va_start(p,arg) __builtin_va_start(p,arg)
#define va_arg(p,t) __builtin_va_arg(p,t)
#define va_end(p) __builtin_va_end(p)
#endif
