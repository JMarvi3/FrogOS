#include <stdint.h>
#include <stdio.h>

const char *inet_ntoa(const void *in_addr)
{
	static char buff[16];
	const uint8_t *addr=in_addr;
	sprintf(buff,"%d.%d.%d.%d",addr[0],addr[1],addr[2],addr[3]);
	return buff;
}
