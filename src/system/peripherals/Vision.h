/*
 * Vision.h
 *
 *  Copyright 2017 Vladimir Nikolić
 */

#ifndef SYSTEM_PERIPHERALS_VISION_H_
#define SYSTEM_PERIPHERALS_VISION_H_

class Vision
{

public:

    // Multiple readers/writers problem
    static const unsigned char* get_frame();

};

#endif /* SYSTEM_PERIPHERALS_VISION_H_ */
