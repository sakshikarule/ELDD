#include<linux/module.h>

static void __exit split_exit(void)
{
	printk(KERN_INFO "%s : split_exit() function is called\n", THIS_MODULE->name);
}

module_exit(split_exit);


