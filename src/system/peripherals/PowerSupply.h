/*
 * PowerSupply.h
 *
 *  Created on: Jan 24, 2017
 *      Author: schutzekatze
 */

#ifndef SYSTEM_PERIPHERALS_POWERSUPPLY_H_
#define SYSTEM_PERIPHERALS_POWERSUPPLY_H_

class PowerSupply
{

public:

	// Returns a value from 0.0 to 1.0 (0% - 100%) based on how much power is left
	float get_supply_status();

	// Returns estimated seconds left until the power supply is exhausted
	int get_estimated_remaining_time();

};

#endif /* SYSTEM_PERIPHERALS_POWERSUPPLY_H_ */
