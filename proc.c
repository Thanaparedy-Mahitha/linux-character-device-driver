
// hello.c - /proc/hello example (modern API)
// Prints "Hello World\n" when you `cat /proc/hello`

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>      // for __user
#include <linux/fs.h>

#define PROC_NAME    "hello"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mahitha");
MODULE_DESCRIPTION("/proc/hello - minimal proc entry");

static ssize_t hello_read(struct file *file, char __user *ubuf,
                          size_t count, loff_t *ppos)
{
    static const char msg[] = "Hello World\n";
    // simple_read_from_buffer handles ppos correctly, making read() behave well
    return simple_read_from_buffer(ubuf, count, ppos, msg, sizeof(msg) - 1);
}

static const struct proc_ops hello_proc_ops = {
    .proc_read = hello_read,
};

static int __init hello_init(void)
{
    struct proc_dir_entry *entry;

    entry = proc_create(PROC_NAME, 0444, NULL, &hello_proc_ops);
    if (!entry) {
        pr_err("Failed to create /proc/%s\n", PROC_NAME);
        return -ENOMEM;
    }
    pr_info("/proc/%s created\n", PROC_NAME);
    return 0;
}

static void __exit hello_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    pr_info("/proc/%s removed\n", PROC_NAME);
}

module_init(hello_init);
module_exit(hello_exit);

