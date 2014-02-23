#include "DiscoveryProtocol.h"

static int isBitIdZERO(Bit bit, Bit complementaryBit);
static int isBitIdONE(Bit bit, Bit complementaryBit);

/*
 * discovery process of the sensors IDs.
 * Should be called after a SEARCH_ROM Command
 */
void performDiscovery( SensorID discoveredSensorID )
{
  int i;
  Bit responseBit, bit, complementaryBit;
  logk((KERN_INFO "Discovery protocol: started\n"));
  for ( i = 0; i < 64; i++ )
  {
    responseBit = ZERO;
    bit = OneWireReadBit();
    complementaryBit = OneWireReadBit();
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
      logk((KERN_ALERT "Discovery protocol[bit %d]: ERROR: protocol is not respected!!\n", i));
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
    OneWireWriteBit(responseBit);
    udelay(2);
    discoveredSensorID[i] = responseBit;
  }
  logk((KERN_INFO "Discovery protocol: ended\n"));
  logk((KERN_INFO "Discovery protocol: Received this ID: "));
  printSensorID(discoveredSensorID);
}

static int isBitIdZERO(Bit bit, Bit complementaryBit)
{
  return (ZERO == bit && ONE == complementaryBit);
}

static int isBitIdONE(Bit bit, Bit complementaryBit)
{
  return (ONE == bit && ZERO == complementaryBit);
}

