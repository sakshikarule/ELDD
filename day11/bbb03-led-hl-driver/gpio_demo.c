#include <linux/module.h>
#include <linux/delay.h>
#include <linux/gpio.h>

#define GPIO_LED            48       // P1.16 i.e. GPIO_48

static int __init gpio_init(void) {
    int i, ret;
    pr_info("%s: gpio_init() called.\n", THIS_MODULE->name);
    ret = gpio_is_valid(GPIO_LED); // check if gpio pin is valid
    if(!ret) {
        pr_info("%s: GPIO pin %d is not valid.\n", THIS_MODULE->name, GPIO_LED);
        return -1;
    }
    pr_info("%s: GPIO pin %d is valid.\n", THIS_MODULE->name, GPIO_LED);
    // acquire access to gpio pin
    ret = gpio_request(GPIO_LED, "my-led");
    pr_info("%s: gpio_request() for GPIO pin %d returned %d.\n", THIS_MODULE->name, GPIO_LED, ret);
    // set pin direction as output
    ret = gpio_direction_output(GPIO_LED, 0);
    pr_info("%s: gpio_direction_output() for GPIO pin %d returned %d.\n", THIS_MODULE->name, GPIO_LED, ret);
    // blink led
    for(i=1; i<=10; i++) {
        gpio_set_value(GPIO_LED, 1);    // set pin high
        msleep(1000); // delay
        gpio_set_value(GPIO_LED, 0);    // set pin low
        msleep(1000); // delay
    }
    return 0;
}

static void __exit gpio_exit(void) {
    pr_info("%s: gpio_exit() called.\n", THIS_MODULE->name);
    gpio_free(GPIO_LED);
    pr_info("%s: gpio_free() released GPIO pin %d.\n", THIS_MODULE->name, GPIO_LED);
}

module_init(gpio_init);
module_exit(gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nilesh Ghule <nilesh@sunbeaminfo.com>");
MODULE_DESCRIPTION("GPIO Hw access via GPIO core lib for BBB");
