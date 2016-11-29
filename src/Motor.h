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

	//Sets the power given to the motor, from -1.0 to 1.0 (0% - 100%, negative means reverse)
	void set_power(float power);

};

#endif /* MOTOR_H_ */
