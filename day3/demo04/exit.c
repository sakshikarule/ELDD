#include<linux/module.h>
#include "file.h"

static void __exit split_exit(void)
{
	printk(KERN_INFO "%s : split_exit() function is started\n", THIS_MODULE->name);
	my_function();
	printk(KERN_INFO "%s : split_exit() function is finished\n", THIS_MODULE->name);
}

module_exit(split_exit);


