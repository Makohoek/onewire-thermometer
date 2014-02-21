/**
 * GPIO management based on http://www.mjmwired.net/kernel/Documentation/gpio.txt
 */
#include "bitOperations.h"

/* members */
static int mGpioPin = -1;

/* methods */
static void assertPinIsInitialized( void );

int initializeBitOperations( int GpioPin )
{
  if (!gpio_is_valid(GpioPin))
  {
    logk((KERN_ALERT "ERROR: %d pin is not valid\n", GpioPin));
    return -1;
  }
  int errorOccured;
  // initialize the port we want to work on
  mGpioPin = GpioPin;
  
  // request the gpio device
  //errorOccured = gpio_request(GpioPin, "Green test led"); // used when testing the Leds
  errorOccured = gpio_request_one(GpioPin, GPIOF_OPEN_DRAIN, "One-Wire Bus"); // used for w1 bus
  
  if (errorOccured)
  {
    logk((KERN_ALERT "ERROR occured requesting the GPIO pin\n"));
  }
  return (errorOccured);
}

void writeBitGpio( Bit bitToWrite )
{
  int bit;
  assertPinIsInitialized();
  bit = BitToInt(bitToWrite);
  gpio_set_value(mGpioPin, bit);
}

int holdBus(Bit initialBit)
{
  int bit, errorOccured;
  bit = BitToInt(initialBit);
  errorOccured = gpio_direction_output(mGpioPin, initialBit);
  if (errorOccured)
  {
    logk((KERN_ALERT "ERROR occured while attempting to hold the one wire bus\n"));
  }
  return errorOccured;
}

int releaseBus( void )
{
  int errorOccured = gpio_direction_input( mGpioPin );
  if (errorOccured)
  {
    logk((KERN_ALERT "ERROR occured while releasing the one wire bus\n"));
  }
  return errorOccured;
}

Bit readBitGpio( void )
{
  int bit;
  assertPinIsInitialized();
  bit = gpio_get_value( mGpioPin );
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

static void assertPinIsInitialized( void )
{
  if (mGpioPin == -1) 
  {
    logk((KERN_ERR "mGpioPin is not initialized: cannot operate on -1\n"));
  }
}

void deleteBitOperations( void )
{
  gpio_free(mGpioPin);
}
