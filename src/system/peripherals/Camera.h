/*
 * Camera.h
 *
 *  Copyright 2017 Vladimir Nikolić
 */

#ifndef SYSTEM_PERIPHERALS_CAMERA_H_
#define SYSTEM_PERIPHERALS_CAMERA_H_

class Camera
{

public:

    // Multiple readers/writers problem
    static const unsigned char* get_frame();

};

#endif /* SYSTEM_PERIPHERALS_CAMERA_H_ */
