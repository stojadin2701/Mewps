/*
 * Motors.cpp
 *
 *  Copyright 2017 Vladimir Nikolić
 */

#include "Motors.h"

#include "comm_protocol_threadsafe.h"

float Motors::power_left = 0;
float Motors::power_right = 0;

void Motors::set_powers(float power_left, float power_right)
{
	int16_t left = power_left * 255;
	int16_t right = power_right * 255;

    issue_motors_command_ts(left, right);
}

void Motors::get_powers(float *power_left, float *power_right)
{
    *power_left = Motors::power_left;
    *power_right = Motors::power_right;
}
