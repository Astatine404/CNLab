#include<linux/module.h>
#include<linux/string.h>
#include<linux/kernel.h>
#include<linux/time.h>
#include<linux/fs.h>
#include<asm/uaccess.h>

static int Tick_init(void);
static void Tick_exit(void);
static int opendev(struct inode *, struct file *);
static int closedev(struct inode *, struct file *);
static ssize_t readme(struct file *, char*, size_t, loff_t *);
static ssize_t writeme(struct file *, const char*, size_t, loff_t *)
static int Major;

#define SUCCESS 0
#define DEVICE_NAME "TickTock"

static struct file_operations fops = {
	.read = readme,
	.write = writeme,
	.open = opendev,
	.release = closedev
};



static int Tick_init(void){
	Major = register_chrdev(0, DEVICE_NAME, &fops);
	if(Major>0){
		printk("TickTock Ready!: %d\n", Major);
		return 0;
	
	}
	else{
		printk("Error! Init failed.: %d\n", Major);
	}
}

static void Tock_exit(void){
	unregister_chrdev(Major, DEVICE_NAME);
	printk("TickTock Exit!");
}

module_exit(Tock_exit);
