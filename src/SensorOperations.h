#ifndef __INC__SENSOROPERATIONS__
#define __INC__SENSOROPERATIONS__

#include <linux/slab.h>

#include "Led.h"
#include "DiscoveryProtocol.h"
#include "LinkedList.h"
#include "TemperatureResolution.h"
#include "TemperatureScratchpad.h"

/* read temperature from the sensor */
int sensorRequestTemperature(Sensor sensor);
void sensorSetNewResolution(Sensor sensor);
unsigned int discoverEachSensorID(LinkedList* sensorsList);

#endif
