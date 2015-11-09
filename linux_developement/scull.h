#ifndef __SCULL_H__
#define __SCULL_H__
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/fs.h>
#define SCULL_DEVICE_NO 4
#define SCULL_PIPE_NO   4
#define SCULL_QUANTUM_BYTES 4000
#define SCULL_QSET          1000
#define SCULL_NAME          "scull"

struct scull_qset
{
    void **data;
    struct scull_qset *next;
};
struct scull_dev
{
    struct scull_qset *data;
    int quantum;
    int qset;
    unsigned long size;
    unsigned int access_key;
    struct semaphore sem;
    struct cdev cdev;
};
dev_t *dev;
struct scull_dev *scull_devices;
#endif
