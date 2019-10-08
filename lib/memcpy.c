#include <string.h>

void *memcpy(void *dest, const void *src, size_t n)
{
  __asm__("cld; rep movsb"::"c"(n),"S"(src),"D"(dest));
  return dest;
}
