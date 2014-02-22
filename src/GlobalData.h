#ifndef __INC_GLOBALDATA__
#define __INC_GLOBALDATA__

typedef enum 
{
  SKIP_ROM = 0xCC, 
  READ_ROM = 0x33, 
  MATCH_ROM  = 0x55, 
  SEARCH_ROM = 0xF0 
} ROMCommand;

// check real hex values here
typedef enum
{
  ALARM_SEARCH = 0xEC, 
  CONVERT_TEMP = 0x44, 
  READ_SCRATCHPAD = 0xBE, 
  READ_PSUPPLY = 0xB4, 
  RESUME_CMD = 0xA5
  /* missing commands can be found in kernel/drivers/w1/w1.h */
} FunctionCommand;

typedef enum 
{
  ZERO = 0,
  ONE = 1
} Bit;

typedef Bit SensorID[64];


#endif /* end of include guard: __INC_GLOBALDATA__ */
