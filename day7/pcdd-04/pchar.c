#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/kfifo.h>

// basic char device operation -- using kfifo instead char kbuf[size];

// macro
#define MIN(a,b) ((a) < (b) ? (a) : (b))

// global variables
#define KBUFLEN 32
static struct kfifo kbuf; // pchar device (pseudo device)
static dev_t devno; // pchar device's device number
static struct class *pclass; // pchar device's device class
static struct cdev pchar_cdev; // pchar device's cdev

// pchar device operations
static int pchar_open(struct inode *pinode, struct file *pfile);
static int pchar_close(struct inode *pinode, struct file *pfile);
static ssize_t pchar_write(struct file *pfile, const char __user *pubuf, size_t bufsize, loff_t *poffset);
static ssize_t pchar_read(struct file *pfile, char __user *pubuf, size_t bufsize, loff_t *poffset);

static struct file_operations pchar_fops = // pchar device's operations
{
    .owner = THIS_MODULE,
    .open = pchar_open,
    .release = pchar_close,
    .read = pchar_read,
    .write = pchar_write
};

static int __init pchar_init(void) {
    int ret;
    struct device *pdevice;
    pr_info("%s: pchar_init() called.\n", THIS_MODULE->name);
    // allocate kfifo buffer
    ret = kfifo_alloc(&kbuf, KBUFLEN, GFP_KERNEL);
    if(ret != 0) {
        pr_info("%s: kfifo_alloc() failed to allocate buffer.\n", THIS_MODULE->name);
        return ret;    
    }
    pr_info("%s: kfifo_alloc() allocate buffer of size %d.\n", THIS_MODULE->name, KBUFLEN);
    // allocate device number
    ret = alloc_chrdev_region(&devno, 0, 1, "pchar");
    pr_info("%s: alloc_chrdev_region() returned=%d.\n", THIS_MODULE->name, ret);
    if(ret != 0) {
        pr_info("%s: alloc_chrdev_region() failed.\n", THIS_MODULE->name);
        kfifo_free(&kbuf);
        return ret;
    }
    pr_info("%s: device number = %d/%d.\n", THIS_MODULE->name, MAJOR(devno), MINOR(devno));
    // create device class
    pclass = class_create("pchar_class");
    if(IS_ERR(pclass)) {
        pr_info("%s: class_create() failed.\n", THIS_MODULE->name);
        unregister_chrdev_region(devno, 1);
        kfifo_free(&kbuf);
        return -1;
    }
    pr_info("%s: class_create() created device class.\n", THIS_MODULE->name);
    // create devie file
    pdevice = device_create(pclass, NULL, devno, NULL, "pchar%d", 0);
    if(IS_ERR(pdevice)) {
        pr_info("%s: device_create() failed.\n", THIS_MODULE->name);
        class_destroy(pclass);
        unregister_chrdev_region(devno, 1);
        kfifo_free(&kbuf);
        return -1;
    }
    pr_info("%s: device_create() created device file.\n", THIS_MODULE->name);
    // init cdev and register in kernel
    cdev_init(&pchar_cdev, &pchar_fops);
    pchar_cdev.owner = THIS_MODULE;
    ret = cdev_add(&pchar_cdev, devno, 1);
    if(ret < 0) {
        pr_info("%s: cdev_add() failed.\n", THIS_MODULE->name);
        device_destroy(pclass, devno);
        class_destroy(pclass);
        unregister_chrdev_region(devno, 1);
        kfifo_free(&kbuf);
        return ret;
    }
    pr_info("%s: cdev_add() added cdev in kernel & returned=%d\n", THIS_MODULE->name, ret);
    return 0;
}

static void __exit pchar_exit(void) {
    pr_info("%s: pchar_exit() called.\n", THIS_MODULE->name);
    // deregister cdev from kernel
    cdev_del(&pchar_cdev);
    pr_info("%s: cdev_del() removed cdev from kernel\n", THIS_MODULE->name);
    // destroy device file
    device_destroy(pclass, devno);
    pr_info("%s: device_destroy() destroyed device file.\n", THIS_MODULE->name);
    // destroy device class
    class_destroy(pclass);
    pr_info("%s: class_destroy() destroyed device class.\n", THIS_MODULE->name);
    // release device number
    unregister_chrdev_region(devno, 1);
    pr_info("%s: unregister_chrdev_region() released the device number.\n", THIS_MODULE->name);
    // free kfifo buffer
    kfifo_free(&kbuf);
    pr_info("%s: kfifo_free() released buffer of kfifo.\n", THIS_MODULE->name);
}

int pchar_open(struct inode *pinode, struct file *pfile) {
    pr_info("%s: pchar_open() called.\n", THIS_MODULE->name);
    return 0;
}

int pchar_close(struct inode *pinode, struct file *pfile) {
    pr_info("%s: pchar_close() called.\n", THIS_MODULE->name);
    return 0;
}

ssize_t pchar_write(struct file *pfile, const char __user *pubuf, size_t ubufsize, loff_t *poffset) {
    int ret, nbytes=0;
    pr_info("%s: pchar_write() called.\n", THIS_MODULE->name);
    ret = kfifo_from_user(&kbuf, pubuf, ubufsize, &nbytes);
    if(ret < 0) {
        pr_info("%s: kfifo_from_user() failed.\n", THIS_MODULE->name);
        return ret; // -EFAULT
    }
    return nbytes;
}

ssize_t pchar_read(struct file *pfile, char __user *pubuf, size_t ubufsize, loff_t *poffset) {
    int ret, nbytes;
    pr_info("%s: pchar_read() called.\n", THIS_MODULE->name);
    ret = kfifo_to_user(&kbuf, pubuf, ubufsize, &nbytes);
    if(ret < 0) {
        pr_info("%s: kfifo_to_user() failed.\n", THIS_MODULE->name);
        return ret; // -EFAULT
    }
    return nbytes;
}

module_init(pchar_init);
module_exit(pchar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sakshikarule");
MODULE_DESCRIPTION("Pseudo char device driver");
