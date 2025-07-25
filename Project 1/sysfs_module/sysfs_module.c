#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/sched.h>

static struct kobject *example_kobject;
volatile int roots = 0;

static ssize_t foo_show(struct kobject * obj, struct kobj_attribute *attr, char *buf)
{
	int id = 1;
	struct task_struct *curr = get_current();
	int pid = curr->pid;

	printk("find_roots sysfs opened by process %d\n", curr->pid);

	do{
			id = curr->pid;
			printk("id: %d, name: %s \n", id, curr->comm);
			curr = curr->parent;
	}while(id != 1);

	return(sprintf(buf, "%d\n", pid));
}
struct kobj_attribute foo_attribute = __ATTR(find_roots, 0660, foo_show, NULL);
static int __init mymodule_init(void)
{
	int error = 0;

	example_kobject = kobject_create_and_add ("team00", kernel_kobj);

	if(!example_kobject)
		return  -ENOMEM;

	error = sysfs_create_file(example_kobject, &foo_attribute.attr);
	if(error) {
		printk("failed to create the foo file in /sys/kernel/kobject_example\n");
	}

	return error;
}

static void __exit mymodule_exit(void)
{
	printk("Module uninitialized successfully\n");
	kobject_put(example_kobject);
}

module_init(mymodule_init);
module_exit(mymodule_exit);
MODULE_AUTHOR("team00");
MODULE_LICENSE("GPL");
