#pragma once
#include <stdlib.h>
#include "catlog.c"

#define LEN(x) sizeof((x))/sizeof((x)[0])

#define delete_this(x) free(x)

char* int2String(int a);
char* float2String(float a);
char* char2String(char a);
void free_this(void *a);
void zerofy(int arr[], size_t size);