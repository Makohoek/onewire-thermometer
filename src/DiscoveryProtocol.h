#ifndef __INC_DISCOVERYPROTOCOL__
#define __INC_DISCOVERYPROTOCOL__

#include "GlobalData.h"
#include "OneWire.h"
#include "SensorID.h"
#include "Klogger.h"

int performDiscovery(SensorID discoveredSensorID);
Bool isEverySensorDiscovered(void);

#endif /* end of include guard: __INC_DISCOVERYPROTOCOL__ */
