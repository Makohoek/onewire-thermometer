/*
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 * Alexandre Montilla
 * <alexandre.montilla@gmail.com>
 */
/* Includes */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/sched.h>

/* minor aliases */
static const unsigned char NB_OF_MINORS = 1;

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

/* Module parameters */
static unsigned int GpioPort = 0;

/* Standard character device operations */
static ssize_t read(struct file *f, char *buf, size_t size, loff_t *offset);
static int open(struct inode *in, struct file *f);
static int release(struct inode *in, struct file *f);

struct priv_t 
{
  unsigned int minor;
};

/* File operations for our character device */
static struct file_operations fileOperations = 
{
  .read = read,
  .write = NULL,
  .open = open,
  .release = release
};

/* dev_t contains major and minor version */
dev_t dev;

/* struct cdev == a Character Device*/
struct cdev *myDevice;

static ssize_t read(struct file *f, char *buf, size_t size, loff_t *offset)
{
  printk(KERN_INFO "Read called!\n");

  //   sizeToRead = MIN(myLinkedList->bufferSize - myLinkedList->beginReadIndex, size); // this condition is not valid for a non destructive read
  //
  //      printk(KERN_INFO "BeginReadIndex: %ld from file\n", myLinkedList->beginReadIndex);
  //      printk(KERN_INFO "Have to read %d from file\n", sizeToRead);
  //
  //   myBuffer = (char*)kmalloc(sizeof(char)*sizeToRead, GFP_KERNEL);
  //
  //   myLinkedList->readFromLinkedList(myLinkedList, myBuffer, sizeToRead);
  //
  //   sizeNotCopiedToUser = copy_to_user(buf, myBuffer, sizeToRead);
  //   printk(KERN_INFO "Could not copy %d bytes to user\n", sizeNotCopiedToUser);
  //
  //   kfree(myBuffer);
  //
  //   return (sizeToRead-sizeNotCopiedToUser);
  return 0;
}


static int open(struct inode *in, struct file *f)
{
  int errorCode = 0;

  printk(KERN_INFO "Pid(%d) Open with (major,minor) = (%d,%d)\n", current->tgid, MAJOR(in->i_rdev), MINOR(in->i_rdev));

  return errorCode;
}

static int release(struct inode *in, struct file *f)
{
  int errorCode = 0;

  printk(KERN_INFO "Pid(%d) Release with (major,minor) = (%d,%d)\n", current->tgid, MAJOR(in->i_rdev), MINOR(in->i_rdev));

  return errorCode;
}


static int init(void)
{
  int errorCode = 0;

  /* dynamic allocation for major/minors */
  if (alloc_chrdev_region(&dev, 0, NB_OF_MINORS, "sample") == -1)
  {
    printk(KERN_ALERT ">>> ERROR alloc_chrdev_region\n");
    return -EINVAL;
  }
  /* display majors/minor */
  printk(KERN_INFO "Init allocated (major, minor)=(%d,%d)\n",MAJOR(dev),MINOR(dev));

  /* displays GPIO port */
  printk(KERN_INFO "GpioPort=%d\n", GpioPort);

  /* allocating memory for our character device and linking fileOperations */
  myDevice = cdev_alloc();
  myDevice->ops = &fileOperations;
  myDevice->owner = THIS_MODULE;

  /* link between chrdev region and fileOperations */
  errorCode = cdev_add(myDevice,dev,NB_OF_MINORS);
  if (errorCode == -1)
  {
    printk(KERN_ALERT ">>> ERROR cdev_add\n");
    return -EINVAL;
  }

  return(errorCode);
}


static void cleanup(void)
{
  int errorCode = 0;

  /* freeing memory and major,(s) */
  unregister_chrdev_region(dev,NB_OF_MINORS);
  cdev_del(myDevice);
}

module_exit(cleanup);
module_init(init);

module_param(GpioPort, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(GpioPort, "Used GPIO port for the thermometer");

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mattijs Korpershoek / Alexandre Montilla");
MODULE_DESCRIPTION("1-Wire Digital thermometer DS18B2 driver");
MODULE_SUPPORTED_DEVICE("DS18B2");
