#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>

// our device is a custom gpio device
// two gpio pins -- 1-led, 2-switch
// private struct for our device
struct mygpio_dev
{
    int led_gpio; // led gpio pin (set to gpio1.17 i.e. 49 in device tree)
    int sw_gpio;  // switch gpio pin (set to gpio3.19 i.e. 115 in device tree)
};

static int led_state = 0;

static int mygpio_probe(struct platform_device *pdev)
{
    struct device_node *np = pdev->dev.of_node;
    struct mygpio_dev *d;
    int ret;

    pr_info("%s: mygpio_probe() is called.\n", THIS_MODULE->name);

    /* --- Alloc Device Private Struct --- */
    d = (struct mygpio_dev *)kmalloc(sizeof(struct mygpio_dev), GFP_KERNEL);
    pr_info("%s: allocated dev private struct ptr=%p\n", THIS_MODULE->name, d);

    /* --- Get GPIOs from DT --- */
    d->led_gpio = of_get_named_gpio(np, "led-gpio", 0); // 49 = gpio1.17 (1*32 + 17)
    ret = gpio_is_valid(d->led_gpio);
    pr_info("%s: led-gpio gpio = %d -- valid ret=%d\n", THIS_MODULE->name, d->led_gpio, ret);

    d->sw_gpio = of_get_named_gpio(np, "switch-gpio", 0); // 115 = gpio3.19 (3*32 + 19)
    ret = gpio_is_valid(d->sw_gpio);
    pr_info("%s: switch-gpio gpio = %d -- valid ret=%d\n", THIS_MODULE->name, d->sw_gpio, ret);

    /* --- Request GPIOs --- */
    ret = gpio_request(d->led_gpio, "mygpio-led");
    pr_info("%s: led-gpio gpio %d requested (ret=%d)\n", THIS_MODULE->name, d->led_gpio, ret);

    ret = gpio_request(d->sw_gpio, "mygpio-switch");
    pr_info("%s: switch-gpio gpio %d requested (ret=%d)\n", THIS_MODULE->name, d->sw_gpio, ret);

    /* --- Configure Directions --- */
    gpio_direction_output(d->led_gpio, 1); // LED ON
    gpio_direction_input(d->sw_gpio);

    platform_set_drvdata(pdev, d);
    pr_info("mygpio legacy driver loaded. LED=%d Switch=%d\n", d->led_gpio, d->sw_gpio);

    // assignment ---
    //  allocate device number
    //  create device class
    //  create device file
    //  cdev init (with file operations)
    //  cdev add

    return 0;
}

static int mygpio_remove(struct platform_device *pdev)
{
    struct mygpio_dev *d = platform_get_drvdata(pdev);

    // assignment ---
    //  cdev delete
    //  destroy device file
    //  destroy device class
    //  release device number

    pr_info("%s: mygpio_remove() is called.\n", THIS_MODULE->name);
    gpio_free(d->led_gpio);
    gpio_free(d->sw_gpio);
    kfree(d);

    return 0;
}

static const struct of_device_id mygpio_of_ids[] = {
    {.compatible = "mycompany,led-switch"},
    {},
};
MODULE_DEVICE_TABLE(of, mygpio_of_ids);

static struct platform_driver mygpio_driver = {
    .probe = mygpio_probe,
    .remove = mygpio_remove,
    .driver = {
        .name = "mygpio-legacy",
        .of_match_table = mygpio_of_ids,
    },
};

// module_platform_driver(mygpio_driver);

static int __init mygpio_driver_init(void)
{
    int ret;
    pr_info("%s: mygpio mygpio_driver_init() is called.\n", THIS_MODULE->name);
    ret = platform_driver_register(&mygpio_driver);
    pr_info("%s: mygpio platform_driver_register() is called (ret=%d).\n", THIS_MODULE->name, ret);
    return 0;
}

static void __exit mygpio_driver_exit(void)
{
    pr_info("%s: mygpio mygpio_driver_exit() is called.\n", THIS_MODULE->name);
    platform_driver_unregister(&mygpio_driver);
    pr_info("%s: mygpio platform_driver_unregister() is called.\n", THIS_MODULE->name);
}

module_init(mygpio_driver_init);
module_exit(mygpio_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nilesh Ghule <nilesh@sunbeaminfo.com>");
MODULE_DESCRIPTION("Platform device for switch-led");
