/*
 * comm_protocol.h
 *
 *  Copyright 2017 Vladimir Nikolić
 */

#ifndef INFRASTRUCTURE_PERIPHERALS_COMM_PROTOCOL_H_
#define INFRASTRUCTURE_PERIPHERALS_COMM_PROTOCOL_H_

#include "hardware/serial_comm.h"

// Unique preambles to identify message type
const int16_t MICROPHONE_REQUEST = 0;
const int16_t MOTORS_COMMAND = 1;
const int16_t DISTANCE_REQUEST = 2;
const int16_t PLAY_SOUND = 3;
const int16_t ACCELEROMETER_REQUEST = 4;
const int16_t POWER_STATUS_REQUEST = 5;
const int16_t MAGNETOMETER_REQUEST = 6;
const int16_t MAGNETOMETER_CAL_REQUEST = 7;

// Protocol wrappers
inline int16_t receive_preamble()
{
    return serial_comm_receive();
}

inline void send_microphone_turn_angle(const int16_t turn_angle)
{
    serial_comm_send(turn_angle);
}

inline void request_microphone_turn_angle(int16_t* turn_angle)
{
    serial_comm_send(MICROPHONE_REQUEST);
    *turn_angle = serial_comm_receive();
}

inline void send_microphone_data(const int16_t intensity1, const int16_t intensity2, const int16_t intensity3)
{
    serial_comm_send(intensity1);
    serial_comm_send(intensity2);
    serial_comm_send(intensity3);
}

inline void request_microphone_data(int16_t* intensity1, int16_t* intensity2, int16_t* intensity3)
{
    serial_comm_send(MICROPHONE_REQUEST);
    *intensity1 = serial_comm_receive();
    *intensity2 = serial_comm_receive();
    *intensity3 = serial_comm_receive();
}


inline void issue_motors_command(const int16_t power_left, const int16_t power_right)
{
    serial_comm_send(MOTORS_COMMAND);
    serial_comm_send(power_left);
    serial_comm_send(power_right);
}

inline void receive_motors_command(int16_t* power_left, int16_t* power_right)
{
    *power_left = serial_comm_receive();
    *power_right = serial_comm_receive();
}

inline void send_distance(const int16_t distance)
{
    serial_comm_send(distance);
}

inline void request_distance(int16_t* distance)
{
    serial_comm_send(DISTANCE_REQUEST);
    *distance = serial_comm_receive();
}

inline void send_sound_data(const int16_t frequency, const int16_t duration){
  serial_comm_send(PLAY_SOUND);
  serial_comm_send(frequency);
  serial_comm_send(duration);
}

inline void receive_sound_data(int16_t* frequency, int16_t* duration){
  *frequency = serial_comm_receive();
  *duration = serial_comm_receive();
}

inline void request_magnetometer_data(int16_t *mx, int16_t *my, int16_t *mz)
{
    serial_comm_send(MAGNETOMETER_REQUEST);
    *mx = serial_comm_receive();
    *my = serial_comm_receive();
    *mz = serial_comm_receive();
}

inline void send_magnetometer_data(const int16_t mx, const int16_t my, const int16_t mz)
{
    serial_comm_send(mx);
    serial_comm_send(my);
    serial_comm_send(mz);
}

inline void request_magnetometer_min_max(int16_t *int_max_x, int16_t *int_min_x, int16_t *int_max_y, int16_t *int_min_y)
{
    serial_comm_send(MAGNETOMETER_CAL_REQUEST);
    *int_max_x = serial_comm_receive();
    *int_min_x = serial_comm_receive();
    *int_max_y = serial_comm_receive();   
    *int_min_y = serial_comm_receive();    
}

inline void send_magnetometer_min_max(const int16_t int_max_x, const int16_t int_min_x, const int16_t int_max_y, const int16_t int_min_y)
{
    serial_comm_send(int_max_x);
    serial_comm_send(int_min_x);
    serial_comm_send(int_max_y);
    serial_comm_send(int_min_y);
}



inline void request_accelerometer_data(int16_t *ax, int16_t *ay, int16_t *az)
{
    serial_comm_send(ACCELEROMETER_REQUEST);
    *ax = serial_comm_receive();
    *ay = serial_comm_receive();
    *az = serial_comm_receive();
}

inline void send_accelerometer_data(const int16_t ax, const int16_t ay, const int16_t az)
{
    serial_comm_send(ax);
    serial_comm_send(ay);
    serial_comm_send(az);
}

inline void request_power_status(int16_t* status)
{
    serial_comm_send(POWER_STATUS_REQUEST);
    *status = serial_comm_receive();
}

inline void send_power_status(const int16_t status)
{
    serial_comm_send(status);
}

#endif /* INFRASTRUCTURE_PERIPHERALS_COMM_PROTOCOL_H_ */
