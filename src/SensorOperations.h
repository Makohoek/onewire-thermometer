#ifndef __INC__SENSOROPERATIONS__
#define __INC__SENSOROPERATIONS__

#include <linux/slab.h>

#include "DiscoveryProtocol.h"
#include "GlobalData.h"
#include "OneWire.h"
#include "SensorID.h"
#include "bitOperations.h"
#include "Klogger.h"
#include "led.h"

#include "TemperatureScratchpad.h"
#include "TemperatureResolution.h"
#include "LinkedList.h"

/* read temperature from the sensor */
int sensorRequestTemperature(Sensor sensor);
void sensorSetNewResolution(Sensor sensor);
unsigned int discoverEachSensorID(LinkedList* sensorsList);
void initializeOneWire(int gpioPin);

#endif
