/*
 * thermOperations
 * 
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 * Alexandre Montilla
 * <alexandre.montilla@gmail.com>
 */
#include "thermOperations.h"

int readTemperature( void )
{
  /* code */
}

static void performTransaction( void )
{
  sendInitializationSequence();
  performRomCommand();
  performFunctionCommand();
}

