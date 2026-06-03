#include <linux/module.h>
#include <linux/kfifo.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/slab.h>


typedef struct pchar_device {
	struct kfifo kbuf;
	struct cdev cdev;
	int minor;
	// additional device instance specifi field
} pchar_device_t;

// for device instance allocation
#define KBUFLEN 32
static int devcnt = 4;
static pchar_device_t *devices;

// other global varables
static dev_t devno;  //first device number
static int major;    // allocated device major number
static struct class *pclass; //device class

//device operation
static int pchar_open(struct inode *pinode, struct file *pfile);
static int pchar_close(struct inode *pinode, struct file *pfile);
static ssize_t pchar_write(struct file *pfile, const char __user *pubuf, size_t ubufsize, loff_t *poffset);
static ssize_t pchar_read(struct file *pfile, char __user *pubuf, size_t ubufsize, loff_t *poffset);

static struct file_operations pchar_fops = {
	.owner = THIS_MODULE,
	.open = pchar_open,
	.read = pchar_read,
	.write = pchar_write
};

static int __init pchar_init(void)
{
	int i, ret;
	struct device *pdevice;
    pr_info("%s: pchar_init() called.\n", THIS_MODULE->name);

	// allocate device array
	devices = kmalloc(devcnt * sizeof(pchar_device_t), GFP_KERNEL);
	pr_info("%s: kmalloc() allocated device struct array.\n", THIS_MODULE->name);
    
	// allocate individual device buffer
	for(i=0; i<devcnt; i++)
		ret = kfifo_alloc(&devices[i].kbuf, KBUFLEN, GFP_KERNEL); 
	pr_info("%s: kfifo_alloc() allocated kfifo for %d devices.\n", THIS_MODULE->name, devcnt);
    
	//allocate device number
	ret = alloc_chrdev_region(&devno, 0, devcnt, "pchar");
	major = MAJOR(devno);
	pr_info("%s: alloc_chrdev_region() allocated device numbers for %d devices i.e. major=%d.\n", THIS_MODULE->name, devcnt, major);

    // create device class
	pclass = class_create("pchar_class");
	pr_info("%s: class_create() created device class.\n", THIS_MODULE->name);

    // create device files
	for(i=0; i<devcnt; i++) {
		devices[i].minor = i;
		dev_t devnum = MKDEV(major, i);
		pdevice = device_create(pclass, NULL, devnum, NULL, "pchar%d", i);
	}
	pr_info("%s: device_create() created devices files %d devices.\n", THIS_MODULE->name, devcnt);
   
	// init cdevs and add in kernel
	for(i=0; i<devcnt; i++) {
		cdev_init(&devices[i].cdev, &pchar_fops);
	    dev_t devnum = MKDEV(major, i);
		ret = cdev_add(&devices[i].cdev, devnum, 1);
      }
	pr_info("%s: cdev_add() added cdevs for %d devices.\n", THIS_MODULE->name, devcnt);
	return 0;
}

static void __exit pchar_exit(void)
{
	int i;
	pr_info("%s: pchar_exit() called.\n", THIS_MODULE->name);
    
	// release cdevs
	for(i=0 ; i<devcnt; i++)
		cdev_del(&devices[i].cdev);
	pr_info("%s: cdev_del() removed cdevs for %d devices.\n", THIS_MODULE->name, devcnt);
   
	//destroy device files
	for(i=0; i<devcnt; i++) {
		dev_t devnum = MKDEV(major, i);
	    device_destroy(pclass, devnum);
    }
	pr_info("%s: device_destroy() destroyed device files for %d devices.\n", THIS_MODULE->name, devcnt);
  
	// destroy device class
	class_destroy(pclass);
	pr_info("%s: class_destroy() destroyed device class.\n", THIS_MODULE->name);

	// release device number
	unregister_chrdev_region(devno, devcnt);
	pr_info("%s: unregister_chrdev_region() released device number for %d devices.\n", THIS_MODULE->name, devcnt);
    
	// release device kfifos
	for(i=0; i<devcnt; i++) 
		kfifo_free(&devices[i].kbuf);
	pr_info("%s: kfifo_free() released fifo for %d devices.\n", THIS_MODULE->name, devcnt);

	//relase device array
    kfree(devices);
    pr_info("%s: kfree() released device struct array.\n", THIS_MODULE->name);
}

int pchar_open(struct inode *pinode, struct file *pfile) {
	// find the private struct for the device containing our cdev
    pchar_device_t *dev = container_of(pinode->i_cdev,pchar_device_t, cdev);
	 // keep its address inside the struct file (i.e. open file table entry)
	pfile->private_data = dev;
	pr_info("%s: pcahr_open() called for pchar%d.\n",THIS_MODULE->name, dev->minor);
	return 0;
}

int pchar_close(struct inode *pinode, struct file *pfile) {
	pchar_device_t *dev = (pchar_device_t *)pfile->private_data;
	pr_info("%s: pchar_close() called for pchar%d.\n",THIS_MODULE->name, dev->minor);
	return 0;
}

ssize_t pchar_write(struct file *pfile, const char __user *pubuf, size_t ubufsize, loff_t *poffset) {
	pchar_device_t *dev = (pchar_device_t *)pfile->private_data;
	int ret, nbytes;
	pr_info("%s: pchar_write() called for pchar%d.\n", THIS_MODULE->name, dev->minor);
	ret = kfifo_from_user(&dev->kbuf, pubuf, ubufsize,&nbytes);
	if(ret != 0) {
		 pr_info("%s: kfifo_from_user() failed.\n", THIS_MODULE->name);
        return ret;
    }
    return nbytes;
}

ssize_t pchar_read(struct file *pfile, char __user *pubuf, size_t ubufsize, loff_t *poffset) {
	pchar_device_t *dev = (pchar_device_t *)pfile->private_data;
	int ret, nbytes;
	pr_info("%s: pchar_read() called for pchar%d.\n", THIS_MODULE->name, dev->minor);
	ret = kfifo_to_user(&dev->kbuf, pubuf, ubufsize, &nbytes);
	if(ret != 0) {
		pr_info("%s: kfifo_to_user() failed.\n", THIS_MODULE->name);
		return ret;
	}
	return nbytes;

}

module_init(pchar_init);
module_exit(pchar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sakshi karule");
MODULE_DESCRIPTION("Pseudo char device driver with multiple device instances.");
