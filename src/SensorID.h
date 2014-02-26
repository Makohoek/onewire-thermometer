#ifndef __INC_SENSORID__
#define __INC_SENSORID__

#include <linux/kernel.h>

#include "dmesgLogging.h"
#include "GlobalData.h"
#include "OneWire.h"

typedef Bit SensorID[64];

void writeSensorID(SensorID sensorID);
void printSensorID(SensorID sensorID);
void affectSensorID(SensorID destination, SensorID source);

#endif /* end of include guard: __INC_SENSORID__ */
