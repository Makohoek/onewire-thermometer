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

float readTemperature( SensorID sensorID )
{
  /* code */
  return -1;
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
  int maxAttempts = 255;
  do
  {
    statusConversion = OneWireReadBit();
    maxAttempts--;
  }while(statusConversion != ONE && maxAttempts > 0); //waiting for the temperature to be fully converted to the scratchpad
  logk((KERN_INFO "Done converting the temperature"));
}

void writeSensorID(SensorID sensorID)
{
  int i;
  const int numberOfBytes = 64/8; // 64 bits, we send 8 bytes to transfer whole sensor id
  for ( i = 0; i < numberOfBytes; i++ )
  {
    /* code */
    //TODO
  }
}
