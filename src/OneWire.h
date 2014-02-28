/*
 * Handles the different timings
 * order to communicate properly with the
 * Maxim DS18B2
 *
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 */
#ifndef __INC_ONEWIRE__
#define __INC_ONEWIRE__

#include <linux/types.h> 
#include <linux/delay.h>
#include <asm/system.h> // for disable interruptions

#include "GlobalData.h"
#include "bitOperations.h"
#include "Klogger.h"

int sendInitializationSequence( void );

void OneWireWriteBit( Bit bitToWrite );
Bit OneWireReadBit( void );

void OneWireWriteByte( u8 byteToWrite );
u8 OneWireReadByte( void );

#endif /* end of include guard: __INC_ONEWIRE__ */
