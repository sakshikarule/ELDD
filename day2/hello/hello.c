#include<linux/module.h>


int init_module(void)
{
	printk(KERN_INFO "hello : init_module is called\n");
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "hello : cleanup_module is called\n");
}

MODULE_INFO(license, "GPL");
MODULE_INFO(author, "ESD @ SUNBEAM");
MODULE_INFO(description, "This is a hello module");

