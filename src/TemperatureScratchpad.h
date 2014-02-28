/**
 * TemperatureScratchpad
 *
 * Handles temperature receiving,
 * conversion and scratchpad read/write
 *
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 * Alexandre Montilla
 * <alexandre.montilla@gmail.com>
 */
#ifndef TEMPERATURESCRATCHPAD_H_RBATPHRG
#define TEMPERATURESCRATCHPAD_H_RBATPHRG

#include <linux/string.h>
#include <linux/types.h>

#include "OneWire.h"

#define MAX_CHARS_TEMPERATURE 14 // max precision = 12, one char for '.' and one for '\0'
typedef char TemperatureString[MAX_CHARS_TEMPERATURE];

// max time to convert temperature from the sensor.
// refer to Table-2 from documentation
#define TCONV 750

long extractTemperatureFromScratchpad(Scratchpad scratchpadData, TemperatureResolution resolution);
int temperatureToString(TemperatureString result, long temperature);
void waitForConversionDone(void);
void readScratchpad(Scratchpad readedScratchpad);
void writeScratchpad(Scratchpad scratchpad);

#endif /* end of include guard: TEMPERATURESCRATCHPAD_H_RBATPHRG */

