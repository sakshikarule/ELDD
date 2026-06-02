#include<linux/module.h>
#include<linux/moduleparam.h>

// create static global variables for module parameters
static int arr[5];
unsigned int length = 5;

// map above variables with module parameters
module_param_array(arr, int, &length, 0644);

static int __init modparam_init(void)
{
	int i;
	printk(KERN_INFO "%s : init() function is started\n", THIS_MODULE->name);
	printk(KERN_INFO "%s : arr = ", THIS_MODULE->name);
	for(i = 0 ; i < length ; i++)
		printk(KERN_INFO "%s : arr[%d] = %d\n", THIS_MODULE->name, i, arr[i]);
	printk(KERN_INFO "%s : init() function is finished\n", THIS_MODULE->name);
	return 0;
}

static void __exit modparam_exit(void)
{
	int i;
	printk(KERN_INFO "%s : exit() function is started\n", THIS_MODULE->name);
	printk(KERN_INFO "%s : arr = ", THIS_MODULE->name);
	for(i = 0 ; i < length ; i++)
		printk(KERN_INFO "%s : arr[%d] = %d\n", THIS_MODULE->name, i, arr[i]);
	printk(KERN_INFO "%s : exit() function is finished\n", THIS_MODULE->name);
}

module_init(modparam_init);
module_exit(modparam_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("esd @ sunbeam");
MODULE_DESCRIPTION("This is a module to explore module parameters");















