/*
 * thermOperations
 * 
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 * Alexandre Montilla
 * <alexandre.montilla@gmail.com>
 */
#include "thermOperations.h"

u8 getByte(SensorID sensorID, int whichByte);

void readTemperature( u8 readedScratchpad[9] )
{
  int i;
  for ( i = 0; i < 9; i++ )
  {
    readedScratchpad[i] = OneWireReadByte();
  }
  for ( i = 0; i < 9; i++ )
  {
    logk((KERN_INFO "%x", readedScratchpad[i]));
  }
}

void writeFunctionCommand(FunctionCommand functionCommand)
{
  OneWireWriteByte(functionCommand);
}

void writeROMCommand(ROMCommand romCommand)
{
  OneWireWriteByte(romCommand);
}

void waitForConversionDone(void)
{
  mdelay(TCONV);
  Bit statusConversion;
  int maxAttempts = 255;
  do
  {
    statusConversion = OneWireReadBit();
    maxAttempts--;
  }while(statusConversion != ONE && maxAttempts > 0); //waiting for the temperature to be fully converted to the scratchpad
  logk((KERN_INFO "Done converting the temperature"));
}

/**
 * Transfers the sensor ID on the one wire bus
 */
void writeSensorID(SensorID sensorID)
{
  int i;
  const int numberOfBytes = 64/8; // 64 bits, we send 8 bytes to transfer whole sensor id
  u8 byteToSend;
  for ( i = 0; i < numberOfBytes; i++ )
  {
    byteToSend = getByte(sensorID, i);
    OneWireWriteByte(byteToSend);
  }
}

/**
 * Get one of the bytes from the sensorID
 */
u8 getByte(SensorID sensorID, int whichByte)
{
  int i, startIndex, endIndex;
  u8 res = 0;
  startIndex = (whichByte+1)*8-1;
  endIndex = 8*whichByte;
  logk((KERN_INFO "getByte(%d): looping from %d -> %d", whichByte, startIndex, endIndex));
  for (i = startIndex; i >= endIndex ; i--)
  {
    res |= sensorID[i];
    res = res << 1;
  }
  return res;
}
