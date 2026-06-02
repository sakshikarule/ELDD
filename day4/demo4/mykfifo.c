#include<linux/module.h>
#include<linux/kfifo.h>

struct kfifo myfifo;

static int __init mykfifo_init(void)
{
	char str1[] = "abcdefghijklmnopqrstuvwxyz";
	char str2[] = "0123456789";
	int ret;
	
	printk(KERN_INFO "%s : init() is started\n", THIS_MODULE->name);
	
	ret = kfifo_alloc(&myfifo, 32, GFP_KERNEL);
	// err check
	
	printk(KERN_INFO "%s : size = %d\n", THIS_MODULE->name, kfifo_size(&myfifo));
	printk(KERN_INFO "%s : len = %d\n", THIS_MODULE->name, kfifo_len(&myfifo));
	printk(KERN_INFO "%s : avail = %d\n", THIS_MODULE->name, kfifo_avail(&myfifo));

	kfifo_in(&myfifo, str1, 26);

	printk(KERN_INFO "%s : size = %d\n", THIS_MODULE->name, kfifo_size(&myfifo));
	printk(KERN_INFO "%s : len = %d\n", THIS_MODULE->name, kfifo_len(&myfifo));
	printk(KERN_INFO "%s : avail = %d\n", THIS_MODULE->name, kfifo_avail(&myfifo));

	kfifo_in(&myfifo, str2, 10);

	printk(KERN_INFO "%s : size = %d\n", THIS_MODULE->name, kfifo_size(&myfifo));
	printk(KERN_INFO "%s : len = %d\n", THIS_MODULE->name, kfifo_len(&myfifo));
	printk(KERN_INFO "%s : avail = %d\n", THIS_MODULE->name, kfifo_avail(&myfifo));
	
	printk(KERN_INFO "%s : is empty = %d\n", THIS_MODULE->name, kfifo_is_empty(&myfifo));
	printk(KERN_INFO "%s : is full = %d\n", THIS_MODULE->name, kfifo_is_full(&myfifo));

	printk(KERN_INFO "%s : init() is completed\n", THIS_MODULE->name);
	return 0;
}

static void __exit mykfifo_exit(void)
{
	char str1[26];
	char str2[10];
	
	printk(KERN_INFO "%s : exit() is started\n", THIS_MODULE->name);
	
	printk(KERN_INFO "%s : size = %d\n", THIS_MODULE->name, kfifo_size(&myfifo));
	printk(KERN_INFO "%s : len = %d\n", THIS_MODULE->name, kfifo_len(&myfifo));
	printk(KERN_INFO "%s : avail = %d\n", THIS_MODULE->name, kfifo_avail(&myfifo));
	
	kfifo_out(&myfifo, str1, sizeof(str1));
	printk(KERN_INFO "%s : str1 = %s\n", THIS_MODULE->name, str1);
	
	printk(KERN_INFO "%s : size = %d\n", THIS_MODULE->name, kfifo_size(&myfifo));
	printk(KERN_INFO "%s : len = %d\n", THIS_MODULE->name, kfifo_len(&myfifo));
	printk(KERN_INFO "%s : avail = %d\n", THIS_MODULE->name, kfifo_avail(&myfifo));
	
	kfifo_out(&myfifo, str2, sizeof(str2));
	printk(KERN_INFO "%s : str2 = %s\n", THIS_MODULE->name, str2);

	printk(KERN_INFO "%s : size = %d\n", THIS_MODULE->name, kfifo_size(&myfifo));
	printk(KERN_INFO "%s : len = %d\n", THIS_MODULE->name, kfifo_len(&myfifo));
	printk(KERN_INFO "%s : avail = %d\n", THIS_MODULE->name, kfifo_avail(&myfifo));

	printk(KERN_INFO "%s : is empty = %d\n", THIS_MODULE->name, kfifo_is_empty(&myfifo));
	printk(KERN_INFO "%s : is full = %d\n", THIS_MODULE->name, kfifo_is_full(&myfifo));
	
	kfifo_free(&myfifo);

	printk(KERN_INFO "%s : exit() is completed\n", THIS_MODULE->name);
}

module_init(mykfifo_init);
module_exit(mykfifo_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("esd @ sunbeam");
MODULE_DESCRIPTION("This is module for kfifo");






