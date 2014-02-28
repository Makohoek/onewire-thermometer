#ifndef __INC_SENSORID__
#define __INC_SENSORID__

#include <linux/kernel.h>

#include "OneWire.h"

void writeSensorID(SensorID sensorID);
void printSensorID(SensorID sensorID);
void affectSensorID(SensorID destination, SensorID source);

#endif /* end of include guard: __INC_SENSORID__ */
