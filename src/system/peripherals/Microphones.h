/*
 * Microphones.h
 *
 *  Created on: Jan 24, 2017
 *      Author: schutzekatze
 */

#ifndef SYSTEM_PERIPHERALS_MICROPHONES_H_
#define SYSTEM_PERIPHERALS_MICROPHONES_H_

class Microphones
{

public:

	// Returns registered intensities for each microphone
	void get_intensities(float *intensity1, float *intensity2, float *intensity3);

};

#endif /* SYSTEM_PERIPHERALS_MICROPHONES_H_ */
