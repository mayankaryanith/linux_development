#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include "scull.h"

ssize_t scull_read (struct file *filp, char __user *buffer, size_t x, loff_t *y)
{
ssize_t res;
return res;
}

ssize_t scull_write(struct file *filp, const char __user *buffer, size_t x, loff_t *y)
{
ssize_t res;
return res;

}

int scull_release(struct inode *x, struct file *filp)
{
int res;
return res;
}

static void scull_trim(struct scull_dev *dev)
{
    
    dev->size=0;
}

struct file_operations scull_fops=
{
    .owner   = THIS_MODULE,
    .read    = scull_read,
    .write   = scull_write,
    .release = scull_release
};

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
            init_mutex(&scull_devices->sem);
            cdev_init(&scull_devices->cdev,&scull_fops);
            res=cdev_add(&scull_devices->cdev,MKDEV(MAJOR(*dev),MINOR(*dev)+i),1);
            scull_devices++;
            if(res)
            {
                printk(KERN_ALERT "cdev addition failed");
                return res;
            }
            else
            {
                printk(KERN_ALERT "cdev addition successful");
            }
            i++;
        }
        return 0;//removed the bad patch
    }
}
static void __exit scull_cleanup(void)
{
    int i=0;
    scull_devices=scull_devices-SCULL_DEVICE_NO;
    unregister_chrdev_region(*dev,SCULL_DEVICE_NO);
    while(i<SCULL_DEVICE_NO)
    {
        //I dont see the reason why do we need to trim the size of scull devices to 0 as we are alredy deleting it
        scull_trim(scull_devices);
        cdev_del(&scull_devices->cdev);
        i++;
    }
    kfree(scull_devices);
    printk(KERN_ALERT "goodbye! scull");
}

module_init(scull_init);
module_exit(scull_cleanup);
MODULE_LICENSE("GPL");
