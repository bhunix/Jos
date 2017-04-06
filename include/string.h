#ifndef INCLUDE_STRING_H_
#define INCLUDE_STRING_H_

#include "types.h"

void memcpy(uint8_t *dst, const uint8_t *src, uint32_t len);

void memset(void *dst, uint8_t val, uint32_t len);

void bzero(void *dst, uint32_t len);

int strcmp(const char *str1, const char *str2);

char *strcpy(char *dst, const char *src);

char *strcat(char *dst, const char *src);

int strlen(const char *src);

#endif 	// INCLUDE_STRING_H_
