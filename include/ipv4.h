#ifndef _IPV4_H
#define _IPV4_H

#include <stdint.h>

typedef struct in_addr { uint32_t s_addr; } in_addr;

const char *inet_ntoa(struct in_addr addr);

#endif
