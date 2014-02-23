#ifndef __INC_GLOBALDATA__
#define __INC_GLOBALDATA__

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


#endif /* end of include guard: __INC_GLOBALDATA__ */
