#include<linux/module.h>

// create few symbols in this module
int my_var = 100;
void my_function(void);

void my_function(void)
{
     printk(KERN_INFO "%s : inside my_function() : my_var = %d\n", THIS_MODULE->name, my_var);
}

// export symbol from this module
EXPORT_SYMBOL(my_var); //my_var will be exported from this module for all other modules
EXPORT_SYMBOL_GPL(my_function); // my_function will be exported from this module for modules which has GPL license

static int __init export_init(void)
{
	printk(KERN_INFO "%s : export_init() is started\n", THIS_MODULE->name);
	my_function();
	my_var = 200;
	printk(KERN_INFO "%s : export_init() is finished\n", THIS_MODULE->name);
	return 0;
}

static void __exit export_exit(void)
{
	printk(KERN_INFO "%s : export_exit() is started\n", THIS_MODULE->name);
	my_function();
	printk(KERN_INFO "%s : export_exit() is finished\n", THIS_MODULE->name);
}

module_init(export_init);
module_exit(export_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ESD @sunbeam");
MODULE_DESCRIPTION("This is module to export symbils");


