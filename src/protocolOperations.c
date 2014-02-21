/*
 * Handles the different timings
 * in order to communicate properly with the
 * Maxim DS18B2
 * 
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 */
#include "protocolOperations.h"

static unsigned long interruptFlags;
static void disableInterruptions(void);
static void enableInterruptions(void);

static void writeOneToBus( void );
static void writeZeroToBus( void );

int sendInitializationSequence( void )
{
  Bit presencePulse, inactiveBus;
  /* generate a reset pulse */
  holdBus(ZERO); 
  writeBitGpio(ZERO);
  udelay(485);
  releaseBus();
  udelay(60);
  /* read a presence pulse */
  presencePulse = readBitGpio();
  udelay(240);
  inactiveBus = readBitGpio();
  if (presencePulse == ZERO && inactiveBus == ONE) // detected a presence pulse
  {
    logk((KERN_INFO "Device is present and answered\n"));
  }
  else
  {
    logk((KERN_ALERT "ERROR: No device answered to initialization sequence\n"));
  }
  udelay(240);
  return presencePulse;
}

void writeByteToBus(u8 byteToWrite)
{
  int i;
  for ( i = 0; i < 8; i++ )
  {
    Bit bitToWrite = intToBit((byteToWrite >> i) & 0x1);
    writeBitToBus(bitToWrite);
    logk((KERN_INFO "send %d to bus\n", BitToInt(bitToWrite)));
    udelay(2);
  }
}

u8 readByteFromBus( void )
{
  int i;
  Bit readedBit;
  u8 result = 0;
  for ( i = 0; i < 8; i++ )
  {
    readedBit = readBitFromBus();
    result |= BitToInt(readedBit) << i;
    udelay(2);
  }
  return (result);
}

static void writeOneToBus( void )
{
  disableInterruptions();
  holdBus(ZERO);
  writeBitGpio(ZERO);
  udelay(6);
  releaseBus();
  udelay(64);
  enableInterruptions();
}

static void writeZeroToBus( void )
{
  disableInterruptions();
  holdBus(ZERO);
  writeBitGpio(ZERO);
  udelay(60);
  releaseBus();
  udelay(10);
  enableInterruptions();
}


/**
 * Note: MUST be called after
 * a Read Command such as read scratchpad, for example
 */
Bit readBitFromBus( void )
{
  Bit result;
  disableInterruptions();
  holdBus(ZERO);
  writeBitGpio(ZERO);
  udelay(6);
  releaseBus();
  udelay(9);
  result = readBitGpio();
  udelay(55);
  enableInterruptions();
  return result;
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

static void disableInterruptions(void)
{
  local_irq_save(interruptFlags);
}
static void enableInterruptions(void)
{
  local_irq_restore(interruptFlags);
}
