#include "bitOperations.h"
#include <linux/gpio.h>
#include "/media/a852afe7-e4bb-48b6-a1da-7583947c84d4/linux-sunxi/include/linux/gpio.h" // include added for autocompletion

/* members */
static int mGpioPin = -1;

/* methods */
static void assertPinIsInitialized( void );

int initializeBitOperations( int GpioPin )
{
  // initialize the port we want to work on
  mGpioPin = GpioPin;
  return 0;
}

void writeBitGpio( Bit bitToWrite )
{
  int bit;
  assertPinIsInitialized();
  bit = BitToInt(bitToWrite);
  gpio_direction_output(mGpioPin, bit);
  gpio_set_value(mGpioPin, bit);
}

Bit readBitGpio( void )
{
  int bit;
  assertPinIsInitialized();
  gpio_direction_input( mGpioPin );
  bit = gpio_get_value( mGpioPin );
  return (intToBit(bit));
}

void writeDefaultBitGpio( void )
{
  writeBitGpio(ONE);
}


inline int BitToInt(Bit myBit)
{
  return (myBit == ZERO) ? 0 : 1;
}

inline Bit intToBit(int myInt)
{
  return (myInt == 0) ? ZERO : ONE;
}

static void assertPinIsInitialized( )
{
  if (mGpioPin == -1) 
  {
    printk(KERN_ERR "mGpioPin is not initialized: cannot operate on -1\n");
  }
}


