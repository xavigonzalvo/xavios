#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char *itoa(int num, char *str, int base)
{
    int i = 0;
    bool isNegative = false;

    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }

    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    if (isNegative)
    {
        str[i++] = '-';
    }

    str[i] = '\0';

    reverse(str, i);

    return str;
}

extern "C"
{

    int simple_vsprintf(char *buf, size_t size, const char *format, va_list ap)
    {
        int int_temp;
        char int_buf[32];
        const char *p, *sval;
        int i = 0;

        // Make sure buf and format are not NULL
        if (buf == NULL || format == NULL || size == 0)
        {
            return -1;
        }

        for (p = format; *p && i < size - 1; p++)
        {
            if (*p != '%')
            {
                buf[i++] = *p;
                continue;
            }

            switch (*++p)
            {
            case 'd':
                int_temp = va_arg(ap, int);
                itoa(int_temp, int_buf, 10);
                for (sval = int_buf; *sval && i < size - 1; sval++)
                {
                    buf[i++] = *sval;
                }
                break;
            case 's':
                for (sval = va_arg(ap, char *); *sval && i < size - 1; sval++)
                {
                    buf[i++] = *sval;
                }
                break;
            case '%':
                buf[i++] = '%'; // handle %%
                p--;            // so next iteration will handle the second %
                break;
            default:
                buf[i++] = '%'; // write the '%' character
                buf[i++] = *p;  // write the unknown specifier character
                break;
            }
        }
        buf[i] = '\0';
        return i;
    }

    int simple_sprintf(char *buf, size_t size, const char *format, ...)
    {
        va_list ap;
        int int_temp;
        char int_buf[32];
        const char *p, *sval;
        int i = 0;

        // Make sure buf and format are not NULL
        if (buf == NULL || format == NULL || size == 0)
        {
            return -1;
        }

        va_start(ap, format);
        for (p = format; *p && i < size - 1; p++)
        {
            if (*p != '%')
            {
                buf[i++] = *p;
                continue;
            }

            switch (*++p)
            {
            case 'd':
                int_temp = va_arg(ap, int);
                itoa(int_temp, int_buf, 10);
                for (sval = int_buf; *sval && i < size - 1; sval++)
                {
                    buf[i++] = *sval;
                }
                break;
            case 's':
                for (sval = va_arg(ap, char *); *sval && i < size - 1; sval++)
                {
                    buf[i++] = *sval;
                }
                break;
            case '%':
                buf[i++] = '%'; // handle %%
                p--;            // so next iteration will handle the second %
                break;
            default:
                buf[i++] = '%'; // write the '%' character
                buf[i++] = *p;  // write the unknown specifier character
                break;
            }
        }
        va_end(ap);
        buf[i] = '\0';
        return i;
    }

} // extern "C"
