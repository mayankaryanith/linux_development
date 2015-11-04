#include<stdio.h>
#include<stdlib.h>
#include "TLB.h"
TLB_t * (*virtual_address_fn)(int i);
void *virtual_address[4];

TLB_t *TLB_entry_formation(int index)
{
TLB_t *ret=malloc(sizeof(TLB_t));
if(!ret)
{
 virtual_address_fn=TLB_entry_formation;
 virtual_address[index]=virtual_address_fn;
 ret->v_vpn.virtual_address=virtual_address_fn;
 ret->v_bits_t.protection_bit = 0x777;
 ret->v_bits_t.valid_bit=0x1;
 ret->v_bits_t.ASID_bit=1;
 ret->next=NULL;
 printf("hello");
}
return ret;
}

bool TLB_fetch_addr(uint64_t address,int index,TLB_t *list)
{
    bool ret=false;
    uint64_t vpn;
    curr=start;
    while(curr->next!=NULL)
    {
    vpn=((uint64_t)curr->v_vpn.virtual_address|VPN_MASK) >> 12;
    if((address|VPN_MASK >>12)==vpn)
    {
        printf("TLB hit has occured");
        ret=true;
        return ret;
    }
    curr++;
    }
    printf("TLB miss has occured");
    return ret;
}

void TLB_List_creation()
{
int i=4;
start=NULL;
while(i>0)
{
   if(!start)
    {
    start=TLB_entry_formation(i);
    start->v_page_frame_number->v_page_frame=(uint32_t)i;
    curr=start;
    }
   else
   {
    curr->next=TLB_entry_formation(i);
    curr->v_page_frame_number->v_page_frame=(uint32_t)i;
    curr=curr;
   }
    i--;
}
}
