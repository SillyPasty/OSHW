#include <linux/sched.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched/signal.h>
/* This function is called when the module is loaded. */
int list_tasks_init(void)
{
    printk(KERN_INFO "Name: %s, State: %ld, Pid: %d\n", init_task.comm, init_task.state, init_task.pid);
    printk(KERN_INFO "Loading Module \n");
    struct task_struct *task;
    for_each_process(task) {
        printk(KERN_INFO "Name: %s, State: %ld, Pid: %d\n", task->comm, task->state, task->pid);
    }
    return 0;
}
/* This function is called when the module is removed. */
void list_tasks_exit(void)
{
    printk(KERN_INFO "Removing Module \n");
}
/* Macros for registering module entry and exit points. */
module_init(list_tasks_init);
module_exit(list_tasks_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("list_tasks Module");
MODULE_AUTHOR("SGG");

