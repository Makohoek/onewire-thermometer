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
#include "protocolOperations.h"

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>

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


int readTemperature( void ); //TODO
void performDiscovery( void );
void writeFunctionCommand(FunctionCommand command);
void writeROMCommand(ROMCommand romcommand);

#endif /* end of include guard: __INC_THERMOPERATIONS__ */
