#include<linux/module.h>


static int __init esd_init(void)
{
	printk(KERN_INFO "%s : esd_init() function is started\n", THIS_MODULE->name);
	printk(KERN_INFO "%s : state = %d\n", THIS_MODULE->name, THIS_MODULE->state);
	return 0;
}

static void __exit esd_exit(void)
{
	printk(KERN_INFO "%s : esd_exit() function is started\n", THIS_MODULE->name);
	printk(KERN_INFO "%s : state = %d\n", THIS_MODULE->name, THIS_MODULE->state);
}

module_init(esd_init);  // will add init_module wrapper on esd_init
module_exit(esd_exit);  // will add cleanup_module wrapper on esd_exit

MODULE_LICENSE("GPL");
MODULE_AUTHOR("esd @sunbeam");
MODULE_DESCRIPTION("Module to get module information");

