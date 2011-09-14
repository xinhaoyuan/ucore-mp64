#include <linux/module.h> //Needed by all modules
#include <linux/kernel.h> //Needed for KERN_ALERT

static int __init dosm_init(void)
{
	printk(KERN_ALERT "Hello world.\n");
	return 0;
}

static void __exit dosm_exit(void)
{
	printk(KERN_ALERT "Goodbye world.\n");
}

module_init(dosm_init);
module_exit(dosm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xinhaoyuan@gmail.com ");
MODULE_DESCRIPTION("Linux as a driver os for ucore-mp64");
