/*                                                     
 * $Id: echo.c,v 1.5 2004/10/26 03:32:21 corbet Exp $ 
 */
//#include <linux/config.h> // Not present at the kernel anymore
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h>   /* kmalloc() */
#include <linux/fs.h>	 /* everything... */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <linux/aio.h>
#include <asm/uaccess.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include "echo.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Rafael Kraemer");

dev_t echoDevice;
struct echo_dev
{
	struct cdev cdev;
};

int echo_open(struct inode *inodep, struct file *filep)
{
	struct echo_dev *dev;
	dev = container_of(inodep->i_cdev, struct echo_dev, cdev);
	filep->private_data = dev;

	printk(KERN_INFO "Device has been opened\n");
	return 0;
}

int echo_release(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "Device has been sucessfully closed\n");
	return 0;
}

ssize_t echo_read(struct file *filep, char __user *buff, size_t count, loff_t *offp)
{

}

ssize_t echo_write(struct file *filep, const char __user *buff, size_t count, loff_t *offp)
{

}

struct file_operations device_fops = {
	.owner = THIS_MODULE,
	.llseek = no_llseek,
	.open = echo_open,
	.read = echo_read,
	.write = echo_write,
	.release = echo_release,
};

struct echo_dev *echo_device;

static int echo_init(void)
{
	int ret, Major, Minor, reg;

	// Allocate Major numbers amd regoster the device
	ret = alloc_chrdev_region(&echoDevice, 0, 1, "echo");
	if (ret < 0)
	{
		printk(KERN_ALERT "Major number alloctation failed\n");
		return ret;
	}

	Major = MAJOR(echoDevice);
	Minor = MINOR(echoDevice);

	printk(KERN_INFO "Aloccated Major number:%d\n", Major);

	// Register one device after the allocation of Minor and Major numbers
	echo_device = kzalloc(sizeof(struct echo_dev), GFP_KERNEL);
	if (!echo_device)
	{
		printk(KERN_ERR "Failed allocating memory\n");
		unregister_chrdev_region(echoDevice, 1);
		return -1;
	}
	cdev_init(&echo_device->cdev, &device_fops);
	echo_device->cdev.owner = THIS_MODULE;
	echo_device->cdev.ops = &device_fops;

	reg = cdev_add(&echo_device->cdev, echoDevice, 1);
	if (reg < 0)
	{
		printk(KERN_ERR "Error in cdev_add\n");
	}

	return 0;
}

static void echo_exit(void)
{
	int Major;
	// Unregister the device
	Major = MAJOR(echoDevice);
	cdev_del(&echo_device->cdev);
	kfree(echo_device);
	unregister_chrdev_region(echoDevice, 1);
	printk(KERN_INFO "Major number: %d unloaded\n", Major);
}

module_init(echo_init);
module_exit(echo_exit);