#ifndef __PAGE_H__
#define __PAGE_H__
#include<stdint.h>
#include "vpn.h"
typedef struct page_frame
{
uint32_t v_page_frame ;//actuall address in the memory
uint8_t  protection_bit:3;
uint8_t  present_bit:1;
uint8_t  dirty_bit:1;
uint8_t  reference_bit:10
}page_frame_t;

typedef struct virtual_page_frame
{
uint8_t virtual_page_frame_num ;
uint8_t offset;
struct virtual_page_frame *next;
}virtual_page_frame_t;

virtual_page_frame_t *start_page,*curr_page;//Base register for virtual page frame number
page_frame_t *start_page_frame,*curr_page_frame;//Base register for virtual page frame number
static void Page_table_creation();
static void Page_table_updation(page_frame_t *list);
static virtual_page_frame_t *Page_entry_formation(int index);
static page_frame_t *page_table_address_translation(virtual_page_frame_t *list);
static bool page_table_fetch_addr(uint64_t address, int index, page_frame_t *list);
#endif
