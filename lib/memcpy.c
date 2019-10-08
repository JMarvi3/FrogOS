#include <string.h>

void *memcpy(void *dest, const void *src, size_t n)
{
  register size_t count=n;
  register unsigned char *d=(unsigned char*)dest;
  register const unsigned char *s=(const unsigned char*)src;
  while(count--) *d++=*s++;
  return dest;
}
