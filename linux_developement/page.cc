#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "PAGE.h"
/****32 bit virtual address space and page size 4kb thus need to select 2^20 pages thus 20 bits for vpn pages and 12 bit for offset****/
page_frame_t * (*virtual_address_page_fn)(int i);
void *virtual_address_page[(uint32_t)pow(2,20)];

static virtual_page_frame_t *Page_entry_formation(uint32_t index)
{
virtual_page_frame_t *ret=malloc(sizeof(virtual_page_frame_t));
if(!ret)
{
    virtual_address_page_fn=Page_entry_formation;
    virtual_address_page[index]=virtual_address_page_fn>>20;
    ptr->virtual_page_frame_num=virtual_address_page_fn>>20;
    ptr->offset=virtual_address_page_fn|0xFFF;
}
return ret;
}

static page_frame_t *page_table_address_translation(virtual_page_frame_t *list)
{
page_frame_t *ret=malloc(sizeof(page_frame_t));
if(!ret)
{
   list->virtual_page_frame_num
}
}





































static bool Page_table_creation()
{
uint32_t i=4;
start_page=NULL;
start_page_frame=NULL;
while(i>0)
{
   if(!start_page)
    {
    start_page=Page_entry_formation(i);
    start_page_frame=page_table_address_translation(start_page);
    start_page->v_page_frame=i;
    curr_page=start_page;
    curr_page_frame=start_page_frame;
    }
   else
   {
    curr_page->next=Page_entry_formation(i);
    start_page_frame=page_table_address_translation(start_page);
    curr_page->v_page_frame=i;
    curr_page_frame=curr_page_frame;
   }
    i--;
}
}
