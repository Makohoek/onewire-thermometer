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
