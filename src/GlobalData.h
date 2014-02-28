/**
 * GlobalData
 *
 * All the interfaces used
 * to communicate between functions
 *
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 * Alexandre Montilla
 * <alexandre.montilla@gmail.com>
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
