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

#include "DiscoveryProtocol.h"
#include "GlobalData.h"
#include "OneWire.h"
#include "SensorID.h"
#include "bitOperations.h"
#include "dmesgLogging.h"
#include "led.h"
#include "thermOperations.h"
#include "LinkedList.h"

/* minor aliases */
static const unsigned char NB_OF_MINORS = 2;
static const unsigned char LED = 0;
static const unsigned char SENSOR = 1;

/* IOCTL commands */
#define IOCTL_THERM_MAGIC 'k'
#define THERM_IOCTL_RESOLUTION _IO(IOCTL_THERM_MAGIC, 0)
#define THERM_IOCTL_MAXNR 0

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

/* Module parameters */
static int GpioPort = 2;

/* Standard character device operations */
static ssize_t read(struct file *f, char *buf, size_t size, loff_t *offset);
static ssize_t read_led(struct file *f, char *buf, size_t size, loff_t *offset);
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

/* struct cdev == a Character Device*/
struct cdev *myDevice;

LinkedList* mSensorsList;
Sensor* mCurrentSensor; // the current open one

/* led blinking for fun-only part */
static void blinkGpioLed(void);
/* read temperature from the sensor */
static int test_temperature_process(Sensor sensor);
static void setNewResolution(Sensor sensor);

// TODO change read to current openedSensor
static ssize_t read(struct file *f, char *buf, size_t size, loff_t *offset)
{
  int temperature = 0;
  int sizeNotCopiedToUser = 0;
  int sizeCopiedToUser = 0;
  int nbCharsTemperatureString = 0;
  TemperatureString temperatureString;
  mCurrentSensor = mSensorsList->getItemFromIndex(mSensorsList, 0);
  logk((KERN_INFO "Read called!\n"));
  temperature = test_temperature_process(*mCurrentSensor);
  nbCharsTemperatureString = temperatureToString(temperatureString, temperature);
  sizeNotCopiedToUser = copy_to_user(buf, temperatureString, nbCharsTemperatureString);
  sizeCopiedToUser = nbCharsTemperatureString - sizeNotCopiedToUser;
  return sizeCopiedToUser;
}

static ssize_t read_led(struct file *f, char *buf, size_t size, loff_t *offset)
{
  blinkGpioLed();
  return 0;
}

// change open based on them sensors
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
      setNewResolution(*mCurrentSensor);
      break;
    default:
      return -ENOTTY;
      break;
  }
  return 0;
}

static unsigned int discoverEachSensorID(LinkedList* sensorsList)
{
  Sensor* currentSensor;
  SensorID discoveredID;
  unsigned int numberOfSensors;
  numberOfSensors = 0;
  while (!isEverySensorDiscovered())
  {
    logk((KERN_INFO "Sending an initialization sequence...\n"));
    sendInitializationSequence();
    writeROMCommand(SEARCH_ROM);
    performDiscovery(discoveredID);

    currentSensor = kmalloc(GFP_KERNEL, sizeof(Sensor));
    if (currentSensor == NULL)
    {
      printk(KERN_ALERT "ERROR: failed to allocate memory");
      return 0;
    }
    affectSensorID(currentSensor->id, discoveredID);
    currentSensor->resolution = MAXIMUM;
    sensorsList->writeItem(sensorsList, currentSensor);
    numberOfSensors++;
  }
  return numberOfSensors;
}

static void initializeOneWire(void)
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
}

// add led turning on when processing sensor
static int test_temperature_process(Sensor sensor)
{
  Scratchpad scratchpadData;
  long temperature = 0;

  initializeLed();
  turnLedOn();
 
  /* CONVERT-T */
  logk((KERN_INFO "Sending an initialization sequence...\n"));
  sendInitializationSequence();
  writeROMCommand(MATCH_ROM);
  writeSensorID(sensor.id);
  writeFunctionCommand(CONVERT_TEMP);
  waitForConversionDone();
  turnLedOff();
  freeLed();
 
  /* READ_SCRATCHPAD */
  logk((KERN_INFO "Send match rom to this ID:"));
  printSensorID(sensor.id);

  sendInitializationSequence();
  writeROMCommand(MATCH_ROM);
  writeSensorID(sensor.id);
  logk((KERN_INFO "send READ_SCRATCHPAD"));
  writeFunctionCommand(READ_SCRATCHPAD);
  readScratchpad(scratchpadData);
  temperature = extractTemperatureFromScratchpad(scratchpadData, sensor.resolution);
  logk((KERN_INFO "Readed temperature: %ld", temperature));
  return temperature;
}

static void setNewResolution(Sensor sensor)
{
  Scratchpad scratchpadData;
  buildScratchpadNewResolution(scratchpadData, sensor.resolution);

  /* WRITE_SCRATCHPAD */
  logk((KERN_INFO "Sending an initialization sequence...\n"));
  sendInitializationSequence();
  writeROMCommand(MATCH_ROM);
  writeSensorID(sensor.id);
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
  int i;
  unsigned int numberOfSensors = 0;
  int errorCode = 0;
  initializeOneWire();
  mSensorsList = newLinkedList();
  numberOfSensors = discoverEachSensorID(mSensorsList);
  printk(KERN_INFO "Discovered %d sensors", numberOfSensors);

  /* dynamic allocation for major/minors */
  if (alloc_chrdev_region(&dev, 0, NB_OF_MINORS, "thermAlexMatt") == -1)
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
  deleteLinkedList(mSensorsList);
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
