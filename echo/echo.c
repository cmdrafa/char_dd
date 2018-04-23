/*                                                     
 * $Id: echo.c,v 1.5 2004/10/26 03:32:21 corbet Exp $ 
 */
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/types.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");
dev_t echoDevice;

static int echo_init(void)
{
	int ret, Major;

	ret = alloc_chrdev_region(&echoDevice, 0, 1, "echo");
	if (ret < 0)
	{
		printk(KERN_ALERT "Major number alloctation failed\n");
		return ret;
	}

	Major = MAJOR(echoDevice);
	printk(KERN_INFO "Aloccated Major number:%d\n", Major);
	return 0;
}

static void echo_exit(void)
{
	int Major;
	Major = MAJOR(echoDevice);
	unregister_chrdev_region(echoDevice, 1);
	printk(KERN_INFO "Major number: %d unloaded\n", Major);

}

module_init(echo_init);
module_exit(echo_exit);
