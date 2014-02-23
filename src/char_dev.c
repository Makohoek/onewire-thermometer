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
#include "SensorID.h"

/* minor aliases */
static const unsigned char NB_OF_MINORS = 2;
static const unsigned char LED = 0;
static const unsigned char SENSOR = 1;

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

/* the current resolution for our sensor */
unsigned char mResolution = 12;

/* dev_t contains major and minor version */
dev_t dev;

/* struct cdev == a Character Device*/
struct cdev *myDevice;

/* led blinking for fun-only part */
static void blinkGpioLed(void);
/* read temperature from the sensor */
static int test_temperature_process(void);

static void setNewResolution(int newResolution);

static ssize_t read(struct file *f, char *buf, size_t size, loff_t *offset)
{
  int temperature = 0;
  int sizeNotCopiedToUser = 0;
  int sizeCopiedToUser = 0;
  int nbCharsTemperatureString = 0;
  TemperatureString temperatureString;
  logk((KERN_INFO "Read called!\n"));
  temperature = test_temperature_process();
  nbCharsTemperatureString = temperatureToString(temperatureString, temperature);
  sizeNotCopiedToUser = copy_to_user(buf, temperatureString, nbCharsTemperatureString);
  sizeCopiedToUser = nbCharsTemperatureString - sizeNotCopiedToUser;
  return sizeCopiedToUser;
}

static ssize_t read_led(struct file *f, char *buf, size_t size, loff_t *offset)
{
  setNewResolution(9);
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
  else if (MINOR(in->i_rdev) == SENSOR) 
  {
    fileOperations.read = read;
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

static int test_temperature_process(void)
{
  Scratchpad scratchpadData;
  long temperature = 0;
 
  /* CONVERT-T */
  logk((KERN_INFO "Sending an initialization sequence...\n"));
  sendInitializationSequence();
  writeROMCommand(MATCH_ROM);
  writeSensorID(discoveredID);
  writeFunctionCommand(CONVERT_TEMP);
  waitForConversionDone();
 
  /* READ_SCRATCHPAD */
  sendInitializationSequence();
  writeROMCommand(MATCH_ROM);
  logk((KERN_INFO "Send match rom to this ID:"));
  printSensorID(discoveredID);
  writeSensorID(discoveredID);
  logk((KERN_INFO "send READ_SCRATCHPAD"));
  writeFunctionCommand(READ_SCRATCHPAD);
  readScratchpad(scratchpadData);
  temperature = extractTemperatureFromScratchpad(scratchpadData, mResolution);
  logk((KERN_INFO "Readed temperature: %ld", temperature));
  return temperature;
}

static void setNewResolution(int newResolution)
{
  Scratchpad scratchpadData;
  buildScratchpadNewResolution(scratchpadData, newResolution);
  
  mResolution = newResolution;

  /* WRITE_SCRATCHPAD */
  logk((KERN_INFO "Sending an initialization sequence...\n"));
  sendInitializationSequence();
  writeROMCommand(MATCH_ROM);
  writeSensorID(discoveredID);
  writeFunctionCommand(WRITE_SCRATCHPAD);
  writeScratchpad(scratchpadData);
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
  test_discovery_process();
  setNewResolution(mResolution); // set default resolution
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
