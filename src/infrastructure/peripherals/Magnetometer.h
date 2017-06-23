/*
 * Magnetometer.h
 *
 *
 */

#ifndef INFRASTRUCTURE_PERIPHERALS_MAGNETOMETER_H_
#define INFRASTRUCTURE_PERIPHERALS_MAGNETOMETER_H_

namespace infrastructure
{

class Magnetometer
{

public:

    static void get_magnetic_field(float* mx, float *my, float *mz);

private:

    static constexpr float CONVERSION_FACTOR = 0.1;

};

}  /* namespace infrastructure */

#endif /* INFRASTRUCTURE_PERIPHERALS_MAGNETOMETER_H_ */
