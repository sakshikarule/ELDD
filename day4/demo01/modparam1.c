#include<linux/module.h>
#include<linux/moduleparam.h>

// create static global variable for module parmeters
static int num = 3;
static char *str = "esd";

//map above variables with module parameter
module_param(num, int, 0644);
module_param_named(u_str, str, charp, 0644);


static int __init modparam_init(void)
{
	   printk(KERN_INFO "%s : init() funcation is stared\n", THIS_MODULE->name);
	   printk(KERN_INFO "%s : num = %d\n", THIS_MODULE->name, num);
	   printk(KERN_INFO "%s : str = %s\n", THIS_MODULE->name, str);
	   printk(KERN_INFO "%s : init() function is finished\n", THIS_MODULE->name);
	   return 0;

}

static void __exit modparam_exit(void)
{
	printk(KERN_INFO "%s : exit() funtion is started\n", THIS_MODULE->name);
	printk(KERN_INFO "%s : num = %d\n", THIS_MODULE->name, num);
	printk(KERN_INFO "%s : str = %s\n", THIS_MODULE->name, str);
	printk(KERN_INFO "%s : exit() function is finished\n", THIS_MODULE->name);
}

module_init(modparam_init);
module_exit(modparam_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("esd @ sunbeam");
MODULE_DESCRIPTION("This is a module to explore module parameters");

