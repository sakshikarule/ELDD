#include<linux/module.h>

int my_var = 100;

void my_function(void)
{
	printk(KERN_INFO "%s : inside my_function() : my_var = %d\n", THIS_MODULE->name, my_var);
}
