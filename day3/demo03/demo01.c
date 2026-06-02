#include<linux/module.h>

//__init ==> gcc attribute
//__attribute__((section(.init.text)))

// one seperate section .init.text is created in kernel module
// initialization code is kept into that section
// this section	is immediately removed from  memory once initialization is done

int __init init_module(void)
{
	printk(KERN_INFO "demo01 : init_module() is called\n");
	return 0;
}
// __exit ==> gcc attribute
//__attribute_((section(.exit.text)))

//one seperate section .exit.text is created in kernel module
// de initialization is done and after this all other section will also be removed

void __exit cleanup_module(void)
{
	printk(KERN_INFO "demo01 : cleanup_module() is called\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ESD @ SUNBEAM");
MODULE_DESCRIPTION("This is a demo01 module");

// MODULE_XYZ() -- will use MODULE_INFO() internally

// static will restrict scope of kernel module functions to the file only
// so that function should not be called in other module

