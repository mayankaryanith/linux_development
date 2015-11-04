#ifndef __TLB_H__
#define __TLB_H__
#include "vpn.h"
#include "page.h"
#include<stdint.h>
#include<stdbool.h>

typedef struct bits
{
uint32_t protection_bit;
uint32_t valid_bit;
uint32_t ASID_bit;
}bits_t;
typedef struct TLB
{
vpn_t         v_vpn;
page_frame_t *v_page_frame_number;
bits_t       v_bits_t;
struct TLB          *next;
}TLB_t;0
TLB_t *start,*curr;//Base register 
bool  TLB_fetch_addr(uint64_t address,int index,TLB_t *list);
TLB_t *TLB_entry_formation(int index);
void TLB_List_creation();


#endif
