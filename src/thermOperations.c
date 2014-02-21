/*
 * thermOperations
 * 
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 * Alexandre Montilla
 * <alexandre.montilla@gmail.com>
 */
#include "thermOperations.h"

//static void performTransaction( void ); //TODO

int readTemperature( void )
{
  /* code */
  return -1;
}

void printSensorID(Bit sensorID[64]);

int isBitIdZERO(Bit bit, Bit complementaryBit)
{
  return (ZERO == bit && ONE == complementaryBit);
}

int isBitIdONE(Bit bit, Bit complementaryBit)
{
  return (ONE == bit && ZERO == complementaryBit);
}

/*
 * discovery process of the sensors IDs.
 * Should be called after a SEARCH_ROM Command
 */
void performDiscovery( Bit discoveredSensorID[64] )
{
  int i;
  Bit responseBit, bit, complementaryBit;
  logk((KERN_INFO "Discovery protocol: started\n"));
  for ( i = 0; i < 64; i++ )
  {
    responseBit = ZERO;
    bit = readBitFromBus();
    complementaryBit = readBitFromBus();
    if (isBitIdZERO(bit, complementaryBit))
    {
      responseBit = ZERO;
    }
    else if (isBitIdONE(bit, complementaryBit))
    {
      responseBit = ONE;
    }
    else if (bit == ONE && complementaryBit == ONE)
    {
      printk(KERN_ALERT "Discovery protocol[bit %d]: ERROR: protocol is not respected!!\n", i);
      return;
    }
    else 
    {
      // we dont know. We have to select which one we want to keep
      // Arbitrary we want to keep the ONE :)
      logk((KERN_INFO "Discovery protocol[bit %d]: don't know", i));
      responseBit = ONE;
    }
    // select which one can survive
    writeBitToBus(responseBit);
    udelay(2);
    discoveredSensorID[i] = responseBit;
  }
  printk(KERN_INFO "Discovery protocol: ended\n");
  printk(KERN_INFO "Discovery protocol: Received this ID: ");
  printSensorID(discoveredSensorID);
}

void printSensorID(Bit sensorID[64])
{
  int i;
  for ( i = 0; i < 64; i+=4 )
  {
    printk(KERN_INFO "%d %d %d %d", sensorID[i] == ZERO ? 0:1,
        sensorID[i+1] == ZERO ? 0:1,
        sensorID[i+2] == ZERO ? 0:1,
        sensorID[i+3] == ZERO ? 0:1);
  }
}

void writeFunctionCommand(FunctionCommand functionCommand)
{
  writeByteToBus(functionCommand);
}

void writeROMCommand(ROMCommand romCommand)
{
  writeByteToBus(romCommand);
}
