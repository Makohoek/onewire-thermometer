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
#define WRITE_ONE_LOW_DELAY 7 
#define WRITE_ONE_HIGH_DELAY 45
#define WRITE_ZERO_PULL_DOWN_DELAY 60
#define READ_PULL_DOWN_DELAY 6
#define READ_HIGH_DELAY 6
#define READ_BIT_DELAY 45 // can be changed to 55 as specified in code

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

int sendInitializationSequence( void )
{
  Bit result, result2;
  /* generate a reset pulse */
  if (holdBus(ZERO) != 0)
  {
    logk((KERN_ALERT "impossible to HOLD BUS\n"));
  }
  writeBitGpio(ZERO);
  udelay(485);
  writeBitGpio(ONE);
  releaseBus();
  udelay(60);

  /* read a presence pulse */
  result = readBitGpio();
  udelay(240);
  result2 = readBitGpio();
  if (result == ZERO && result2 == ONE) // detected a presence pulse
  {
    logk((KERN_INFO "Device is present and answered\n"));
  }
  else
  {
    logk((KERN_ALERT "ERROR: No device answered to initialization sequence\n"));
  }
  udelay(240);
  return result;
}

void writeByteToBus(u8 byteToWrite)
{
  int i;
  for ( i = 0; i < 7; i++ )
  {
    Bit bitToWrite = intToBit((byteToWrite >> i) & 0x1);
    writeBitToBus(bitToWrite);
    udelay(2);
  }
  //logk((KERN_INFO "Debug: send a byte (0x%2x) to the bus\n",  byteToWrite);
}

u8 readByteFromBus( void )
{
  int i;
  u8 result = 0;
  for ( i = 0; i < 7; i++ )
  {
    Bit readedBit = readBitFromBus();
    result |= BitToInt(readedBit) << i;
    udelay(2);
  }
  //logk((KERN_INFO "Debug: received a byte (0x%2x) from the bus\n",  result);
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
  holdBus(ZERO);
  writeBitGpio(ZERO);
  udelay(6);
  writeBitGpio(ONE);
  releaseBus();
  udelay(64);
}

static void writeZeroToBus( void )
{
  holdBus(ZERO);
  writeBitGpio(ZERO);
  udelay(60);
  writeBitGpio(ONE);
  releaseBus();
  udelay(10);
}


/**
 * Note: MUST be called after 
 * a Read Command such as read scratchpad, for example
 */
Bit readBitFromBus( void )
{
  unsigned long flags;
  Bit result = ONE;
  local_irq_save(flags); // disable interruptions here because of critical timing
  holdBus(ZERO);
  writeBitGpio(ZERO);
  udelay(6);
  writeBitGpio(ONE);
  releaseBus();
  udelay(9);
  result = readBitGpio(); // data from the DS18B20 is valid 15us after falling edge
  local_irq_restore(flags);
  udelay(55);
  return result;
}
