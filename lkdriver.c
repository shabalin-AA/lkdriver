#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define BUF_SIZE 256

char buf[BUF_SIZE] = "hello from kernel\n";
size_t buf_len = 18;

static ssize_t myfs_read(struct file *f, char __user *ubuf, size_t count, loff_t *ppos) {
    return simple_read_from_buffer(ubuf, count, ppos, buf, buf_len);
}

static ssize_t myfs_write(struct file *f, const char __user *ubuf, size_t count, loff_t *ppos) {
    if (count > BUF_SIZE)
        count = BUF_SIZE;
    if (copy_from_user(buf, ubuf, count))
        return -EFAULT;
    buf_len = count;
    return count;
}

const struct file_operations myfs_fops = {
    .owner = THIS_MODULE,
    .read  = myfs_read,
    .write = myfs_write,
};

struct miscdevice myfs_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = "myfs",
    .fops  = &myfs_fops,
    .mode  = 0666,
};

static int __init myfs_init(void) {
    return misc_register(&myfs_dev);
}

static void __exit myfs_exit(void) {
    misc_deregister(&myfs_dev);
}

module_init(myfs_init);
module_exit(myfs_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Minimal char driver");
