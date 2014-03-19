/**
 * Copyright (c) 2014, Mattijs Korpershoek
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */
#include "DiscoveryProtocol.h"

static Bool mExploredBits[64] = {FALSE};

static int isBitIdZERO(Bit bit, Bit complementaryBit);
static int isBitIdONE(Bit bit, Bit complementaryBit);
static Bool isHighestUnknownBit(int currentBitIndex);

/*
 * discovery process of the sensors IDs.
 * Should be called after a SEARCH_ROM Command
 */
int performDiscovery(SensorID discoveredSensorID)
{
  int i;
  Bit responseBit, bit, complementaryBit;
  logk((KERN_INFO "Discovery protocol: started\n"));
  for ( i = 0; i < 64; ++i )
  {
    responseBit = ZERO;
    bit = oneWireReadBit();
    complementaryBit = oneWireReadBit();
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
    oneWireWriteBit(responseBit);
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
  for ( i = 0; i < 64; ++i )
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
  for ( i = currentBitIndex+1; i < 64-currentBitIndex; ++i )
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
