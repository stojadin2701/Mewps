/*
 * Motors.cpp
 *
 *  Created on: Jan 5, 2017
 *      Author: schutzekatze
 */

#include "Motors.h"

#include "comm_protocol_threadsafe.h"

short Motors::power1 = 0;
short Motors::power2 = 0;

void Motors::set_powers(short power1, short power2)
{
	issue_motors_command_ts(power1, power2);
}

void Motors::get_powers(short *power1, short *power2)
{
	*power1 = Motors::power1;
	*power2 = Motors::power2;
}
