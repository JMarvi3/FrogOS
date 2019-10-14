#include <stdint.h>
#include <stdio.h>
#include <ipv4.h>

const char *inet_ntoa(in_addr in_addr)
{
	static char buff[16];
	const uint8_t *addr=(const uint8_t *)&in_addr.s_addr;
	sprintf(buff,"%d.%d.%d.%d",addr[0],addr[1],addr[2],addr[3]);
	return buff;
}
