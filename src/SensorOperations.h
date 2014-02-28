/**
 * SensorID
 *
 * Implements high level functions related to
 * the functionalities expected by the driver
 * (read temperature and setresolution)
 *
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 * Alexandre Montilla
 * <alexandre.montilla@gmail.com>
 */
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
