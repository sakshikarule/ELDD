#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

// advanced char device operation -- lseek().

// macro
#define MIN(a,b) ((a) < (b) ? (a) : (b))

// global variables
#define KBUFLEN 32
static char kbuf[KBUFLEN]; // pchar device (pseudo device)
static dev_t devno; // pchar device's device number
static struct class *pclass; // pchar device's device class
static struct cdev pchar_cdev; // pchar device's cdev

// pchar device operations
static int pchar_open(struct inode *pinode, struct file *pfile);
static int pchar_close(struct inode *pinode, struct file *pfile);
static ssize_t pchar_write(struct file *pfile, const char __user *pubuf, size_t bufsize, loff_t *poffset);
static ssize_t pchar_read(struct file *pfile, char __user *pubuf, size_t bufsize, loff_t *poffset);
static loff_t pchar_lseek(struct file *pfile, loff_t offset, int origin); 

static struct file_operations pchar_fops = // pchar device's operations
{
    .owner = THIS_MODULE,
    .open = pchar_open,
    .release = pchar_close,
    .read = pchar_read,
    .write = pchar_write,
    .llseek = pchar_lseek
};

static int __init pchar_init(void) {
    int ret;
    struct device *pdevice;
    pr_info("%s: pchar_init() called.\n", THIS_MODULE->name);
    // allocate device number
    ret = alloc_chrdev_region(&devno, 0, 1, "pchar");
    pr_info("%s: alloc_chrdev_region() returned=%d.\n", THIS_MODULE->name, ret);
    if(ret != 0) {
        pr_info("%s: alloc_chrdev_region() failed.\n", THIS_MODULE->name);
        return ret;
    }
    pr_info("%s: device number = %d/%d.\n", THIS_MODULE->name, MAJOR(devno), MINOR(devno));
    // create device class
    pclass = class_create("pchar_class");
    if(IS_ERR(pclass)) {
        pr_info("%s: class_create() failed.\n", THIS_MODULE->name);
        unregister_chrdev_region(devno, 1);
        return -1;
    }
    pr_info("%s: class_create() created device class.\n", THIS_MODULE->name);
    // create devie file
    pdevice = device_create(pclass, NULL, devno, NULL, "pchar%d", 0);
    if(IS_ERR(pdevice)) {
        pr_info("%s: device_create() failed.\n", THIS_MODULE->name);
        class_destroy(pclass);
        unregister_chrdev_region(devno, 1);
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
    int avail, to_write, nbytes;
    pr_info("%s: pchar_write() called.\n", THIS_MODULE->name);
    avail = KBUFLEN - *poffset;
    to_write = MIN(avail, ubufsize);
    if(to_write <= 0) {
        pr_info("%s: pchar_write(): device kbuf is full.\n", THIS_MODULE->name);
        //return 0; // option1 - zero bytes written in device
        return -ENOSPC; // option2 - return error i.e. no space on device
    }
    nbytes = to_write - copy_from_user(kbuf + *poffset, pubuf, to_write);
    *poffset += nbytes;
    return nbytes;
}

ssize_t pchar_read(struct file *pfile, char __user *pubuf, size_t ubufsize, loff_t *poffset) {
    int avail, to_read, nbytes;
    pr_info("%s: pchar_read() called.\n", THIS_MODULE->name);
    avail = KBUFLEN - *poffset;
    if(avail <= 0) {
        pr_info("%s: pchar_read(): device kbuf fully read.\n", THIS_MODULE->name);
        return 0; // end-of-device reached
    } 
    to_read = MIN(avail, ubufsize);
    nbytes = to_read - copy_to_user(pubuf, kbuf + *poffset, to_read);
    *poffset += nbytes;
    return nbytes;
}

loff_t pchar_lseek(struct file *pfile, loff_t offset, int origin) {
    loff_t newpos;
    pr_info("%s: pchar_lseek() called.\n", THIS_MODULE->name);
    // calculate new possible pos
    switch(origin) {
        case SEEK_SET:
            newpos = 0 + offset;
            break;
        case SEEK_CUR:
            newpos = pfile->f_pos + offset;
            break;
        case SEEK_END:
            newpos = KBUFLEN + offset;
            break;
        default:
            pr_info("%s: pchar_lseek() invalid origin arg.\n", THIS_MODULE->name);
            return -EINVAL;
    }
    // adjust newpos if it is beyond the range
    if(newpos > KBUFLEN)
        newpos = KBUFLEN;
    if(newpos < 0)
        newpos = 0;
    // set the new file pos
    pfile->f_pos = newpos;
    return newpos;
}

module_init(pchar_init);
module_exit(pchar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sakshi karule ");
MODULE_DESCRIPTION("Pseudo char device driver");
