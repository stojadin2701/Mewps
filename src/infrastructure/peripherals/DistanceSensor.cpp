/*
 * DistanceSensor.cpp
 *
 *  Created on: Jan 24, 2017
 *      Author: schutzekatze
 */

#include "../../infrastructure/peripherals/DistanceSensor.h"

#include "../../infrastructure/peripherals/comm_protocol_threadsafe.h"

namespace infrastructure
{

float DistanceSensor::get_distance()
{
	int16_t millimeters;

	request_distance_ts(&millimeters);

	return millimeters / 1000.0;
}

}  /* namespace infrastructure */
