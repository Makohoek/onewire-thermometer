#ifndef __INC__SENSOROPERATIONS__
#define __INC__SENSOROPERATIONS__

#include <linux/slab.h>

#include "DiscoveryProtocol.h"
#include "GlobalData.h"
#include "OneWire.h"
#include "SensorID.h"
#include "bitOperations.h"
#include "dmesgLogging.h"
#include "led.h"
#include "thermOperations.h"
#include "LinkedList.h"

/* led blinking for fun-only part */
void blinkGpioLed(void);
/* read temperature from the sensor */
int sensorRequestTemperature(Sensor sensor);
void setNewResolution(Sensor sensor);
//unsigned int discoverEachSensorID(LinkedList* sensorsList);
void initializeOneWire(int gpioPin);

#endif
