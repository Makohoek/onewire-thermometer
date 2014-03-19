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
#include "SensorID.h"

static u8 getByte(SensorID sensorID, int whichByte);

/**
 * Transfers the sensor ID on the one wire bus
 */
void writeSensorID(SensorID sensorID)
{
  int i;
  const int numberOfBytes = 64/8; // 64 bits, we send 8 bytes to transfer whole sensor id
  u8 byteToSend;
  for ( i = 0; i < numberOfBytes; ++i )
  {
    byteToSend = getByte(sensorID, i);
    oneWireWriteByte(byteToSend);
  }
}

/**
 * Get one of the bytes from the sensorID
 */
static u8 getByte(SensorID sensorID, int whichByte)
{
  int i, shift, startIndex, endIndex;
  u8 res = 0;
  shift = 7;
  startIndex = (whichByte+1)*8-1;
  endIndex = 8*whichByte;
  logk((KERN_INFO "getByte(%d): looping from %d -> %d", whichByte, startIndex, endIndex));
  for ( i = startIndex; i >= endIndex ; i--)
  {
    logk((KERN_INFO "Sensor(%d): %d", i, sensorID[i]));
    res |= (sensorID[i] << (shift--));
  }
  return res;
}

void affectSensorID(SensorID destination, SensorID source)
{
  int i;
  for ( i = 0; i < 64; ++i )
  {
    destination[i] = source[i];
  }
}

void printSensorID(SensorID sensorID)
{
  int i;
  for ( i = 0; i < 64; i+=8 )
  {
    printk(KERN_INFO "%d%d%d%d %d%d%d%d", sensorID[i],
        sensorID[i+1],
        sensorID[i+2],
        sensorID[i+3],
        sensorID[i+4],
        sensorID[i+5],
        sensorID[i+6],
        sensorID[i+7]
       );
  }
}
