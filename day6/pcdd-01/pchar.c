#include <linux/module.h> //Essential header for every linux module. it conation macros like module_init, module_exit, & module metadata token.
#include <linux/fs.h> // provied Fun & structure realated to the Virtual File system,like struct file-oper & alloc_chrdev_region.
#include <linux/cdev.h> //Manages struct cdev, the kernel's represtations of a character device.
#include <linux/device.h> // supports the Linux Device Model. provieds macros/fun like class_creates &device_create for automated device file management via udev.
#include <linux/cdev.h>
#include <linux/uaccess.h>//Delivers momory protection mechanisms (copy_to_user, copy_from_user) to bridge user-space addresses safely.

static char buf[32]; // pchar device (pseudo device)
static dev_t devno; // pchar device's device number
static struct class *pclass; // pchar device's device class
static struct cdev pchar_cdev; // pchar device's cdev

// pchar device operation
static int pchar_open(struct inode *pinode, struct file *pfile);
static int pchar_close(struct inode  *pinode, struct file *pfile);
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
	
	// allocate device number
	ret = alloc_chrdev_region(&devno, 0, 1, "pchar");
	pr_info("%s: alloc_chrdev_region() returned=%d.\n", THIS_MODULE->name, ret);
	pr_info("%s: device number = %d/%d.\n", THIS_MODULE->name, MAJOR(devno), MINOR(devno));

	//create device class
	pclass = class_create("pchar_class");
	pr_info("%s: class_create() created devices class.\n", THIS_MODULE->name);

	//create device file 
	pdevice = device_create(pclass, NULL, devno, NULL, "pchar%d", 0);
	pr_info("%s: device_create() created device file.\n", THIS_MODULE->name);

	// init cdev and register in kernel
	cdev_init(&pchar_cdev, &pchar_fops);
	pchar_cdev.owner = THIS_MODULE;
	ret = cdev_add(&pchar_cdev, devno, 1);
	pr_info("%s: cdev_add() added cdev in kernel &returned=%d\n", THIS_MODULE->name,ret);
	return 0;
}

static void __exit pchar_exit(void) {

	pr_info("%s: pchar_exit() called.\n", THIS_MODULE->name);
	//deregister cdev from kernel
	cdev_del(&pchar_cdev);
	pr_info("%s: cdev_del() removed cdev from kernel\n",THIS_MODULE->name);

	//destroy device file
    device_destroy(pclass, devno);
	pr_info("%s: device_destroy() destroyed device file\n", THIS_MODULE->name);
	//destroy device class
	class_destroy(pclass);
	pr_info("%s: class_destroy() destroyed devices class\n", THIS_MODULE->name);

	//release device number
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

ssize_t pchar_write(struct file *pfile, const char __user *pubuf, size_t bufsize, loff_t *poffset) {
    int count;
    pr_info("%s: pchar_write() called.\n", THIS_MODULE->name);
    count = bufsize - copy_from_user(buf, pubuf, bufsize);
    return count;
}

ssize_t pchar_read(struct file *pfile, char __user *pubuf, size_t bufsize, loff_t *poffset) {
    int count;
    pr_info("%s: pchar_read() called.\n", THIS_MODULE->name);
    count = bufsize - copy_to_user(pubuf, buf, bufsize);
    return count;
}

module_init(pchar_init);
module_exit(pchar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sakshi karule <sakshikarule150@gmail.com>");
MODULE_DESCRIPTION("Pseudo char device driver");
