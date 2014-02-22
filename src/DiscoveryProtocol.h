#ifndef __INC_DISCOVERYPROTOCOL__
#define __INC_DISCOVERYPROTOCOL__

#include "OneWire.h"
#include "dmesgLogging.h"
#include "GlobalData.h"

void performDiscovery( SensorID discoveredSensorID );
void printSensorID( SensorID sensorID );

#endif /* end of include guard: __INC_DISCOVERYPROTOCOL__ */
