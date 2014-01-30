/*
 * thermOperations
 * 
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 * Alexandre Montilla
 * <alexandre.montilla@gmail.com>
 */
#ifndef __INC_THERMOPERATIONS__
#define __INC_THERMOPERATIONS__
#include "bitOperations.h"

typedef enum 
{
  SKIP_ROM = 0xCC, 
  READ_ROM = 0x33, 
  MATCH_ROM  = 0x55, 
} ROMCommand;

// check real hex values here
typedef enum 
{
  SEARCH = 0xF0, 
  ALARM_SEARCH = 0xEC, 
  CONVERT_TEMP = 0x44, 
  READ_SCRATCHPAD = 0xBE, 
  READ_PSUPPLY = 0xB4, 
  RESUME_CMD = 0xA5
  /* missing commands can be found in kernel/drivers/w1/w1.h */
} FunctionCommand;


int readTemperature( void ); //TODO
static void performTransaction( void ); //TODO

#endif /* end of include guard: __INC_THERMOPERATIONS__ */
