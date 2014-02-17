/**
 * GPIO management based on http://www.mjmwired.net/kernel/Documentation/gpio.txt
 */
#include "bitOperations.h"
#include <linux/gpio.h>

/* members */
static int mGpioPin = -1;

/* methods */
static void assertPinIsInitialized( void );

int initializeBitOperations( int GpioPin )
{
  int errorOccured;
  // initialize the port we want to work on
  mGpioPin = GpioPin;
  // request the gpio device
  errorOccured = gpio_request(GpioPin, "One-Wire Bus");
  return (errorOccured);
}

void writeBitGpio( Bit bitToWrite )
{
  int bit;
  assertPinIsInitialized();
  bit = BitToInt(bitToWrite);
  gpio_set_value(mGpioPin, bit);
}

void holdBus(Bit initialBit)
{
  int bit, errorOccured;
  bit = BitToInt(initialBit);
  errorOccured = gpio_direction_output(mGpioPin, bit);
  if (errorOccured)
  {
    printk(KERN_ALERT "ERROR occured while attempting to hold the one wire bus\n");
  }
}

void releaseBus( void )
{
  int errorOccured = gpio_direction_input( mGpioPin );
  if (errorOccured)
  {
    printk(KERN_ALERT "ERROR occured while releasing the one wire bus\n");
  }
}

Bit readBitGpio( void )
{
  int bit;
  assertPinIsInitialized();
  bit = gpio_get_value( mGpioPin );
  return (intToBit(bit));
}

void writeDefaultBitGpio( void )
{
  holdBus(ONE);
  writeBitGpio(ONE);
  releaseBus();
}

inline int BitToInt(Bit myBit)
{
  return (myBit == ZERO) ? 0 : 1;
}

inline Bit intToBit(int myInt)
{
  return (myInt == 0) ? ZERO : ONE;
}

static void assertPinIsInitialized( void )
{
  if (mGpioPin == -1) 
  {
    printk(KERN_ERR "mGpioPin is not initialized: cannot operate on -1\n");
  }
}

void deleteBitOperations( void )
{
  gpio_free(mGpioPin);
}
