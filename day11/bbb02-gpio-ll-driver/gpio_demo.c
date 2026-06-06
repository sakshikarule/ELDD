#include <linux/module.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/ioport.h>

#define GPIO1_BASE_ADDR     0x4804C000
#define GPIO1_ADDR_RANGE    4096
#define GPIO_OE             0x134
#define GPIO_SETDATAOUT     0x194
#define GPIO_CLEARDATAOUT   0x190
#define GPIO_LED            (1 << 16)       // P1.16 i.e. GPIO_48
static void *gpio_base;

static int __init gpio_init(void) {
    struct resource *res;
    int i;
    pr_info("%s: gpio_init() called.\n", THIS_MODULE->name);
    // res = request_mem_region(GPIO1_BASE_ADDR, GPIO1_ADDR_RANGE, "sunbeam-gpio");
    // if(IS_ERR_OR_NULL(res)) {
    //     pr_info("%s: request_mem_region() failed to access gpio1.\n", THIS_MODULE->name);
    //     return -1;
    // }
    // pr_info("%s: request_mem_region() acquired GPIO1 addresses.\n", THIS_MODULE->name);
    
    // convert the physical gpio port addre to virtual address
    gpio_base = ioremap(GPIO1_BASE_ADDR, GPIO1_ADDR_RANGE);
    if(IS_ERR_OR_NULL(gpio_base)) {
        pr_info("%s: ioremap() failed to get gpio1 address.\n", THIS_MODULE->name);
        return -1;
    }
    pr_info("%s: ioremap() got gpio1 address.\n", THIS_MODULE->name);

    // enable gpio output for p1.16 pin
    u32 oe_reg = ioread32(gpio_base + GPIO_OE); // read OE regr
    oe_reg &= ~GPIO_LED;  // clear bit16 of value
    iowrite32(oe_reg, gpio_base + GPIO_OE); // write OE regr
    pr_info("%s: set P1.16 as output pin.\n", THIS_MODULE->name);
    for(i=1; i<=10; i++) {
        // make p1.16 high - led on
        iowrite32(GPIO_LED, gpio_base + GPIO_SETDATAOUT); 
        pr_info("%s: set P1.16 to 1 (led on).\n", THIS_MODULE->name);
        msleep(500); // delay
        // make p1.16 low - led off
        iowrite32(GPIO_LED, gpio_base + GPIO_CLEARDATAOUT); 
        pr_info("%s: set P1.16 to 0 (led off).\n", THIS_MODULE->name);
        msleep(500); // delay
    }
    return 0;
}

static void __exit gpio_exit(void) {
    pr_info("%s: gpio_exit() called.\n", THIS_MODULE->name);
    // release_mem_region(GPIO1_BASE_ADDR, GPIO1_ADDR_RANGE);
    // pr_info("%s: release_mem_region() released GPIO1 addresses.\n", THIS_MODULE->name);
    iounmap(gpio_base);
    pr_info("%s: iounmap() released gpio1 address.\n", THIS_MODULE->name);
}

module_init(gpio_init);
module_exit(gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nilesh Ghule <nilesh@sunbeaminfo.com>");
MODULE_DESCRIPTION("GPIO Direct Hw access for BBB");
