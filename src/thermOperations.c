/*
 * thermOperations
 * 
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 * Alexandre Montilla
 * <alexandre.montilla@gmail.com>
 */
#include "thermOperations.h"

static u8 getByte(SensorID sensorID, int whichByte);

void readTemperature( u8 readedScratchpad[9] )
{
  int i;
  for ( i = 0; i < 9; i++ )
  {
    readedScratchpad[i] = OneWireReadByte();
  }
  for ( i = 0; i < 9; i++ )
  {
    logk((KERN_INFO "%2x", readedScratchpad[i]));
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
  Bit statusConversion;
  const int maxAttempts = 100;
  int attempts = 0;
  mdelay(TCONV/8); // wait for the minimum precision at least. (93.75ms when resolution = 9)
  do
  {
    logk((KERN_INFO "Waiting for conversion... (attempt %3d/%3d)", attempts, maxAttempts));
    statusConversion = OneWireReadBit();
    attempts++;
  }while(statusConversion != ONE && attempts < maxAttempts); //waiting for the temperature to be fully converted to the scratchpad
  if (attempts >= maxAttempts)
  {
    logk((KERN_ALERT "ERROR: Sensor did not finished conversion in %d attempts!", maxAttempts));
  }
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
static u8 getByte(SensorID sensorID, int whichByte)
{
  int i, shift, startIndex, endIndex;
  u8 res = 0;
  shift = 7;
  startIndex = (whichByte+1)*8-1;
  endIndex = 8*whichByte;
  logk((KERN_INFO "getByte(%d): looping from %d -> %d", whichByte, startIndex, endIndex));
  for (i = startIndex; i >= endIndex ; i--)
  {
    logk((KERN_INFO "Sensor(%d): %d", i, sensorID[i]));
    res |= (sensorID[i] << (shift--));
  }
  return res;
}
