#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static int my_thread_func(void *param) {
	int i;
	for(i=1; i<=10; i++) {
		pr_info("%s:my_thread_func() = %d.\n", THIS_MODULE->name, i);
		msleep(1000);
	}
	return 0;
}

static int __init my_init(void) {
	pr_info("%s: module_init() called.\n", THIS_MODULE->name);
	kthread_run(my_thread_func, NULL, "mykernthemod");
	return 0;
}

static void __exit my_exit(void) {
	pr_info("%s: module_exit() called.\n", THIS_MODULE->name);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sakshikarule");
MODULE_DESCRIPTION("kernel Thread Demo.");

