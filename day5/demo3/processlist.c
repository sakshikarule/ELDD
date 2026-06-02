#include<linux/module.h>
#include<linux/init_task.h>
#include<linux/list.h>

static int __init esd_init(void)
{
	struct task_struct *trav;

	printk(KERN_INFO "%s : esd_init() funcation is started\n", THIS_MODULE->name);

	// struct tsak_struct init_task --pcb of init process
    // struct list_head tasks --> process list

	list_for_each_entry(trav, &init_task.tasks, tasks)
		 printk(KERN_INFO "%s : pid = %d, name = %s\n", THIS_MODULE->name, trav->pid, trav->comm);

	printk(KERN_INFO "%s : esd_init() is completed\n", THIS_MODULE->name);
	return 0;
}

static void __exit esd_exit(void)
{
	printk(KERN_INFO "%s : esd_exit() function is called\n", THIS_MODULE->name);
}

module_init(esd_init);	// will add init_module wrapper on esd_init
module_exit(esd_exit);	// will add cleanup_module wrapper on esd_exit

MODULE_LICENSE("GPL");
MODULE_AUTHOR("esd @ sunbeam");
MODULE_DESCRIPTION("Module to get module information");
