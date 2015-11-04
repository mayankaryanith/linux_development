#include<stdio.h>
#include<stdlib.h>
#include "TLB.h"
extern virtual_address[4];
int main()
{
   TLB_List_creation();
   int i=4;
   while(i>0)
    {
        bool var;
        var=TLB_fetch_addr(virtual_address[i],i,start);
        if(var)
            printf("hit from main TLB");
        else
            printf("miss from main TLB");
        i--;
    }
return 0;
}
