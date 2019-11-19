#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

static const char *str_digits = "0123456789abcdef";

int abs(int j)
{
    return (j < 0 ? -j : j);
}

void reverse(char *begin, char *end)
{
    char *is = begin;
    char *ie = end - 1;
    while (is < ie)
    {
        char tmp = *ie;
        *ie = *is;
        *is = tmp;
        ++is;
        --ie;
    }
}

extern long atol(const char *s)
{
    long a;
    unsigned char sign, c;
    do
        c = *s++;
    while (c == ' ' || c == '\t');
    a = 0;
    sign = 0;
    if (c == '-')
    {
        sign++;
        c = *s++;
    }
    else if (c == '+')
        c = *s++;
    while (isdigit(c))
    {
        a = a * 10L + (unsigned char)(c - '0');
        c = *s++;
    }
    if (sign)
        return -a;
    return a;
}

extern char *itoa(int value, char *result, int base)
{
    if (result)
    {
        if (base < 2 || base > 16)
        {
            *result = 0;
            return result;
        }
        char *out = result;
        int quotient = abs(value);
        do
        {
            const int tmp = quotient / base;
            *out = str_digits[quotient - (tmp * base)];
            ++out;
            quotient = tmp;
        } while (quotient);
        if (value < 0)
            *out++ = '-';
        reverse(result, out);
        *out = 0;
    }
    return result;
}

extern char *ltoa(long value, char *result, int base)
{
    if (result)
    {
        if (base < 2 || base > 16)
        {
            *result = 0;
            return result;
        }
        char *out = result;
        long quotient = abs(value);
        do
        {
            const long tmp = quotient / base;
            *out = str_digits[quotient - (tmp * base)];
            ++out;
            quotient = tmp;
        } while (quotient);
        if (value < 0)
            *out++ = '-';
        reverse(result, out);
        *out = 0;
    }
    return result;
}

extern char *utoa(unsigned value, char *result, int base)
{
    if (result)
    {
        if (base < 2 || base > 16)
        {
            *result = 0;
            return result;
        }
        char *out = result;
        unsigned quotient = value;
        do
        {
            const unsigned tmp = quotient / base;
            *out = str_digits[quotient - (tmp * base)];
            ++out;
            quotient = tmp;
        } while (quotient);
        reverse(result, out);
        *out = 0;
    }
    return result;
}

extern char *ultoa(unsigned long value, char *result, int base)
{
    if (result)
    {
        if (base < 2 || base > 16)
        {
            *result = 0;
            return result;
        }
        char *out = result;
        unsigned long quotient = value;
        do
        {
            const unsigned long tmp = quotient / base;
            *out = str_digits[quotient - (tmp * base)];
            ++out;
            quotient = tmp;
        } while (quotient);
        reverse(result, out);
        *out = 0;
    }
    return result;
}

double atof(register const char *s)
{
    return strtod(s, 0);
}
