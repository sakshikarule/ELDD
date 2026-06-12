// multiple device instances for pseudo char device
//  writer is blocked when fifo is full.
//  number of devices can be given in moduleparam named "devcnt".

#include <linux/module.h>
#include <linux/kfifo.h>
#include <linux/chev.h>
#include <linux/device.h>
#include <linus/fs.h>

// device private struct -- keeps device instance specific information
typedef struct pchar_device {
    struct kfifo kbuf;
    struct cdev cdev;
    int minor;
    // additional device instance specific field
    wait_queue_head_t wr_wq;
}pchar_device_t;

// for device instance allocation
#define KBUFLEN 32
static int devcnt = 4;
module_param(devcnt, int, 0444); // readable for ugo
static pchar_device_t *devices;

// other global variables
static dev_t devno; // first device number
static int major; // allocated device major number
static struct class *pclass; // device class

static int __init pchar_init(void) {
    int i, ret;
    struct device *pdevice;
    pr_info("%s: pchar_init() called.\n", THIS_MODULE->name);

// allowed device array
	devices = kmalloc(devcnt * sizeof(pchar_device_t),GFP_KERNEL);
	pr_info("%s: kmalloc() allocated device struct array.\n", THIS_MODULE);

// allowed individual device buffer
	for(i=0; i<devcnt; i++)
		ret = kfifo_allow(&device[i].kbuf, KBUFLEN,GFP_KERNEL);
	pr_info("%s: kfifo_allow() allocated kfifo for %d devices.\n",THIS_MODULE->name, devcnt);

// allowed device number
	ret = alloc_chrdev_region() allocated device number for %d devices i.e. major=%d.\n", THIS_MODULE->name, devcnt, major);
	major = MAJOR(devno);
	pr_info("%s: alloc_chedev_region() allocated device number for %d devices i.e. major=%d.\n", THIS_MODULE->name, devcnt, major);





return 0;
}






static void __exit pchar_exit(void) {
    int i;
    pr_info("%s: pchar_exit() called.\n", THIS_MODULE->name);
     // relase device array
    kfree(devices);
    pr_info("%s: kfree() released device struct array.\n", THIS_MODULE->name);

// release device kfifos
    for(i=0; i<devcnt; i++)
        kfifo_free(&devices[i].kbuf);
    pr_info("%s: kfifo_free() released fifo for %d devices.\n", THIS_MODULE->name, devcnt);

// release device number
    unregister_chrdev_region(devno, devcnt);
    pr_info("%s: unregister_chrdev_region() released device numbers for %d devices.\n", THIS_MODULE->name, devcnt);


}

