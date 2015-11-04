#ifndef __VPN_H__
#define __VPN_H__
#include<stdint.h>
#define VPN_MASK 0xFFFFFFFF//Larger memory segment hence smaller VPN_MASK
#define OFFSET   0xFFFFFFFF
#define SHIFT    32
typedef struct vpn
{
void * virtual_address;
}vpn_t;






#endif
