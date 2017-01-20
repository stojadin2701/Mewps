/*
 * Motors.cpp
 *
 *  Copyright 2017 Vladimir Nikolić
 */

#include "Motors.h"

#include "comm_protocol_threadsafe.h"

int16_t Motors::power_left = 0;
int16_t Motors::power_right = 0;

void Motors::set_powers(int16_t power_left, int16_t power_right)
{
    issue_motors_command_ts(power_left, power_right);
}

void Motors::get_powers(int16_t *power_left, int16_t *power_right)
{
    *power_left = Motors::power_left;
    *power_right = Motors::power_right;
}
