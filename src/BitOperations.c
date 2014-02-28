/**
 * GPIO management based on http://www.mjmwired.net/kernel/Documentation/gpio.txt
 */
#include "BitOperations.h"

/* members */
static int mGpioPin = -1;

/* methods */
static void assertPinIsValid(void);

static int outputGpio(Bit initialBit);
static int inputGpio(void);
static void writeGpio(Bit bitToWrite);
static Bit readGpio(void);

void pullBusLow(void)
{
  outputGpio(ZERO);
  writeGpio(ZERO);
}

void releaseBus(void)
{
  inputGpio();
}

Bit readBus(void)
{
  return readGpio();
}

static void writeGpio(Bit bitToWrite)
{
  int bit;
  assertPinIsValid();
  bit = BitToInt(bitToWrite);
  gpio_set_value(mGpioPin, bit);
}

static int outputGpio(Bit initialBit) // takes an argument to avoid bit glitching
{
  int bit, errorOccured;
  assertPinIsValid();
  bit = BitToInt(initialBit);
  errorOccured = gpio_direction_output(mGpioPin, initialBit);
  return errorOccured;
}

static int inputGpio(void)
{
  int errorOccured;
  assertPinIsValid();
  errorOccured = gpio_direction_input(mGpioPin);
  if (errorOccured)
  {
    logk((KERN_ALERT "Error when setting %d pin to input\n", mGpioPin));
  }
  return errorOccured;
}

Bit readGpio(void)
{
  int bit;
  assertPinIsValid();
  bit = gpio_get_value(mGpioPin);
  return (intToBit(bit));
}

int BitToInt(Bit myBit)
{
  return (myBit == ZERO) ? 0 : 1;
}

Bit intToBit(int myInt)
{
  return (myInt == 0) ? ZERO : ONE;
}

static void assertPinIsValid(void)
{
  if (mGpioPin == -1 || !gpio_is_valid(mGpioPin))
  {
    logk((KERN_ALERT "ERROR: %d pin is not valid\n", mGpioPin));
  }
}

int initializeBus(int GpioPin)
{
  int errorOccured;
  mGpioPin = GpioPin;
  if (!gpio_is_valid(mGpioPin))
  {
    logk((KERN_ALERT "ERROR: %d pin is not valid\n", mGpioPin));
    return -1;
  }
  // initialize the port we want to work on
  // request the gpio device
  errorOccured = gpio_request_one(mGpioPin, GPIOF_OPEN_DRAIN, "One-Wire Bus"); // used for w1 bus
  if (errorOccured)
  {
    logk((KERN_ALERT "ERROR occured requesting the GPIO pin\n"));
  }
  return (errorOccured);
}

void deleteBus(void)
{
  gpio_free(mGpioPin);
}
