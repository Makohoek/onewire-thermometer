#include "DiscoveryProtocol.h"

static Bool mExploredBits[64] = {FALSE};

static int isBitIdZERO(Bit bit, Bit complementaryBit);
static int isBitIdONE(Bit bit, Bit complementaryBit);
static Bool isHighestUnknownBit(int currentBitIndex);

/*
 * discovery process of the sensors IDs.
 * Should be called after a SEARCH_ROM Command
 */
int performDiscovery( SensorID discoveredSensorID )
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
      mExploredBits[i] = TRUE;
    }
    else if (isBitIdONE(bit, complementaryBit))
    {
      responseBit = ONE;
      mExploredBits[i] = TRUE;
    }
    else if (bit == ONE && complementaryBit == ONE)
    {
      logk((KERN_ALERT "Discovery protocol[bit %d]: ERROR: protocol is not respected!!\n", i));
      return -1;
    }
    else
    {
      if (isHighestUnknownBit(i))
      {
        // its the last one, we can select ONE here and mark the bit as explored
        responseBit = ONE;
        mExploredBits[i] = TRUE;
      }
      else
      {
        // we dont know. We have to select which one we want to keep
        // Arbitrary we want to keep the ONE :)
        logk((KERN_INFO "Discovery protocol[bit %d]: don't know", i));
        responseBit = ZERO;
        mExploredBits[i] = FALSE;
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
  return 0;
}

Bool isEverySensorDiscovered(void)
{
  int i;
  for (i = 0; i < 64; ++i)
  {
    if (mExploredBits[i] == FALSE)
    {
      return FALSE;
    }
  }
  return TRUE;
}

static Bool isHighestUnknownBit(int currentBitIndex)
{
  int i;
  unsigned char result = TRUE;
  for (i = currentBitIndex+1; i < 64-currentBitIndex; ++i)
  {
    if (mExploredBits[i] == FALSE)
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

