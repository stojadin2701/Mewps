/*
 * Motor.h
 *
 *  Created on: Oct 4, 2016
 *      Author: schutzekatze
 */

#ifndef MOTOR_H_
#define MOTOR_H_

class Motor
{

public:

	static Motor LEFT_MOTOR, RIGHT_MOTOR;

	//Sets the power given to the motor, from -255 to 255 (0 - 100%, negative means reverse)
	void set_power(short power);
	short get_power() const { return power; }

private:

	Motor(char id): id(id) {}

	char id;
	short power = 0;

};

#endif /* MOTOR_H_ */
