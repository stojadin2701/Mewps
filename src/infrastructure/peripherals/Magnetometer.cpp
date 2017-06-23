/*
 * Magnetometer.cpp
 *
 *
 */

#include "Magnetometer.h"

#include "CommProtocolThreadsafe.h"

namespace infrastructure
{

void Magnetometer::get_magnetic_field(float* mx, float* my, float* mz)
{
	int16_t int_mx, int_my, int_mz;

    CommProtocolThreadsafe::request_magnetometer_data_ts(&int_mx, &int_my, &int_mz);

    *mx = CONVERSION_FACTOR*int_mx; //uT
    *my = CONVERSION_FACTOR*int_my; //uT
    *mz = CONVERSION_FACTOR*int_mz; //uT
}

}  /* namespace infrastructure */
