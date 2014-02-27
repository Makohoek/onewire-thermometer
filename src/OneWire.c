/**
 * Handles the different one-wire timings
 * in order to communicate properly with the
 * Maxim DS18B2
 * 
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 * Alexandre Montilla
 * <alexandre.montilla@gmail.com>
 */
#include "OneWire.h"

static unsigned long interruptFlags;
static void OneWireWriteOneSlot( void );
static void OneWireWriteZeroSlot( void );
static void disableInterruptions(void);
static void enableInterruptions(void);

int sendInitializationSequence( void )
{
  Bit presencePulse, inactiveBus;
  /* generate a reset pulse */
  pullBusLow();
  udelay(485);
  releaseBus();
  udelay(60);
  /* read a presence pulse */
  presencePulse = readBus();
  udelay(240);
  inactiveBus = readBus();
  if (presencePulse == ZERO && inactiveBus == ONE) // detected a presence pulse
  {
    logk((KERN_INFO "Device is present and answered\n"));
  }
  else
  {
    logk((KERN_ALERT "ERROR: No device answered to initialization sequence\n"));
    presencePulse = -1;
  }
  udelay(240);
  return presencePulse;
}

void OneWireWriteByte(u8 byteToWrite)
{
  int i;
  for ( i = 0; i < 8; i++ )
  {
    Bit bitToWrite = intToBit((byteToWrite >> i) & 0x1);
    OneWireWriteBit(bitToWrite);
    logk((KERN_INFO "send %d to 1w bus\n", BitToInt(bitToWrite)));
    udelay(2);
  }
}

u8 OneWireReadByte( void )
{
  int i;
  Bit readedBit;
  u8 result = 0;
  for ( i = 0; i < 8; i++ )
  {
    readedBit = OneWireReadBit();
    result |= BitToInt(readedBit) << i;
    udelay(2);
  }
  return (result);
}

static void OneWireWriteOneSlot( void )
{
  disableInterruptions();
  pullBusLow();
  udelay(6);
  releaseBus();
  udelay(64);
  enableInterruptions();
}

static void OneWireWriteZeroSlot( void )
{
  disableInterruptions();
  pullBusLow();
  udelay(60);
  releaseBus();
  udelay(10);
  enableInterruptions();
}

/**
 * Note: MUST be called after
 * a Read Command such as read scratchpad, for example
 */
Bit OneWireReadBit( void )
{
  Bit result;
  disableInterruptions();
  pullBusLow();
  udelay(6);
  releaseBus();
  udelay(9);
  result = readBus();
  udelay(55);
  enableInterruptions();
  return result;
}

void OneWireWriteBit( Bit bitToWrite )
{
  if ( bitToWrite == ONE )
  {
    OneWireWriteOneSlot();
  }
  else
  {
    OneWireWriteZeroSlot();
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
