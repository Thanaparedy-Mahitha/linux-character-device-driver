#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/uaccess.h>
 
#define DEV_MEM_SIZE 20
char device_buffer[DEV_MEM_SIZE];
dev_t device_number;
struct cdev pcd_cdev;
struct class *class_pcd;
struct device *device_pcd;
static int len;
ssize_t pcd_read(struct file *filp,char __user *buff,size_t count,loff_t *f_pos)
{
    pr_info("Requested for %zu bytes\n",count);
    
    if(*f_pos>=len)
	    return 0;
    int n;

    n= len-*f_pos; //HOW MUCH IS LEFT 
		   
   if(count < n)
	    n=count;
    else
	    n=len;


    if(n>DEV_MEM_SIZE)
	    count=DEV_MEM_SIZE-*f_pos;
    /*
    if((*f_pos+count)>DEV_MEM_SIZE)
        count=DEV_MEM_SIZE-*f_pos;  */
    
    
    copy_to_user(buff,&device_buffer[*f_pos],count);
 
    *f_pos+=count;
    pr_info("data is =%s\n",device_buffer);
    return 0;
}
ssize_t pcd_write(struct file *filp,const char __user *u_buff,size_t count,loff_t *f_pos)
{

    pr_err(" iam in write..\n");	
  	if(count==0)
		return 0;

//chck is it more than 20 bytes
    if(count > DEV_MEM_SIZE)
	return 0;
//1st time 15 bytes remain 5 now again 10 byres ===== fp is 15 + count = 25 > kbuf
if((*f_pos+count)>DEV_MEM_SIZE)
	count=DEV_MEM_SIZE -*f_pos; // 20-15 --only write 5 bytes

    copy_from_user(&device_buffer[*f_pos],u_buff,count);
    len+=count;
    *f_pos+=count;
    pr_info("no of bytes successfully written=%zu\n",count);
    pr_err("data  from user =%s\n",device_buffer);
    return count;
 
}
 
int pcd_open(struct inode*inode,struct file *filp)
{
    pr_info("open success\n");
    return 0;
}
int pcd_release(struct inode *inode,struct file *filp)
{
    pr_info("release success\n");
    return 0;
 
}
struct file_operations pcd_fops =
{
    .open=pcd_open,
    .write=pcd_write,
    .read=pcd_read,
    .release=pcd_release,
    .owner=THIS_MODULE
};
static int __init pcd_driver_init(void)
{
    int ret;
//dynamically allocate device num
ret=alloc_chrdev_region(&device_number,0,1,"pcd_dev");
pr_info("device num <major>:<minor>=%d:%d\n",MAJOR(device_number),MINOR(device_number));
//intialize the cdev structure with fops
cdev_init(&pcd_cdev,& pcd_fops);
 
//register a device(cdev structure) with VFS
//pcd_cdev.owner=THIS_MODULE;
ret=cdev_add(&pcd_cdev,device_number,1);
 
 
//to create the device file
class_pcd=class_create("pcd_drv");
 
device_pcd=device_create(class_pcd,NULL,device_number,NULL,"chr_drv");
pr_info("module loaded\n");
return 0;
 
}
static void __exit pcd_driver_cleanup(void)
{
    device_destroy(class_pcd,device_number);
    class_destroy(class_pcd);
    cdev_del(&pcd_cdev);
    unregister_chrdev_region(device_number,1);
    pr_info("module unloaded\n");
}
module_init(pcd_driver_init);
module_exit(pcd_driver_cleanup);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("archana");
MODULE_DESCRIPTION("pseudo char driver");
