/**
 * OneWire
 *
 * Handles the different one-wire timings
 * in order to communicate properly with the
 * Maxim DS18B2
 *
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 * Alexandre Montilla
 * <alexandre.montilla@gmail.com>
 */
#ifndef __INC_ONEWIRE__
#define __INC_ONEWIRE__

#include <asm/system.h> // for disable interruptions
#include <linux/delay.h>
#include <linux/types.h>

#include "BitOperations.h"

void initializeOneWire(int gpioPin);

int sendInitializationSequence(void);

void oneWireWriteBit(Bit bitToWrite);
Bit oneWireReadBit(void);

void oneWireWriteByte(u8 byteToWrite);
u8 oneWireReadByte(void);

#endif /* end of include guard: __INC_ONEWIRE__ */
