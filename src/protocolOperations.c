/*
 * Handles the different timings
 * in order to communicate properly with the
 * Maxim DS18B2
 * 
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 */
#include "protocolOperations.h" 

void sendInitializationSequence( void )
{
  /* generate a reset pulse */
  writeBitGpio(ZERO);
  udelay(480);

  writeBitGpio(ONE);
  udelay(70);

  /* read a presence pulse */
  result = readBitGpio();
  msleep(1);
  if (result == ZERO)
  {
    printk(KERN_INFO "Device is present and answered\n");
  }
  else
  {
    printk(KERN_ALERT "ERROR: No device answered to initialization sequence\n");
  }
  writeDefaultBitGpio();
}

void writeBitToBus( Bit bitToWrite )
{
  if (bitToWrite == ONE) 
  {
    writeBitGpio(ZERO);
    udelay(15);
    writeBitGpio(ONE);
    udelay(45);
  }  
  else 
  {
    writeBitGpio(ZERO);
    udelay(60);
    writeBitGpio(ONE);
  }
  udelay(1);
  writeDefaultBitGpio();
}

/**
 * Note: MUST be called after 
 * a Read Command such as read scratchpad, for example
 */
Bit readBitFromBus( void )
{
  Bit result;
  writeBitGpio(ZERO);
  udelay(6);
  writeBitGpio(ONE);
  udelay(9);
  // data from the DS18B20 is valid 15us after falling edge
  result = readBitGpio();

  udelay(55);
  writeDefaultBitGpio();

  return result;
}

