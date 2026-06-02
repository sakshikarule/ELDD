#include<linux/module.h>
#include "file.h"

static int __init split_init(void)
{
	printk(KERN_INFO "%s : split_init() function is started\n", THIS_MODULE->name);
	my_function();
	my_var = 200;
	printk(KERN_INFO "%s : split_init() function is finished\n", THIS_MODULE->name);
	return 0;
}

module_init(split_init);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("esd @ sunbeam");
MODULE_DESCRIPTION("This is a split/multi file demo");
