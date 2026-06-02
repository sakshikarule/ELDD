#include<linux/module.h>

static int __init split_init(void)
{
	printk(KERN_INFO "%s : split_init() function is calles\n", THIS_MODULE->name);
	return 0;
}

module_init(split_init);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("esd @SUNBEAM");
MODULE_DESCRIPTION("This is a split/muti file demo");

