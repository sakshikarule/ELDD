#include<linux/module.h>
#include<linux/moduleparam.h>

// create static global variables for module oaramters
static int baudRate = 9600;

//define your own callback function
int baudRate_change(const char *val, const struct kernel_param *kp)
{
	int ret;
	printk(KERN_INFO "%s : baud rate is changed to %s\n", THIS_MODULE->name, val);
	ret = param_set_int(val, kp);
	if(ret == 0)
		 printk(KERN_INFO "%s  : new baud rate calculations are done\n", THIS_MODULE->name);
	return ret;
}

// map above variables with module parameters

struct kernel_param_ops ops = {
	.set = baudRate_change,
	.get = param_get_int
};

module_param_cb(baudRate, &ops, &baudRate, 0644);

static int __init modparam_init(void)
{
	printk(KERN_INFO "%s : init() function is  started\n", THIS_MODULE->name);
	printk(KERN_INFO "%s : baudRate = %d\n", THIS_MODULE->name, baudRate);
	printk(KERN_INFO "%s : init() function is finished\n", THIS_MODULE->name);
	return 0;
}

static void __exit modparam_exit(void)
{
	printk(KERN_INFO "%s : exit() function is started\n", THIS_MODULE->name);
	printk(KERN_INFO "%s :bauRate = %d\n", THIS_MODULE->name, baudRate);
	printk(KERN_INFO "%s : exit() function is finished\n", THIS_MODULE->name);
}


module_init(modparam_init);
module_exit(modparam_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("esd @sunbeam");
MODULE_DESCRIPTION("This is a module to explore module parameter");

