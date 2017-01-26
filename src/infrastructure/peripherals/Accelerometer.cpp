/*
 * Accelerometer.cpp
 *
 *  Copyright 2017 Vladimir Nikolić
 */

#include "../../infrastructure/peripherals/Accelerometer.h"

#include "../../infrastructure/peripherals/comm_protocol_threadsafe.h"

namespace infrastructure
{

void Accelerometer::get_accelerations(float* ax, float *ay, float *az)
{
	int16_t int_ax, int_ay, int_az;

    request_accelerometer_data_ts(&int_ax, &int_ay, &int_az);

    *ax = CONVERSION_FACTOR * int_ax;
    *ay = CONVERSION_FACTOR * int_ay;
    *az = CONVERSION_FACTOR * int_az;
}

}  /* namespace infrastructure */
