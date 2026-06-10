#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/slab.h>

/* --------------------------------------------------
 * Platform Data
 * --------------------------------------------------*/
struct mygpio_pdata
{
	int led_gpio;
	int sw_gpio;
};

/* --------------------------------------------------
 * Driver Private Data
 * --------------------------------------------------*/
struct mygpio_dev
{
	int led_gpio;
	int sw_gpio;
};

static int led_state;

/* --------------------------------------------------
 * Platform Device
 * --------------------------------------------------*/
static struct mygpio_pdata mygpio_pdata = {
	.led_gpio = 49, /* gpio1_17*/
	.sw_gpio = 115, /*gpio_19 */
};

static struct platform_device mygpio_device = {
	.name = "mygpio",
	.id = -1,
	.dev = {
		.platfrom_data = &mygpio_pdata,
	},
};
/* --------------------------------------------------
 * Probe
 * --------------------------------------------------*/
static int mygpio_probe(struct platform_device *pdata)
{
	struct mygpio_dev *d;
	struct mygpio_pdata *pdata;
	int ret;

    pr_info("%s: mygpio_probe() called\n", THIS_MODULE->name);

	pdata = dev_get_platdata(&dev->dev);
	if (!pdata) {
		pr_err("%s: no platform data found\n",THIS_MODULE->name);
		return -ETNVAL;
	}

	/*Allocate private data*/
	d = kmalloc(sizeof(*d), GFP_KERNEL);
	if (!d)
		return -ENOMEM;
	
	pr_info("%s: private struct allocated @ %p\n",THIS_MODULE->name, d);

	d->led_gpio = pdata->led_gpio;
	d->sw_gpio  = pdata->sw_gpio;

	pr_info("%s: LED GPIO = %d\n",THIS_MODULE->name, d->led_gpio);
    pr_info("%s: SWITCH GPIO = %d\n",THIS_MODULE->name, d->sw_gpio);

	/*validate GPIOs*/
	ret = gpio_is_vaild(d->led_gpio);
	pr_info("%s: gpio %d vaild=%d\n",THIS_MODULE->name, d->led_gpio,ret);

	ret = gpio_is_vaild(d->sw_gpio);
	pr_info("%s: gpio %d vaild=%d\n",THIS_MODULE->name, d->sw_gpio,ret);

	/*request GPIOs*/
	ret = gpio_request(d->led_gpio, "mygpio-led");
	if (ret) {
		pr_err("failed to request LED gpio\n");
		kfree(d);
		return ret;
	}

	ret = gpio_request(d->sw_gpio, "mygpio-switch");
	if (ret) {
		gpio_free(d->led_gpio);
		kfree(d);
		return ret;
	}

	/* Configure directions */
    gpio_direction_output(d->led_gpio, 1);
    gpio_direction_input(d->sw_gpio);

    platform_set_drvdata(pdev, d);

    pr_info("%s: driver loaded. LED=%d SWITCH=%d\n",
            THIS_MODULE->name,
            d->led_gpio,
            d->sw_gpio);

    // like pchar init code
    return 0;
}

static int mygpio_remove(struct platform_device *pdev)
{
	struct mygpio_dev *d;

	//like pchar exit code
	pr_info("%s: mygpio_remove() called\n",THIS_MODULE->name);

	d = platform_get_drvdata(pdev);
	gpio_set_value(d->led_gpio, 0);

	gpio_free(d->led_gpio);
	gpio_free(d->sw_gpio);

	kfree(d);

	return 0;
/* --------------------------------------------------
 * Platform Driver
 * --------------------------------------------------*/
static struct platform_driver mygpio_driver = {
    .probe  = mygpio_probe,
    .remove = mygpio_remove,
    .driver = {
        .name = "mygpio",
    },
};

/* --------------------------------------------------
 * Module Init
 * --------------------------------------------------*/
static int __init mygpio_init(void)
{
    int ret;

    pr_info("%s: module init\n",
            THIS_MODULE->name);

    ret = platform_device_register(&mygpio_device);
    if (ret) {
        pr_err("platform_device_register failed\n");
        return ret;
    }

    pr_info("%s: platform device registered\n",
            THIS_MODULE->name);

    ret = platform_driver_register(&mygpio_driver);
    if (ret) {
        platform_device_unregister(&mygpio_device);
        return ret;
    }

    pr_info("%s: platform driver registered\n",
            THIS_MODULE->name);

    return 0;
}

/* --------------------------------------------------
 * Module Exit
 * --------------------------------------------------*/
static void __exit mygpio_exit(void)
{
    pr_info("%s: module exit\n",
            THIS_MODULE->name);

    platform_driver_unregister(&mygpio_driver);

    platform_device_unregister(&mygpio_device);

    pr_info("%s: driver/device unregistered\n",
            THIS_MODULE->name);
}

module_init(mygpio_init);
module_exit(mygpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nilesh Ghule <nilesh@sunbeaminfo.com>");
MODULE_DESCRIPTION("Legacy Platform Device + Platform Driver Example");
