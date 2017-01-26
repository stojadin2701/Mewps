/*
 * PowerSupply.cpp
 *
 *  Copyright 2017 Vladimir Nikolić
 */

#include "../../infrastructure/peripherals/PowerSupply.h"

#include "../../infrastructure/peripherals/comm_protocol_threadsafe.h"

namespace infrastructure
{

float PowerSupply::get_supply_status()
{
	int16_t int_status;

	request_power_status_ts(&int_status);

	return (int_status * CONVERSION_FACTOR - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE);
}

int PowerSupply::get_estimated_remaining_time()
{
	return 0;
}

}  /* namespace infrastructure */
