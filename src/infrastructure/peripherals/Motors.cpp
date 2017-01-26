/*
 * Motors.cpp
 *
 *  Copyright 2017 Vladimir Nikolić
 */

#include "../../infrastructure/peripherals/Motors.h"

#include "../../infrastructure/peripherals/comm_protocol_threadsafe.h"

namespace infrastructure
{

float Motors::power_left = 0;
float Motors::power_right = 0;

void Motors::set_powers(float power_left, float power_right)
{
	int16_t left = power_left * CONVERSION_FACTOR;
	int16_t right = power_right * CONVERSION_FACTOR;

    issue_motors_command_ts(left, right);
}

void Motors::get_powers(float *power_left, float *power_right)
{
    *power_left = Motors::power_left;
    *power_right = Motors::power_right;
}

}  /* namespace infrastructure */
