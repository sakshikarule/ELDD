#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#define GPIO_LED            49       // P1.17 i.e. GPIO_49
#define GPIO_SWITCH         115      // P3.19 i.e. GPIO_115
static int led_state = 0;                  // led state (0=OFF, 1=ON)
static int switch_irq = -1;

// switch isr declaration
static irqreturn_t switch_isr(int irq, void *param);

// global variables
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
    ret = gpio_is_valid(GPIO_LED); // check if gpio pin is valid
    if(!ret) {
        pr_info("%s: GPIO LED pin %d is not valid.\n", THIS_MODULE->name, GPIO_LED);
        return -1;
    }
    pr_info("%s: GPIO LED pin %d is valid.\n", THIS_MODULE->name, GPIO_LED);
    ret = gpio_is_valid(GPIO_SWITCH); // check if gpio pin is valid
    if(!ret) {
        pr_info("%s: GPIO SWITCH pin %d is not valid.\n", THIS_MODULE->name, GPIO_SWITCH);
        return -1;
    }
    pr_info("%s: GPIO SWITCH LED pin %d is valid.\n", THIS_MODULE->name, GPIO_SWITCH);

    // acquire access to gpio pin
    ret = gpio_request(GPIO_LED, "my-led");
    pr_info("%s: gpio_request() for GPIO LED pin %d returned %d.\n", THIS_MODULE->name, GPIO_LED, ret);
    if(ret != 0)
        return -1;
    
    ret = gpio_request(GPIO_SWITCH, "my-switch");
    pr_info("%s: gpio_request() for GPIO SWITCH pin %d returned %d.\n", THIS_MODULE->name, GPIO_SWITCH, ret);
    if(ret != 0) {
        gpio_free(GPIO_LED);
        return -1;
    }

    // set LED pin direction as output
    ret = gpio_direction_output(GPIO_LED, led_state);
    pr_info("%s: gpio_direction_output() for GPIO LED pin %d returned %d.\n", THIS_MODULE->name, GPIO_LED, ret);
    if(ret != 0) {
        gpio_free(GPIO_LED);
        return -1;
    }

    // set SWITCH pin direction as input
    ret = gpio_direction_input(GPIO_SWITCH);
    pr_info("%s: gpio_direction_input() for GPIO SWITCH pin %d returned %d.\n", THIS_MODULE->name, GPIO_SWITCH, ret);
    if(ret != 0) {
        gpio_free(GPIO_SWITCH);
        gpio_free(GPIO_LED);
        return -1;
    }

    // get the interrupt number for switch and register its isr
    switch_irq = gpio_to_irq(GPIO_SWITCH);
    pr_info("%s: gpio_to_irq() yield irq num %d for GPIO SWITCH pin %d.\n", THIS_MODULE->name, switch_irq, GPIO_SWITCH);
    ret = request_irq(switch_irq, switch_isr, IRQF_TRIGGER_RISING, "my-switch", NULL);
    if(ret != 0) {
        gpio_free(GPIO_SWITCH);
        gpio_free(GPIO_LED);
        return -1;
    }
    pr_info("%s: request_irq() registered isr for GPIO SWITCH pin %d.\n", THIS_MODULE->name, GPIO_SWITCH);

    // allocate device number
    ret = alloc_chrdev_region(&devno, 0, 1, "pchar");
    pr_info("%s: alloc_chrdev_region() returned=%d.\n", THIS_MODULE->name, ret);
    if(ret != 0) {
        pr_info("%s: alloc_chrdev_region() failed.\n", THIS_MODULE->name);
        gpio_free(GPIO_SWITCH);
        gpio_free(GPIO_LED);
        return ret;
    }
    pr_info("%s: device number = %d/%d.\n", THIS_MODULE->name, MAJOR(devno), MINOR(devno));
    // create device class
    pclass = class_create("pchar_class");
    if(IS_ERR(pclass)) {
        pr_info("%s: class_create() failed.\n", THIS_MODULE->name);
        unregister_chrdev_region(devno, 1);
        gpio_free(GPIO_SWITCH);
        gpio_free(GPIO_LED);
        return -1;
    }
    pr_info("%s: class_create() created device class.\n", THIS_MODULE->name);
    // create devie file
    pdevice = device_create(pclass, NULL, devno, NULL, "pchar%d", 0);
    if(IS_ERR(pdevice)) {
        pr_info("%s: device_create() failed.\n", THIS_MODULE->name);
        class_destroy(pclass);
        unregister_chrdev_region(devno, 1);
        gpio_free(GPIO_SWITCH);
        gpio_free(GPIO_LED);
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
        gpio_free(GPIO_SWITCH);
        gpio_free(GPIO_LED);
        return ret;
    }
    pr_info("%s: cdev_add() added cdev in kernel & returned=%d\n", THIS_MODULE->name, ret);
    pr_info("%s: in module init fn - in interrupt context=%s\n", THIS_MODULE->name, (in_irq()?"yes":"no"));
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
    free_irq(switch_irq, NULL);
    pr_info("%s: free_irq() unregistered switch_isr.\n", THIS_MODULE->name);
    gpio_free(GPIO_SWITCH);
    pr_info("%s: gpio_free() released GPIO SWITCH pin %d.\n", THIS_MODULE->name, GPIO_SWITCH);
    gpio_free(GPIO_LED);
    pr_info("%s: gpio_free() released GPIO LED pin %d.\n", THIS_MODULE->name, GPIO_LED);
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
    char kbuf[2] = "";
    int ret;
    pr_info("%s: pchar_write() called.\n", THIS_MODULE->name);
    ret = copy_from_user(kbuf, pubuf, 1);
    if(ret == 0) { // copy is successful
        if(kbuf[0] == '0')
            led_state = 0;
        else
            led_state = 1;
        gpio_set_value(GPIO_LED, led_state);
    }
    return ubufsize;
}

ssize_t pchar_read(struct file *pfile, char __user *pubuf, size_t ubufsize, loff_t *poffset) {
    pr_info("%s: pchar_read() called.\n", THIS_MODULE->name);
    return 0;
}

// switch isr implementation -- executes in interrupt context
irqreturn_t switch_isr(int irq, void *param) {
    pr_info("%s: switch_isr() called: in interrupt context=%s.\n", THIS_MODULE->name, (in_irq()?"yes":"no"));
    // toggle the led
    led_state = !led_state;
    gpio_set_value(GPIO_LED, led_state);
    return IRQ_HANDLED;
}


module_init(pchar_init);
module_exit(pchar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nilesh Ghule <nilesh@sunbeaminfo.com>");
MODULE_DESCRIPTION("BBB GPIO Linux device driver");
