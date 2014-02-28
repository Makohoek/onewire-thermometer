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
    OneWireWriteByte(byteToSend);
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
