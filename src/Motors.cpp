/*
 * Motors.cpp
 *
 *  Created on: Jan 5, 2017
 *      Author: schutzekatze
 */

#include "Motors.h"

#include "comm_protocol_threadsafe.h"

short Motors::power_left = 0;
short Motors::power_right = 0;

void Motors::set_powers(short power_left, short power_right)
{
	issue_motors_command_ts(power_left, power_right);
}

void Motors::get_powers(short *power_left, short *power_right)
{
	*power_left = Motors::power_left;
	*power_right = Motors::power_right;
}
