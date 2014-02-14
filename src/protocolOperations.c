/*
 * Handles the different timings
 * in order to communicate properly with the
 * Maxim DS18B2
 * 
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 */
#include "protocolOperations.h"

#define INITIALIZATION_LOW_DELAY 480
#define INITIALIZATION_HIGH_DELAY 60
#define WRITE_ONE_LOW_DELAY 15 
#define WRITE_ONE_HIGH_DELAY 45
#define WRITE_ZERO_PULL_DOWN_DELAY 60
#define READ_PULL_DOWN_DELAY 3
#define READ_HIGH_DELAY 3
#define READ_BIT_DELAY 54 // can be changed to 55 as specified in code

//Values from code
#if 0
#define INITIALIZATION_LOW_DELAY 480
#define INITIALIZATION_HIGH_DELAY 60
#define WRITE_ONE_LOW_DELAY 6
#define WRITE_ONE_HIGH_DELAY 64
#define WRITE_ZERO_PULL_DOWN_DELAY 60
#define READ_PULL_DOWN_DELAY 6
#define READ_HIGH_DELAY 9
#define READ_BIT_DELAY 55 // can be changed to 55 as specified in code
#endif

static void writeOneToBus( void );
static void writeZeroToBus( void );

void sendInitializationSequence( void )
{
  Bit result;

  /* generate a reset pulse */
  pullBus();
  writeBitGpio(ZERO);
  udelay(INITIALIZATION_LOW_DELAY);
  releaseBus();
  
  udelay(INITIALIZATION_HIGH_DELAY);

  /* read a presence pulse */
  result = readBitGpio();
  //msleep(1);
  if (result == ZERO)
  {
    printk(KERN_INFO "Device is present and answered\n");
  }
  else
  {
    printk(KERN_ALERT "ERROR: No device answered to initialization sequence\n");
  }
  releaseBus();
}

void writeByteToBus(u8 byteToWrite)
{
  int i;
  for ( i = 0; i < 7; i++ )
  {
    Bit bitToWrite = intToBit((byteToWrite >> i) & 0x1);
    writeBitToBus(bitToWrite);
  }
  //printk(KERN_INFO "Debug: send a byte (0x%2x) to the bus\n",  byteToWrite);
}

u8 readByteFromBus( void )
{
  int i;
  u8 result = 0;
  for ( i = 0; i < 7; i++ )
  {
    Bit readedBit = readBitFromBus();
    result |= BitToInt(readedBit) << i;
  }
  //printk(KERN_INFO "Debug: received a byte (0x%2x) from the bus\n",  result);
  return (result);
}

void writeBitToBus( Bit bitToWrite )
{
  if ( bitToWrite == ONE ) 
  {
    writeOneToBus();
  }
  else 
  {
    writeZeroToBus();
  }
}

static void writeOneToBus( void )
{
  pullBus();
  writeBitGpio(ZERO);
  udelay(WRITE_ONE_LOW_DELAY);
  releaseBus();
  udelay(WRITE_ONE_HIGH_DELAY);

  /* recovery time */
  releaseBus();
  udelay(2);
}

static void writeZeroToBus( void )
{
  pullBus();
  writeBitGpio(ZERO);
  udelay(WRITE_ZERO_PULL_DOWN_DELAY);

  /* recovery time */
  releaseBus();
  udelay(2);
}


/**
 * Note: MUST be called after 
 * a Read Command such as read scratchpad, for example
 */
Bit readBitFromBus( void )
{
  Bit result;
  pullBus();
  writeBitGpio(ZERO);
  udelay(READ_PULL_DOWN_DELAY);
  releaseBus();
  udelay(READ_HIGH_DELAY);

  // data from the DS18B20 is valid 15us after falling edge
  result = readBitGpio();

  udelay(READ_BIT_DELAY);
  //recovery time
  releaseBus();
  udelay(2);
  
  return result;
}
