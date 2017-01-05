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

	static void set_powers(short power1, short power2);
	static void get_powers(short *power1, short *power2);

private:

	Motors() = delete;

	static short power1;
	static short power2;

};

#endif /* MOTORS_H_ */
