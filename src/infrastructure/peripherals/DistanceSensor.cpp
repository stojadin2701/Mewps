#include <stdint.h>
#include "DistanceSensor.h"

#include "CommProtocolThreadsafe.h"

namespace infrastructure
{

int16_t DistanceSensor::get_distance()
{
	int16_t millimeters;

	CommProtocolThreadsafe::request_distance_ts(&millimeters);

	return millimeters;
}

}  /* namespace infrastructure */
