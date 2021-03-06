#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include <asm/uaccess.h>
#include "scull.h"

int scull_trim(struct scull_dev *dev)
{
    struct scull_qset *dataptr,*next;
    int i=0;
    dataptr=dev->data;
    next=dataptr;
    while(dataptr!=NULL)
    {
        for(i=0;i<dev->qset;i++)
            kfree(dataptr->data[i]);
        next=dataptr->next;
        kfree(dataptr);
        dataptr->data=NULL;
        dataptr=next;
    }
    dev->size=0;
    dev->quantum=SCULL_QUANTUM_BYTES;
    dev->qset=SCULL_QSET;
    dev->data=NULL;
    return 0;
}

int scull_open(struct inode *inode, struct file *file)
{
struct scull_dev *s_dev;
s_dev=container_of(inode->i_cdev,struct scull_dev,cdev);
file->private_data=s_dev;
if((file->f_flags & O_ACCMODE)== O_WRONLY)
{
    scull_trim(s_dev);
}
return 0;
}

ssize_t scull_read (struct file *filp, char __user *buffer, size_t x, loff_t *y)
{
ssize_t res=0;//Bad patch
return res;
}

ssize_t scull_write(struct file *filp, const char __user *buffer, size_t x, loff_t *y)
{
ssize_t res=0;//Bad patch
return res;

}

int scull_release(struct inode *x, struct file *filp)
{
    return 0;
}


struct file_operations scull_fops =
{
    .owner   = THIS_MODULE,
    .read    = scull_read,
    .write   = scull_write,
    .release = scull_release,
    .open    = scull_open
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
            mutex_init(&scull_devices->mutex);
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
