#include "DiscoveryProtocol.h"

#define TRUE 1
#define FALSE 0

static int isBitIdZERO(Bit bit, Bit complementaryBit);
static int isBitIdONE(Bit bit, Bit complementaryBit);
unsigned char isHighestUnknownBit(unsigned char exploredBits[64], int currentBitIndex);

/*
 * discovery process of the sensors IDs.
 * Should be called after a SEARCH_ROM Command
 */
void performDiscovery( SensorID discoveredSensorID )
{
  static unsigned char exploredBits[64] = {FALSE};
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
      exploredBits[i] = TRUE;
    }
    else if (isBitIdONE(bit, complementaryBit))
    {
      responseBit = ONE;
      exploredBits[i] = TRUE;
    }
    else if (bit == ONE && complementaryBit == ONE)
    {
      logk((KERN_ALERT "Discovery protocol[bit %d]: ERROR: protocol is not respected!!\n", i));
      return;
    }
    else
    {
      if (isHighestUnknownBit(exploredBits, i))
      {
        // its the last one, we can select ONE here and mark the bit as explored
        responseBit = ONE;
        exploredBits[i] = TRUE;
      }
      else
      {
        // we dont know. We have to select which one we want to keep
        // Arbitrary we want to keep the ONE :)
        logk((KERN_INFO "Discovery protocol[bit %d]: don't know", i));
        responseBit = ZERO;
        exploredBits[i] = FALSE;
      }
    }
    // select which one can survive
    OneWireWriteBit(responseBit);
    udelay(2);
    discoveredSensorID[i] = responseBit;
  }
  logk((KERN_INFO "Discovery protocol: ended\n"));
  printk(KERN_INFO "Discovery protocol: Received this ID: ");
  printSensorID(discoveredSensorID);
}

unsigned char isHighestUnknownBit(unsigned char exploredBits[64], int currentBitIndex)
{
  int i;
  unsigned char result = TRUE;
  for (i = currentBitIndex+1; i < 64-currentBitIndex; ++i)
  {
    if (exploredBits[i] == FALSE)
    {
      return FALSE;
    }
  }
  return result;
}

static int isBitIdZERO(Bit bit, Bit complementaryBit)
{
  return (ZERO == bit && ONE == complementaryBit);
}

static int isBitIdONE(Bit bit, Bit complementaryBit)
{
  return (ONE == bit && ZERO == complementaryBit);
}

