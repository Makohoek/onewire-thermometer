/*
 * thermOperations
 * 
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 * Alexandre Montilla
 * <alexandre.montilla@gmail.com>
 */
#include "thermOperations.h"

/**
 * Read the scratchpad data after a READ_SCRATCHPAD function command
 */
void readScratchpad(Scratchpad readedScratchpad)
{
  int i;
  for ( i = 0; i < 9; i++ )
  {
    readedScratchpad[i] = OneWireReadByte();
  }
  for ( i = 0; i < 9; i++ )
  {
    logk((KERN_INFO "readScratchpad(%d): received %2x", i, readedScratchpad[i]));
  }
}

/**
 * Write the scratchpad data after a WRITE_SCRATCHPAD function command
 * */
void writeScratchpad(Scratchpad scratchpad)
{
  int i;
  for ( i = 0; i < 9; i++ )
  {
    OneWireWriteByte(scratchpad[i]);
  }
  for ( i = 0; i < 9; i++ )
  {
    logk((KERN_INFO "writeScratchpad(%d): send %2x",i , scratchpad[i]));
  }
}

/* use the configuration register described in the manual page 8 */
void setNewResolution(int howManyBits)
{
  u8 configurationByte;
  if (howManyBits > 12 || howManyBits < 9)
  {
    logk((KERN_ALERT "Attempting to set a bad temperature resolution(%d)", howManyBits));
    return;
  }
  if (howManyBits == 12)
  {
    //TODO
  }

}

/* based on memory map manual page 7 */
/* from w1_therm.c */
long extractTemperatureFromScratchpad(Scratchpad scratchpadData)
{
  s16 temperature = le16_to_cpup((__le16 *)scratchpadData);
  return temperature*1000/16;
}

int temperatureToString(TemperatureString result, long temperature)
{
  sprintf(result, "%ld.%ld", temperature/1000, temperature%1000);
  return (strlen(result)+1) * sizeof(char); //+1 for also copying the and the \0'
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
  mdelay(TCONV/8/2); // wait for the half of the minimum precision at least. (93.75/2 ms when resolution = 9)
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

