/**
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 * Alexandre Montilla
 * <alexandre.montilla@gmail.com>
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/types.h> // include added for autocompletion
#include <linux/delay.h>

#include "thermOperations.h"
#include "DiscoveryProtocol.h"
#include "GlobalData.h"
#include "bitOperations.h"
#include "OneWire.h"
#include "dmesgLogging.h"
#include "led.h"

/* minor aliases */
static const unsigned char NB_OF_MINORS = 2;
static const unsigned char LED = 0;

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

/* Module parameters */
static int GpioPort = 2;

/* Standard character device operations */
static ssize_t read(struct file *f, char *buf, size_t size, loff_t *offset);
static ssize_t read_led(struct file *f, char *buf, size_t size, loff_t *offset);
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

/* the sensor id */
SensorID discoveredID = {ZERO};

/* dev_t contains major and minor version */
dev_t dev;

/* struct cdev == a Character Device*/
struct cdev *myDevice;

/* led blinking for fun-only part */
static void blinkGpioLed(void);

static ssize_t read(struct file *f, char *buf, size_t size, loff_t *offset)
{
  logk((KERN_INFO "Read called!\n"));
  //   sizeToRead = MIN(myLinkedList->bufferSize - myLinkedList->beginReadIndex, size); // this condition is not valid for a non destructive read
  //
  //      logk((KERN_INFO "BeginReadIndex: %ld from file\n", myLinkedList->beginReadIndex);
  //      logk((KERN_INFO "Have to read %d from file\n", sizeToRead);
  //
  //   myBuffer = (char*)kmalloc(sizeof(char)*sizeToRead, GFP_KERNEL);
  //
  //   myLinkedList->readFromLinkedList(myLinkedList, myBuffer, sizeToRead);
  //
  //   sizeNotCopiedToUser = copy_to_user(buf, myBuffer, sizeToRead);
  //   logk((KERN_INFO "Could not copy %d bytes to user\n", sizeNotCopiedToUser);
  //
  //   kfree(myBuffer);
  //
  //   return (sizeToRead-sizeNotCopiedToUser);
  return 0;
}

static ssize_t read_led(struct file *f, char *buf, size_t size, loff_t *offset)
{
  blinkGpioLed();
  return 0;
}

static int open(struct inode *in, struct file *f)
{
  int errorCode = 0;
  if (MINOR(in->i_rdev) == LED)
  {
    fileOperations.read = read_led;
  }
  logk((KERN_INFO "Pid(%d) Open with (major,minor) = (%d,%d)\n", current->tgid, MAJOR(in->i_rdev), MINOR(in->i_rdev)));
  return errorCode;
}

static int release(struct inode *in, struct file *f)
{
  int errorCode = 0;
  logk((KERN_INFO "Pid(%d) Release with (major,minor) = (%d,%d)\n", current->tgid, MAJOR(in->i_rdev), MINOR(in->i_rdev)));

  return errorCode;
}

static void test_discovery_process(void)
{
  /* displays GPIO port */
  logk((KERN_INFO "GpioPort=%d\n", GpioPort));
  if (initializeBus(GpioPort))
  {
    logk((KERN_INFO "Gpio initialized"));
  }
  else
  {
    logk((KERN_ALERT "ERROR while calling initializeGPIO"));
  }
  logk((KERN_INFO "Sending an initialization sequence...\n"));
  sendInitializationSequence();
  writeROMCommand(SEARCH_ROM);
  performDiscovery(discoveredID);
}

/* based on memory map manual page 7 */
/* from w1_therm.c */
static inline int convertToCelsius(u8 rom[9])
{
  s16 temperature = le16_to_cpup((__le16 *)rom);
  return temperature*1000/16;
}

static void test_temperature_process(void)
{
  u8 readedValues[9];
  int temperature = 0;
  /* attempt to read temperature */
  logk((KERN_INFO "Sending an initialization sequence...\n"));
  sendInitializationSequence();
  //writeROMCommand(SKIP_ROM);
  writeROMCommand(MATCH_ROM);
  writeSensorID(discoveredID);
  writeFunctionCommand(CONVERT_TEMP);
  waitForConversionDone();
  
  sendInitializationSequence();
  writeROMCommand(MATCH_ROM);
  logk((KERN_INFO "Send match rom to this ID:"));
  printSensorID(discoveredID);
  writeSensorID(discoveredID);
  logk((KERN_INFO "send READ_SCRATCHPAD"));
  writeFunctionCommand(READ_SCRATCHPAD);
  readTemperature(readedValues);
  
  temperature = convertToCelsius(readedValues);

  logk((KERN_INFO "Readed temperature: %d", temperature));
}

static void blinkGpioLed(void)
{
  int i;
  initializeLed();
  logk((KERN_INFO "Blinking led 3 times\n"));
  for ( i = 0; i < 3; i++ )
  {
    turnLedOn();
    msleep(1000);
    turnLedOff();
    msleep(1000);
  }
  freeLed();
}

static int init(void)
{
  int errorCode = 0;

  // trouver le nombre de capteurs.
  // TODO

  /* dynamic allocation for major/minors */
  if (alloc_chrdev_region(&dev, 0, NB_OF_MINORS, "sample") == -1)
  {
    logk((KERN_ALERT ">>> ERROR alloc_chrdev_region\n"));
    return -EINVAL;
  }
  /* display majors/minor */
  logk((KERN_INFO "Init allocated (major, minor)=(%d,%d)\n",MAJOR(dev),MINOR(dev)));

  /* allocating memory for our character device and linking fileOperations */
  myDevice = cdev_alloc();
  myDevice->ops = &fileOperations;
  myDevice->owner = THIS_MODULE;

  /* link between chrdev region and fileOperations */
  errorCode = cdev_add(myDevice,dev,NB_OF_MINORS);
  if (errorCode == -1)
  {
    logk((KERN_ALERT ">>> ERROR cdev_add\n"));
    return -EINVAL;
  }
  test_discovery_process();
  test_temperature_process();

 return(errorCode);
}


static void cleanup(void)
{
  deleteBus();
  /* freeing memory and major,(s) */
  unregister_chrdev_region(dev,NB_OF_MINORS);
  cdev_del(myDevice);
}

module_exit(cleanup);
module_init(init);

module_param(GpioPort, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(GpioPort, "Used GPIO port for the thermometer");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mattijs Korpershoek / Alexandre Montilla");
MODULE_DESCRIPTION("1-Wire Digital thermometer DS18B2 driver");
MODULE_SUPPORTED_DEVICE("DS18B2");
