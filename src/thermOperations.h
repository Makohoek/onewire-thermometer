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
#include "dmesgLogging.h"
#include "GlobalData.h"

float readTemperature( SensorID sensorID );
void writeFunctionCommand(FunctionCommand command);
void writeROMCommand(ROMCommand romcommand);
void waitForConversionDone(void);
void writeSensorID(SensorID sensorID);

#endif /* end of include guard: __INC_THERMOPERATIONS__ */
