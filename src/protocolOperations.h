/*
 * Handles the different timings
 * in order to communicate properly with the
 * Maxim DS18B2
 * 
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 */
#ifndef __INC_PROTOCOLOPERATIONS__
#define __INC_PROTOCOLOPERATIONS__

#include "bitOperations.h"
#include "protocolOperations.h" 
#include "thermOperations.h"

void sendInitializationSequence( void );

void writeFunctionCommand(FunctionCommand command);
void writeROMCommand(ROMCommand romcommand);

void writeByteToBus(u8 byteToWrite);
u8 readByteFromBus( void );

void writeBitToBus( Bit bitToWrite );
Bit readBitFromBus( void );

#endif /* end of include guard: __INC_PROTOCOLOPERATIONS__ */
