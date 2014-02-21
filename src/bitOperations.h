/*
 * bitOperations
 * 
 * Interface to the GPIO 
 * Read linux-sunxi/Documentation/gpio.txt for more information
 *
 * Interface can be found here: linux-sunx/drivers/gpio/gpio-sunxi.{c, h}
 *
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 * Alexandre Montilla
 * <alexandre.montilla@gmail.com>
 */
#ifndef __INC_BITOPERATIONS__
#define __INC_BITOPERATIONS__

#include <linux/gpio.h>
#include "dmesgLogging.h"

typedef enum 
{
  ZERO = 0,
  ONE = 1
} Bit;

int initializeBitOperations( int GpioPin );
void deleteBitOperations( void );

int releaseBus( void );
int holdBus(Bit initialBit);

void writeBitGpio( Bit bitToWrite );
Bit readBitGpio( void );

int BitToInt(Bit myBit);
Bit intToBit(int myInt);

#endif /* end of include guard: __INC_BITOPERATIONS__ */
