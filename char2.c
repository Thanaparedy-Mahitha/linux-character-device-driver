
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/errno.h>

#define DEV_MEM_SIZE 20

static char device_buffer[DEV_MEM_SIZE];
static dev_t device_number;
static struct cdev pcd_cdev;
static struct class *class_pcd;
static struct device *device_pcd;
static size_t len;  // tracks how many valid bytes exist in buffer

#define pr_fmt(fmt) KBUILD_MODNAME ": %s: " fmt, __func__

/* READ */
static ssize_t pcd_read(struct file *filp, char __user *ubuf,
                        size_t count, loff_t *f_pos)
{
    size_t remaining, to_copy;
    unsigned long not_copied;

    pr_info("read requested: count=%zu, f_pos=%lld, len=%zu\n",
            count, *f_pos, len);

    /* Nothing left to read from current offset */
    if (*f_pos >= len)
        return 0;

    /* Remaining valid bytes from current position */
    remaining = len - *f_pos;

    /* Clamp to either requested, remaining, and device size bounds */
    to_copy = min(count, remaining);
    to_copy = min(to_copy, (size_t)(DEV_MEM_SIZE - *f_pos));

    if (to_copy == 0)
        return 0;

    not_copied = copy_to_user(ubuf, &device_buffer[*f_pos], to_copy);
    if (not_copied) {
        size_t copied = to_copy - not_copied;
        *f_pos += copied;          /* advance only by copied bytes */
        pr_err("copy_to_user failed: not_copied=%lu\n", not_copied);
        return -EFAULT;            /* classic convention on failure */
    }

    *f_pos += to_copy;

    pr_info("read done: copied=%zu, new f_pos=%lld\n", to_copy, *f_pos);
    return to_copy;
}

/* WRITE */
static ssize_t pcd_write(struct file *filp, const char __user *ubuf,
                         size_t count, loff_t *f_pos)
{
    size_t space, to_copy;
    unsigned long not_copied;

    pr_info("write requested: count=%zu, f_pos=%lld, len=%zu\n",
            count, *f_pos, len);

    if (!ubuf)
        return -EINVAL;

    /* No room if offset already beyond end of buffer */
    if (*f_pos >= DEV_MEM_SIZE)
        return -ENOSPC;

    space   = DEV_MEM_SIZE - *f_pos;
    to_copy = min(count, space);

    if (to_copy == 0)
        return -ENOSPC;

    not_copied = copy_from_user(&device_buffer[*f_pos], ubuf, to_copy);
    if (not_copied) {
        size_t copied = to_copy - not_copied;
        *f_pos += copied;
        len     = max(len, (size_t)*f_pos); /* update len if we extended */
        pr_err("copy_from_user failed: not_copied=%lu\n", not_copied);
        return -EFAULT;
    }

    *f_pos += to_copy;
    len     = max(len, (size_t)*f_pos);     /* track max written extent */

    pr_info("written=%zu bytes, new f_pos=%lld, len=%zu\n",
            to_copy, *f_pos, len);

    /* Optional: show up to 64 bytes for sanity */
    pr_info("data=\"%.*s\"\n", (int)min(len, (size_t)64), device_buffer);

    return to_copy;
}

/* OPEN / RELEASE */
static int pcd_open(struct inode *inode, struct file *filp)
{
    pr_info("open success\n");
    return 0;
}

static int pcd_release(struct inode *inode, struct file *filp)
{
    pr_info("release success\n");
    return 0;
}

static const struct file_operations pcd_fops = {
    .owner   = THIS_MODULE,
    .open    = pcd_open,
    .read    = pcd_read,
    .write   = pcd_write,
    .release = pcd_release,
};

/* INIT */
static int __init pcd_driver_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&device_number, 0, 1, "pcd_dev");
    if (ret) {
        pr_err("alloc_chrdev_region failed: %d\n", ret);
        return ret;
    }
    pr_info("device num <major>:<minor>=%d:%d\n",
            MAJOR(device_number), MINOR(device_number));

    cdev_init(&pcd_cdev, &pcd_fops);
    pcd_cdev.owner = THIS_MODULE;

    ret = cdev_add(&pcd_cdev, device_number, 1);
    if (ret) {
        pr_err("cdev_add failed: %d\n", ret);
        unregister_chrdev_region(device_number, 1);
        return ret;
    }

    class_pcd = class_create( "pcd_drv");
    if (IS_ERR(class_pcd)) {
        pr_err("class_create failed\n");
        cdev_del(&pcd_cdev);
        unregister_chrdev_region(device_number, 1);
        return PTR_ERR(class_pcd);
    }

    /* Use consistent device name: /dev/char_drv */
    device_pcd = device_create(class_pcd, NULL, device_number, NULL, "char_drv");
    if (IS_ERR(device_pcd)) {
        pr_err("device_create failed\n");
        class_destroy(class_pcd);
        cdev_del(&pcd_cdev);
        unregister_chrdev_region(device_number, 1);
        return PTR_ERR(device_pcd);
    }

    len = 0; /* reset buffer length */
    pr_info("module loaded\n");
    return 0;
}

/* EXIT */
static void __exit pcd_driver_cleanup(void)
{
    device_destroy(class_pcd, device_number);
    class_destroy(class_pcd);
    cdev_del(&pcd_cdev);
    unregister_chrdev_region(device_number, 1);
    pr_info("module unloaded\n");
}

module_init(pcd_driver_init);
module_exit(pcd_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("archana");
MODULE_DESCRIPTION("pseudo char driver");

