#ifndef __INC_DISCOVERYPROTOCOL__
#define __INC_DISCOVERYPROTOCOL__

#include "OneWire.h"
#include "SensorID.h"

int performDiscovery(SensorID discoveredSensorID);
Bool isEverySensorDiscovered(void);

#endif /* end of include guard: __INC_DISCOVERYPROTOCOL__ */
