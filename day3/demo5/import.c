#include<linux/module.h>
#include "import.h"

static int __init import_init(void)
{
	printk(KERN_INFO "%s : import_init() is started\n", THIS_MODULE->name);
	my_function();
	printk(KERN_INFO "%s : import_init() is finished\n", THIS_MODULE->name);
	return 0;
}

static void __exit import_exit(void)
{
	printk(KERN_INFO "%s : import_exit() is started\n", THIS_MODULE->name);
	my_function();
	my_var = 300;
	printk(KERN_INFO "%s : import_exit() is finished\n", THIS_MODULE->name);
}

module_init(import_init);
module_exit(import_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("esd @ sunbeam");
MODULE_DESCRIPTION("This is module to import symbols");











