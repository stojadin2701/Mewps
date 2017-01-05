/*
 * Motors.h
 *
 *  Created on: Jan 5, 2017
 *      Author: schutzekatze
 */

#ifndef MOTORS_H_
#define MOTORS_H_

class Motors
{

public:

	/*
	 * Set and get powers given to the robot motors.
	 * The values range from -255 to 255, with -255 meaning
	 * going reverse at max speed, and 255 going forward at max speed.
	 */
	static void set_powers(short power_left, short power_right);
	static void get_powers(short *power_left, short *power_right);

private:

	Motors() = delete;

	static short power_left;
	static short power_right;

};

#endif /* MOTORS_H_ */
