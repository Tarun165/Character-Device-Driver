/*
 *  chardev.c: Creates a read-only char device that store into buffer using mutex and semaphore
 *  
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>	/* for put_user */
#include <linux/slab.h>	
#include <linux/semaphore.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/rtc.h>
#include <linux/sched.h>
#include <linux/fcntl.h>	


MODULE_LICENSE("GPL");
#define Desc "Char Device Driver"
#define Author "Vishvas Patel & Gourav Rattihali"
MODULE_DESCRIPTION(Desc);
MODULE_AUTHOR(Author);

/*  
 *  Prototypes - this would normally go in a .h file
 */

static int device_open(struct inode *, struct file *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static int device_release(struct inode *, struct file *);

#define SUCCESS 0
#define DEVICE_NAME "chardev_misc"	/* Dev name as it appears in /proc/devices   */

int init_module(void);
void cleanup_module(void);
/* 
 * Global variables are declared as static, so are global within the file. 
 */

static int Device_Open = 0;	/* Is device open?  
				 * Used to prevent multiple access to device */

static char **msg;
static struct miscdevice device;

static int readEnd =0;

static int residue=0;
static struct semaphore readerMutex;
static struct semaphore empty;
static struct semaphore full;
static struct semaphore writerMutex;
static int bsize=0;				//size of the buffer;
static int writeEnd=0;

static struct file_operations my_fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

module_param(bsize, int, 0000);

/*
 * This function is called when the module is loaded
 */
int init_module(void)
{
	int register_ret;
	device.name = DEVICE_NAME;
	device.fops = &my_fops;
	device.minor = MISC_DYNAMIC_MINOR;
	register_ret = misc_register(&device);
	if(register_ret){
		return register_ret;
	}
	printk(KERN_INFO "Device registered with buffer size = %d and minor no: %d\n", bsize,device.minor);
	residue = bsize;
	
	sema_init(&full,0); 		//counting semaphore for buffer full;
	sema_init(&empty,bsize);	//counting semaphore for buffer empty;
	sema_init(&readerMutex,1);	//mutex for readers;
	sema_init(&writerMutex,1);	//mutex for writers ;
	msg=kmalloc(bsize*sizeof(char*),GFP_KERNEL);
	if(!msg)
		{printk(KERN_INFO "Could not allocate memory! \n");}
	

	return 0;
}

/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void)
{
	/* 
	 * Unregister the device 
	 */
	kfree(msg);
	misc_deregister(&device);
	printk(KERN_INFO "Char Device is unregistered!\n");
	
}

/*
 * Methods
 */

/* 
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int device_open(struct inode *inode, struct file *file)
{
	Device_Open++;
	return 0;
}



/* 
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file *filp,	/* see include/linux/fs.h   */
			   char *buffer,	/* buffer to fill with data */
			   size_t length,	/* length of the buffer     */
			   loff_t * offset){

	int i=0;
	down_interruptible(&readerMutex);	//lock for multiple readers;
	down_interruptible(&full);		//lock for reading;
		readEnd=readEnd%bsize;
		while(msg[readEnd][i]!='\0'){
			if(!(residue < bsize)) break;
			copy_to_user(&buffer[i],&msg[readEnd][i],1);
			i++;
		}
		copy_to_user(&buffer[i],'\0',1);
		readEnd++;
		residue++;
		up(&empty);			//releasing lock for reading;
	up(&readerMutex);			//releasing lock for reader(s);
	return i;
}

/*  
 * Called when a process writes to dev file: echo "hi" > /dev/hello 
 */
static ssize_t
device_write(struct file *filp, const char *buff, size_t length, loff_t * off)
{	
	
	int i=0;
	down_interruptible(&writerMutex);	//lock for multiple writer;
	down_interruptible(&empty);		//lock for writing;
		writeEnd=writeEnd%bsize;
		char * string = kmalloc(sizeof(char)*length,GFP_KERNEL);
		while(i<length){
			if(!(residue > 0)) break;
			string[i]=buff[i];
			i++;
		}
		msg[writeEnd]=string;		//wring to buffer;
		writeEnd++;
		residue--;
		up(&full);			//releasing lock for writing;
	up(&writerMutex);			//releasing lock for writer(s);
	return i;
}

/* 
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
	Device_Open--;		/* We're now ready for our next caller */
	return 0;
}
