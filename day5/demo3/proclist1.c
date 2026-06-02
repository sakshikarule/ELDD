#include <linux/module.h>
#include <linux/list.h>
#include <linux/init_task.h>
#include <asm/current.h>
#include <linux/thread_info.h>

static __init int desd_init(void) {
    struct task_struct *ptr;
    printk(KERN_INFO "%s: desd_init() called.\n", THIS_MODULE->name);
    // https://elixir.bootlin.com/linux/v5.15.1/source/arch/ia64/include/asm/current.h#L16
    // older kernel on x86_32 -- to access current task_struct --> "current_thread_info()->task"
    // newer kernel on x86_ia64 -- to access current task_struct --> current
    printk(KERN_INFO "%s: current pid=%d, cmd=%s.\n", THIS_MODULE->name, current->pid, current->comm);
    list_for_each_entry(ptr, &current->tasks, tasks) {
        printk(KERN_INFO "%s: pid=%d, cmd=%s.\n", THIS_MODULE->name, ptr->pid, ptr->comm);
    }
    return 0;
}

static __exit void desd_exit(void) {
    printk(KERN_INFO "%s: desd_exit() called.\n", THIS_MODULE->name);
}

module_init(desd_init);
module_exit(desd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Devendra Dhande");
MODULE_DESCRIPTION("Traverse Kernel process list (Job queue) demo");
