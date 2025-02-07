#pragma once
#include "this2this.h"

char* int2String(int a)
{
    size_t length = snprintf(NULL, 0, "%d", a)+1; // +1 for terminating character
    char *str = (char*)safe_malloc(length);
    snprintf(str, length, "%d", a);
    return str;
}

char* float2String(float a)
{
    size_t length = snprintf(NULL, 0, "%f", a)+1; // +1 for terminating character
    char *str = (char*)safe_malloc(length);
    snprintf(str, length, "%f", a);
    return str;
}

char* char2String(char a)
{
    size_t length = sizeof(char)+1;
    char *str = (char*)safe_malloc(length);
    snprintf(str, length, "%c", a);
    return str;
}

void free_this(void *a)
{
    free(a);
}

void zerofy(int arr[], size_t size) //size is the bytes to fill.
{
    memset(arr, 0, size);
}