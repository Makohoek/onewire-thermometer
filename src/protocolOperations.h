/*
 * Handles the different timings
 * order to communicate properly with the
 * Maxim DS18B2
 *
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 */
#ifndef __INC_PROTOCOLOPERATIONS__
#define __INC_PROTOCOLOPERATIONS__

#include "bitOperations.h"
#include "thermOperations.h"
#include "dmesgLogging.h"
#include <linux/types.h> 
#include <linux/delay.h>
#include <asm/system.h> // for disable interruptions

int sendInitializationSequence( void );

void writeByteToBus(u8 byteToWrite);
u8 readByteFromBus( void );

void writeBitToBus( Bit bitToWrite );
Bit readBitFromBus( void );

#endif /* end of include guard: __INC_PROTOCOLOPERATIONS__ */
