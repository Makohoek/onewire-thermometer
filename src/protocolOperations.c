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

void writeByteToBus(u8 byteToWrite)
{
  int i;
  for ( i = 0; i < 7; i++ )
  {
    Bit bitToWrite = intToBit((byteToWrite >> i) & 0x1);
    writeBitToBus(bitToWrite);
  }
  printk(KERN_INFO "Debug: send a byte (0x%2x) to the bus\n",  byteToWrite);
}

u8 readByteFromBus( void )
{
  u8 result;
  int i;
  for ( i = 0; i < 7; i++ )
  {
    Bit readedBit = readBitFromBus();
    result |= BitToInt(readBit) << i;
  }
  printk(KERN_INFO "Debug: received a byte (0x%2x) to the bus\n",  byteToWrite);
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

void writeFunctionCommand(FunctionCommand functionCommand)
{
    writeByteToBus(functionCommand);
}

void writeROMCommand(ROMCommand romCommand)
{
    writeByteToBus(romCommand);
}



