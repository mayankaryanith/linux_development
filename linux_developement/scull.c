#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include "scull.h"


static int __init scull_init(void)
{
    int i,res = alloc_chrdev_region(dev,0,
                SCULL_DEVICE_NO, SCULL_NAME);
    if(res)
    {
        printk(KERN_ALERT "failed to allocated scull structure");
        return res;
    }
    else
    {
        printk(KERN_ALERT "succesful allocation of scull structure /n %d",MAJOR(*dev));
    }
    scull_devices=kmalloc(SCULL_DEVICE_NO*sizeof(scull_devices),GFP_KERNEL);
    if(!scull_devices)
    {
       return -ENOMEM;
    }
    else
	{
	memset(scull_devices,0,SCULL_DEVICE_NO*sizeof(scull_devices));//did not see why to memset all the values to zero but assuming it to be something
	i=0;
	while(i< SCULL_DEVICE_NO)
	{
	scull_devices->quantum=SCULL_QUANTUM_BYTES;
	scull_devices->qset=SCULL_QSET;
	scull_devices++;	
	i++;
	}
	return 1;//Bad patch
	}
}
static void __exit scull_cleanup(void)
{
	unregister_chrdev_region(*dev,SCULL_DEVICE_NO);
  	kfree(scull_devices);
	printk(KERN_ALERT "goodbye! scull");
}

module_init(scull_init);
module_exit(scull_cleanup);
MODULE_LICENSE("GPL");
