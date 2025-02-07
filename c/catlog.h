#pragma once
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void *safe_malloc(size_t s);
char *concat(const char *s1, const char *s2);
const char *catlog (int count, ...);
void LOG (int count, ...);
void p(const char *str, ...);
const char* n (const char* print);
void errmsg(const char *msg, ...);
const char* printfStr(const char* str, ...);