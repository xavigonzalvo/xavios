#include "string.h"

extern "C"
{

  void *memcpy(void *dest, const void *src, size_t n)
  {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    for (size_t i = 0; i < n; i++)
    {
      d[i] = s[i];
    }

    return dest;
  }

  int strncmp(const char *str1, const char *str2, size_t n)
  {
    while (n--)
    {
      if (*str1 != *str2)
      {
        return *(unsigned char *)str1 - *(unsigned char *)str2;
      }
      if (!*str1)
      { // If end of string is reached
        return 0;
      }
      str1++;
      str2++;
    }
    return 0;
  }

  void strncpy(char *dest, const char *src, size_t n)
  {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++)
    {
      dest[i] = src[i];
    }
    // If the source string is shorter than n, fill the rest with null bytes
    for (; i < n; i++)
    {
      dest[i] = '\0';
    }
  }

} // extern "C"
