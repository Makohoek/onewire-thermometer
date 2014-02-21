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

//static void performTransaction( void )
//{
//  //TODO
//  //sendInitializationSequence();
//  //performRomCommand();
//  //performFunctionCommand();
//}
//

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
void performDiscovery( void )
{
  logk((KERN_INFO "Performing discovery on the bus\n"));
  int i;
  Bit discoveredSensorID[64];
  for ( i = 0; i < 64; i++ )
  {
    Bit responseBit = ZERO;
    Bit bit = readBitFromBus();
    udelay(2);
    Bit complementaryBit = readBitFromBus();
    udelay(2);
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
      logk((KERN_INFO "Discovery protocol (bit %d) is not respected!!\n", i));
      responseBit = ZERO;
      return;
    }
    else 
    {
      // we dont know. We have to select which one we want to keep
      // Arbitrary we want to keep the ONE :)
      logk((KERN_INFO "Dont know"));
      responseBit = ONE;
    }
    // select which one can survive
    writeBitToBus(responseBit);
    udelay(2);
    discoveredSensorID[i] = responseBit;
  }
  logk((KERN_INFO "End of discovery on the bus\n"));
  logk((KERN_INFO "Received this ID: "));
  for ( i = 0; i < 64; i+=4 )
  {
    logk((KERN_INFO "%d %d %d %d", discoveredSensorID[i] == ZERO ? 0:1,
        discoveredSensorID[i] == ZERO ? 0:1,
        discoveredSensorID[i+1] == ZERO ? 0:1,
        discoveredSensorID[i+2] == ZERO ? 0:1));
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


