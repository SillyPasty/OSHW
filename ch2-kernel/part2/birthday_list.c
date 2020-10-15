#include <linux/init.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>

struct birthday
{
    int day;
    int month;
    int year;
    struct list_head list;
};

static LIST_HEAD(birthday_list);

struct birthday *new_bir(int day, int month, int year)
{
    struct birthday *person;
    person = kmalloc(sizeof(*person), GFP_KERNEL);
    person->day = day;
    person->month = month;
    person->year = year;
    return person;
}

int birthday_list_init(void)
{
    printk(KERN_INFO "Loading Module \n");
    struct birthday *person;
    int i;
    for (i = 1; i < 6; i++)
    {
        person = new_bir(i, 8, 1992);
        INIT_LIST_HEAD(&person->list); // init the list member
        list_add_tail(&person->list, &birthday_list);
    }
    struct birthday *ptr;
    list_for_each_entry(ptr, &birthday_list, list)
    {
        printk(KERN_INFO "Year: %d, Month: %d, Day: %d\n", ptr->year, ptr->month, ptr->day);
    }

    return 0;
}

/* This function is called when the module is removed. */
void birthday_list_exit(void)
{
    printk(KERN_INFO "Removing Module \n");
    struct birthday *ptr, *next;
    list_for_each_entry_safe(ptr, next, &birthday_list, list)
    {
        printk(KERN_INFO "Removing: Year: %d, Month: %d, Day: %d\n", ptr->year, ptr->month, ptr->day);
        list_del(&ptr->list);
        kfree(ptr);
    }
}
/* Macros for registering module entry and exit points. */
module_init(birthday_list_init);
module_exit(birthday_list_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Birthday List Module");
MODULE_AUTHOR("WYB");