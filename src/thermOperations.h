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

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>

#include "OneWire.h"
#include "SensorID.h"
#include "dmesgLogging.h"
#include "GlobalData.h"

// max time to convert temperature from the sensor.
// refer to Table-2 from documentation
#define TCONV 750

#define MAX_CHARS_TEMPERATURE 14 // max precision = 12, one char for '.' and one for '\0'
typedef char TemperatureString[MAX_CHARS_TEMPERATURE];
typedef u8 Scratchpad[9];
typedef enum
{
  MAXIMUM=12,
  HIGH=11,
  LOW=10,
  MINIMUM=9
} TemperatureResolution;

void readScratchpad(Scratchpad readedScratchpad);
void writeScratchpad(Scratchpad scratchpad);
void writeFunctionCommand(FunctionCommand command);
void writeROMCommand(ROMCommand romcommand);
void waitForConversionDone(void);
void writeSensorID(SensorID sensorID);
long extractTemperatureFromScratchpad(Scratchpad scratchpadData, TemperatureResolution resolution);
int temperatureToString(TemperatureString result, long temperature);
void buildScratchpadNewResolution(Scratchpad scratchpad, TemperatureResolution resolution);

#endif /* end of include guard: __INC_THERMOPERATIONS__ */
