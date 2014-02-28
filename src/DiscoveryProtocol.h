/**
 * DiscoveryProtocol
 *
 * Implements the sensor id discovery process
 * which is called after a SEARCH_ROM command
 *
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 * Alexandre Montilla
 * <alexandre.montilla@gmail.com>
 */
#ifndef __INC_DISCOVERYPROTOCOL__
#define __INC_DISCOVERYPROTOCOL__

#include "SensorID.h"

int performDiscovery(SensorID discoveredSensorID);
Bool isEverySensorDiscovered(void);

#endif /* end of include guard: __INC_DISCOVERYPROTOCOL__ */
