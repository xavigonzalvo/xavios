#include <stdarg.h>

void reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char* itoa(int num, char* str, int base) {
    int i = 0;
    bool isNegative = false;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }

    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    reverse(str, i);

    return str;
}

extern "C" {

int simple_sprintf(char* buf, const char* format, ...) {
    char* arg;
    va_list ap;
    int int_temp;
    char int_buf[32];
    const char *p, *sval;
    int i = 0;

    va_start(ap, format);
    for (p = format; *p; p++) {
        if (*p != '%') {
            buf[i++] = *p;
            continue;
        }

        switch (*++p) {
            case 'd':
                int_temp = va_arg(ap, int);
                itoa(int_temp, int_buf, 10);
                for (sval = int_buf; *sval; sval++) {
                    buf[i++] = *sval;
                }
                break;
            case 's':
                for (sval = va_arg(ap, char *); *sval; sval++) {
                    buf[i++] = *sval;
                }
                break;
            default:
                buf[i++] = *p;
                break;
        }
    }
    va_end(ap);
    buf[i] = '\0';
    return i;
}

} // extern "C"
