

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>      // for __user
#include <linux/fs.h>

#define PROC_NAME    "mahi_proc"
#define BUFFER_SIZE 128
static char proc_buff[BUFFER_SIZE];
static int buff_len;
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mahitha");
MODULE_DESCRIPTION("/proc/hello - minimal proc entry");

static ssize_t my_read(struct file *file, char __user *ubuf,
                          size_t count, loff_t *ppos)
{
	if(* ppos>0  )
		return 0;


	printk(" currently no erros in read\n");
	 if(copy_to_user(ubuf,proc_buff,buff_len))
		 return -EFAULT;
	 *ppos=buff_len;
printk(" sending data to userspace :%s\n",proc_buff);	 
	 return buff_len;

}
static ssize_t my_write(struct file *file,const char __user * ubuf ,size_t count ,loff_t *ppos){


	if(count ==0)
		return 0;

	 if(count > BUFFER_SIZE )
		 return -EINVAL;
	 if(copy_from_user(proc_buff,ubuf,count))
		 return -EFAULT;

	 buff_len=count;
	 proc_buff[count]='\0';
printk("user wrote into kernel :%s\n",proc_buff);
	 return count;

}
static  const struct proc_ops  file_p= {
 .proc_read = my_read,
    .proc_write=my_write,
};

static int __init hello_init(void)
{
    struct proc_dir_entry *entry;

    entry = proc_create(PROC_NAME, 0666, NULL, &file_p);
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





