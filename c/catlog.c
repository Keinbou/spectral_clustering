#pragma once
#include "catlog.h"
#include "this2this.c"
#include <stdio.h>

/* SPECIFIERS in C
    %c = char
    %d or %i = int
    %e or %E = scientific notation or float
    %f = float
    %g = float with current precision
    %ld or %li = long
    %lf = double
    %Lf = long double
    %lu = unsigned int or unsigned long
    %lli or %lld = unsigned long long
    %o = octal representation
    %p = pointer
    %s = string
    %u = unsigned int
    %x or %X = hexadecimal representation
    %n = prints nothing
    %% = % character
*/

void *safe_malloc (size_t s)
{
    void *p = malloc(s);
    if ( p==NULL )
    {
        fprintf(stderr, "Fatal: failed to allocate %zu bytes.", s);
        abort();
    }
    return p;
}

char *concat (const char *s1, const char *s2)
{
    char *result = (char*)safe_malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

const char *catlog (int count, ...)
{
    va_list args;
    va_start(args, count);
    const char *result = va_arg(args, const char *);
    for( int i=1; i<count; ++i )
    {
        result = concat(result, " ");
        const char *arg = va_arg(args, const char *);
        result = concat(result, arg);
    }
    va_end(args);
    return result;
}

void LOG (int count, ...)
{
    va_list args;
    va_start(args, count);
    const char *result = va_arg(args, const char *);
    for( int i=1; i<count; ++i )
    {
        result = concat(result, " ");
        const char *arg = va_arg(args, const char *);
        result = concat(result, arg);
    }
    vprintf(n(result), args);
    va_end(args);
}

void p (const char* str, ...)
    {
        va_list args;
        va_start(args, str);
        vprintf(n(str), args);
        va_end(args);
    }

const char* n (const char* print)
    {
        size_t length = snprintf(NULL, 0, "%s\n", print)+1; // +1 for terminating character
        char *str = (char*)safe_malloc(length);
        snprintf(str, length, "%s\n", print);
        return str;
    }

void errmsg (const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    size_t length = snprintf(NULL, 0, msg, args)+1; // +1 for terminating character
    char *str = (char*)safe_malloc(length);
    vsnprintf(str, length, msg, args);
    fprintf(stderr, n(str), args);
    va_end(args);
    exit(1);
}

const char* printfStr(const char* print, ...)
{
    va_list args;
    va_start(args, print);
    size_t length = snprintf(NULL, 0, print, args)+1; // +1 for terminating character
    char *str = (char*)safe_malloc(length);
    vsnprintf(str, length, print, args);
    va_end(args);
    return str;
}