#include "OneWire.h"

static unsigned long interruptFlags;
static void oneWireWriteOneSlot(void);
static void oneWireWriteZeroSlot(void);
static void disableInterruptions(void);
static void enableInterruptions(void);

void initializeOneWire(int gpioPin)
{
  /* displays GPIO port */
  logk((KERN_INFO "gpioPin=%d\n", gpioPin));
  if (initializeBus(gpioPin))
  {
    logk((KERN_INFO "Gpio initialized"));
  }
  else
  {
    logk((KERN_ALERT "ERROR while calling initializeGPIO"));
  }
}

int sendInitializationSequence(void)
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

void oneWireWriteByte(u8 byteToWrite)
{
  int i;
  for ( i = 0; i < 8; ++i )
  {
    Bit bitToWrite = intToBit((byteToWrite >> i) & 0x1);
    oneWireWriteBit(bitToWrite);
    logk((KERN_INFO "send %d to 1w bus\n", BitToInt(bitToWrite)));
    udelay(2);
  }
}

u8 oneWireReadByte(void)
{
  int i;
  Bit readedBit;
  u8 result = 0;
  for ( i = 0; i < 8; ++i )
  {
    readedBit = oneWireReadBit();
    result |= BitToInt(readedBit) << i;
    udelay(2);
  }
  return (result);
}

static void oneWireWriteOneSlot(void)
{
  disableInterruptions();
  pullBusLow();
  udelay(6);
  releaseBus();
  udelay(64);
  enableInterruptions();
}

static void oneWireWriteZeroSlot(void)
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
Bit oneWireReadBit(void)
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

void oneWireWriteBit(Bit bitToWrite)
{
  if ( bitToWrite == ONE )
  {
    oneWireWriteOneSlot();
  }
  else
  {
    oneWireWriteZeroSlot();
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
