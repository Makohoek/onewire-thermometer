#include "TemperatureResolution.h"

#define CONFIGURATION_BYTE 4
static u8 getConfigurationByteFromResolution(TemperatureResolution resolution);

/**
 * creates a new scratchpad data which can be send after a WRITE_SCRATCHPAD command
 */
void buildScratchpadNewResolution(Scratchpad scratchpad, TemperatureResolution resolution)
{
  u8 configurationByte = 0x7f;
  if (resolution > 12 || resolution < 9)
  {
    logk((KERN_ALERT "Attempting to set a bad temperature resolution(%d)", resolution));
    return;
  }
  configurationByte = getConfigurationByteFromResolution(resolution);
  scratchpad[CONFIGURATION_BYTE] = configurationByte;
}

/* use the configuration register described in the manual page 8 */
static u8 getConfigurationByteFromResolution(TemperatureResolution resolution)
{
  if (resolution == MAXIMUM)
  {
    return 0b01111111;
  }
  if (resolution == HIGH)
  {
    return 0b01011111;
  }
  if (resolution == LOW)
  {
    return 0b00111111;
  }
  if (resolution == MINIMUM)
  {
    return 0b00011111;
  }
  return 0b01111111; // return default configuration if false resolution is given
}

