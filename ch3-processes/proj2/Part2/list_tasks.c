#include <linux/sched.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
// #include <linux/sched/task.h>
#include <linux/sched/signal.h>
/* This function is called when the module is loaded. */
void dfs(struct task_struct *cur_task) 
{
    if (cur_task == NULL)
        return;
    struct list_head *list;
    struct task_struct *task;
    printk(KERN_INFO "Name: %s, State: %ld, Pid: %d\n", cur_task->comm, cur_task->state, cur_task->pid);
    list_for_each(list, &(cur_task->children)) {
        task = list_entry(list, struct task_struct, sibling);
        dfs(task);
    }
}
int list_tasks_init(void)
{
    printk(KERN_INFO "Loading Module \n");
    dfs(&init_task);
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

