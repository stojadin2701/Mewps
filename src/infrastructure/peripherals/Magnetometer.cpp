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

void Magnetometer::get_magnetic_field_info(float* offset_x, float* offset_y, float* scale_x, float* scale_y)
{
	int16_t int_max_x, int_min_x, int_max_y, int_min_y;

	CommProtocolThreadsafe::request_magnetometer_min_max_ts(&int_max_x, &int_min_x, &int_max_y, &int_min_y);

	*offset_x = ((float)(int_min_x + int_max_x))/2.0;
	*offset_y = ((float)(int_min_y + int_max_y))/2;

	*scale_x = 1.0/(float)(int_max_x - int_min_x);
	*scale_y = 1.0/(float)(int_max_y - int_min_y);

}

}  /* namespace infrastructure */
