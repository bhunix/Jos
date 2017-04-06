#include "string.h"


void memcpy(uint8_t *dst, const uint8_t *src, uint32_t len)
{
    while (len--){
        *dst++ = *src++;
    }
}

void memset(void *dst, uint8_t val, uint32_t len)
{
    while (len--){
        *(uint8_t*)dst++ = val;
    }
}

void bzero(void *dst, uint32_t len)
{
    memset (dst, 0, len);
}

int strcmp(const char *str1, const char *str2)
{
    while ((*str1 != NULL) && (*str2 != NULL)){
        if (*str1 != *str2){
            return (*str1=='\0')?-1:1;
        }
        str1++;
        str2++;
    }

    return 0;
}

char *strcpy(char *dst, const char *src)
{
    char *tmp = dst;

    while (*src != '\0'){
        *dst++ = *src++;
    }
    *dst = '\0';

    return tmp;
}

char *strcat(char *dst, const char *src)
{
    char *tmp = dst;

    while (*dst){
        dst++;
    }

    while ((*dst++ = *src++))
        ;

    return tmp;
}

int strlen(const char *src)
{
    const char *end = src;

    while (*end++ != '\0')
        ;

    return (end - src - 1);
}
