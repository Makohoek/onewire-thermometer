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
/**
 * GlobalData
 *
 * All the interfaces used
 * to communicate between functions
 */
#ifndef __INC_GLOBALDATA__
#define __INC_GLOBALDATA__

#include <linux/types.h>

typedef enum
{
  FALSE=0,
  TRUE=1,
} Bool;

typedef enum 
{
  SKIP_ROM = 0xCC,
  READ_ROM = 0x33,
  MATCH_ROM  = 0x55,
  SEARCH_ROM = 0xF0
} ROMCommand;

typedef enum
{
  CONVERT_TEMP = 0x44,
  READ_SCRATCHPAD = 0xBE,
  WRITE_SCRATCHPAD = 0x4E,
  /* missing commands can be found in kernel/drivers/w1/w1.h */
} FunctionCommand;

typedef enum
{
  ZERO = 0,
  ONE = 1
} Bit;

typedef u8 Scratchpad[9];

typedef Bit SensorID[64];

typedef enum
{
  MAXIMUM=12,
  HIGH=11,
  LOW=10,
  MINIMUM=9
} TemperatureResolution;

typedef struct
{
  SensorID id;
  TemperatureResolution resolution;
}Sensor;

#endif /* end of include guard: __INC_GLOBALDATA__ */
