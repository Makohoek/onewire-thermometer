/**
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 * Alexandre Montilla
 * <alexandre.montilla@gmail.com>
 */
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/gfp.h>

#include "DiscoveryProtocol.h"
#include "GlobalData.h"
#include "OneWire.h"
#include "SensorID.h"
#include "bitOperations.h"
#include "Klogger.h"
#include "led.h"

#include "LinkedList.h"
#include "SensorOperations.h"

/* minor aliases */
static unsigned char mNbOfMinors = 0;
static const unsigned char LED = 0;
static const unsigned char SENSOR = 1;

/* IOCTL commands */
#define IOCTL_THERM_MAGIC 'k'
#define THERM_IOCTL_RESOLUTION _IO(IOCTL_THERM_MAGIC, 0)
#define THERM_IOCTL_MAXNR 0

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

/* Module parameters */
static int mGpioPin = 2;

/* Standard character device operations */
static ssize_t read(struct file *f, char *buf, size_t size, loff_t *offset);
static long ioctlTermDriver(struct file *f, unsigned int cmd, unsigned long arg);
static int open(struct inode *in, struct file *f);
static int release(struct inode *in, struct file *f);

/* File operations for our character device */
static struct file_operations fileOperations =
{
  .read = read,
  .write = NULL,
  .open = open,
  .release = release,
  .unlocked_ioctl = ioctlTermDriver
};

struct priv_t 
{
  unsigned int minor;
};

/* dev_t contains major and minor version */
dev_t dev;

/* device class */
static struct class* thermClass = NULL;

/* struct cdev == a Character Device*/
struct cdev *myDevice;

LinkedList* mSensorsList = NULL;
Sensor* mCurrentSensor = NULL; // the current open one

static ssize_t read(struct file *f, char *buf, size_t size, loff_t *offset)
{
  int temperature = 0;
  int sizeNotCopiedToUser = 0;
  int sizeCopiedToUser = 0;
  int nbCharsTemperatureString = 0;
  TemperatureString temperatureString;
  logk((KERN_INFO "Read called!\n"));
  temperature = sensorRequestTemperature(*mCurrentSensor);
  nbCharsTemperatureString = temperatureToString(temperatureString, temperature);
  sizeNotCopiedToUser = copy_to_user(buf, temperatureString, nbCharsTemperatureString);
  sizeCopiedToUser = nbCharsTemperatureString - sizeNotCopiedToUser;
  return sizeCopiedToUser;
}

// change open based on the sensors
static int open(struct inode *in, struct file *f)
{
  int errorCode = 0;
  int sensorIndex = MINOR(in->i_rdev);
  mCurrentSensor = mSensorsList->getItemFromIndex(mSensorsList, sensorIndex);
  logk((KERN_INFO "Pid(%d) Open with (major,minor) = (%d,%d)\n", current->tgid, MAJOR(in->i_rdev), MINOR(in->i_rdev)));
  return errorCode;
}

static int release(struct inode *in, struct file *f)
{
  int errorCode = 0;
  logk((KERN_INFO "Pid(%d) Release with (major,minor) = (%d,%d)\n", current->tgid, MAJOR(in->i_rdev), MINOR(in->i_rdev)));
  return errorCode;
}

static long ioctlTermDriver(struct file *f, unsigned int cmd, unsigned long arg)
{
  // check if valid command
  // otherwise we return -ENOTTY
  if (_IOC_TYPE(cmd) != IOCTL_THERM_MAGIC || _IOC_NR(cmd) > THERM_IOCTL_MAXNR)
  {
    return -ENOTTY;
  }
  switch(cmd)
  {
    case THERM_IOCTL_RESOLUTION:
      // check if argument is valid. Must be between MAXIMUM and MINIMUM from TemperatureResolution type (thermOperations.h)
      if (arg < MINIMUM || arg > MAXIMUM)
      {
        return -ENOTTY;
      }
      mCurrentSensor->resolution = arg;
      sensorSetNewResolution(*mCurrentSensor);
      break;
    default:
      return -ENOTTY;
      break;
  }
  return 0;
}

static int init(void)
{
  unsigned int numberOfSensors = 0;
  int errorCode = 0;
  int i;
  initializeOneWire(mGpioPin);
  mSensorsList = newLinkedList();
  numberOfSensors = discoverEachSensorID(mSensorsList);
  logk((KERN_INFO "Discovered %d sensors", numberOfSensors));
  mCurrentSensor = mSensorsList->getItemFromIndex(mSensorsList, 0);
  mNbOfMinors = numberOfSensors;

  /* dynamic allocation for major/minors */
  if (alloc_chrdev_region(&dev, 0, mNbOfMinors, "thermAlexMatt") < 0)
  {
    logk((KERN_ALERT ">>> ERROR alloc_chrdev_region\n"));
    return -EINVAL;
  }
  /* display majors/minor */
  logk((KERN_INFO "Init allocated (major, minor)=(%d,%d)\n",MAJOR(dev),MINOR(dev)));

  /* allocating memory for our character device and linking fileOperations */
  myDevice = cdev_alloc();
  if (myDevice == NULL)
  {
    logk((KERN_INFO "MYDEVICE == NULL"));
    return -EINVAL;
  }
  logk((KERN_INFO "Device allocated"));

  myDevice->ops = &fileOperations;
  myDevice->owner = THIS_MODULE;

  /* link between chrdev region and fileOperations */
  errorCode = cdev_add(myDevice,dev, mNbOfMinors);
  if (errorCode < 0)
  {
    logk((KERN_ALERT ">>> ERROR cdev_add\n"));
    return -EINVAL;
  }
  logk((KERN_INFO "Cdev added"));

  /* creation of /dev/ points */
  thermClass = class_create(THIS_MODULE, "thermAlexMatt");
  if (IS_ERR(thermClass)) 
  {
    logk((KERN_ALERT "failed to register device class\n"));
    goto failed_classreg;
  }
  logk((KERN_INFO "thermClass added!"));

  for ( i = 0; i < mNbOfMinors; ++i )
  {
    struct device* thermDevice = device_create(thermClass, NULL, MKDEV(MAJOR(dev), MINOR(dev)+i), NULL, "thermAlexMatt%d", i);
    if (IS_ERR(thermDevice))
    {
      logk((KERN_ALERT "failed to create device"));
      goto failed_devreg;
    }
  }
  logk((KERN_INFO "added %d devices to /dev/", mNbOfMinors));
  return 0;

failed_devreg:
  class_unregister(thermClass);
  class_destroy(thermClass);
failed_classreg:
  unregister_chrdev_region(dev, mNbOfMinors);
  return -1;
}

static void cleanup(void)
{
  int i;
  deleteBus();
  deleteLinkedList(mSensorsList);

  for ( i = 0; i < mNbOfMinors; ++i )
  {
    device_destroy(thermClass, MKDEV(MAJOR(dev), MINOR(dev)+i));
  }
  class_unregister(thermClass);
  class_destroy(thermClass);

  /* freeing memory and major,(s) */
  unregister_chrdev_region(dev,mNbOfMinors);
  cdev_del(myDevice);

  logk((KERN_INFO "Module exited clean"));
}

module_exit(cleanup);
module_init(init);

module_param(mGpioPin, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(mGpioPin, "Used GPIO port for the thermometer");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mattijs Korpershoek / Alexandre Montilla");
MODULE_DESCRIPTION("1-Wire Digital thermometer DS18B2 driver");
MODULE_SUPPORTED_DEVICE("DS18B2");
