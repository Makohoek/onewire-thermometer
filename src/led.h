/*
 * led
 * 
 * Led module for the OlinuXinO board
 * Can be used to test if gpio communication is working correctly
 *
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 * Alexandre Montilla
 * <alexandre.montilla@gmail.com>
 */
#ifndef __INC_LEDOPERATION__
#define __INC_LEDOPERATION__

#include <linux/delay.h>
#include <linux/gpio.h>

#include "Klogger.h"

void blinkGpioLed(void);

void turnLedOn( void );
void turnLedOff( void );

int initializeLed( void );
void freeLed( void );

#endif /* end of include guard: __INC_LEDOPERATION__ */
