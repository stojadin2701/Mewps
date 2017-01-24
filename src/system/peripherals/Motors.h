/*
 * Motors.h
 *
 *  Copyright 2017 Vladimir Nikolić
 */

#ifndef SYSTEM_PERIPHERALS_MOTORS_H_
#define SYSTEM_PERIPHERALS_MOTORS_H_

#include <cstdint>

class Motors
{

public:

    /*
     * Set and get powers given to the robot motors.
     * The values range from -1.0 to 1.0, with -1.0 meaning
     * going reverse at max speed, and 1.0 going forward at max speed.
     */
    static void set_powers(float power_left, float power_right);
    static void get_powers(float *power_left, float *power_right);

private:

    Motors() = delete;

    static float power_left;
    static float power_right;

};

#endif /* SYSTEM_PERIPHERALS_MOTORS_H_ */
