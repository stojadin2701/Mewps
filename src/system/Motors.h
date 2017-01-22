/*
 * Motors.h
 *
 *  Copyright 2017 Vladimir Nikolić
 */

#ifndef SYSTEM_MOTORS_H_
#define SYSTEM_MOTORS_H_

#include <cstdint>

class Motors
{

public:

    /*
     * Set and get powers given to the robot motors.
     * The values range from -255 to 255, with -255 meaning
     * going reverse at max speed, and 255 going forward at max speed.
     */
    static void set_powers(int16_t power_left, int16_t power_right);
    static void get_powers(int16_t *power_left, int16_t *power_right);

private:

    Motors() = delete;

    static int16_t power_left;
    static int16_t power_right;

};

#endif /* SYSTEM_MOTORS_H_ */
