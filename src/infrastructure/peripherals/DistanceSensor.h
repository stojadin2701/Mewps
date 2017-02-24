/*
 * DistanceSensor.h
 *
 *  Created on: Jan 24, 2017
 *      Author: schutzekatze
 */

#ifndef INFRASTRUCTURE_PERIPHERALS_DISTANCESENSOR_H_
#define INFRASTRUCTURE_PERIPHERALS_DISTANCESENSOR_H_

#include <stdint.h>

namespace infrastructure
{

class DistanceSensor
{

public:

	// Returns distance registered in centimeters
	static int16_t get_distance();

private:

   // static constexpr float CONVERSION_FACTOR = 0.001;

};

}  /* namespace infrastructure */

#endif /* INFRASTRUCTURE_PERIPHERALS_DISTANCESENSOR_H_ */
