/*
 * comm_protocol.h
 *
 *  Created on: Jan 4, 2017
 *      Author: schutzekatze
 */

#ifndef COMM_PROTOCOL_H_
#define COMM_PROTOCOL_H_

#include "serial_comm.h"

// Unique preambles to identify message type
const int16_t MICROPHONE_REQUEST = 0;
const int16_t MOTORS_COMMAND = 1;
const int16_t DISTANCE_REQUEST = 2;

// Protocol wrappers
inline int16_t receive_preamble()
{
	return serial_comm_receive();
}

inline void request_microphone_data(int16_t* intensity1, int16_t* intensity2, int16_t* intensity3)
{
	serial_comm_send(MICROPHONE_REQUEST);
	*intensity1 = serial_comm_receive();
	*intensity2 = serial_comm_receive();
	*intensity3 = serial_comm_receive();
}

inline void send_microphone_data(const int16_t intensity1, const int16_t intensity2, const int16_t intensity3)
{
	serial_comm_send(intensity1);
	serial_comm_send(intensity2);
	serial_comm_send(intensity3);
}

inline void issue_motors_command(const int16_t power1, const int16_t power2)
{
	serial_comm_send(MOTORS_COMMAND);
	serial_comm_send(power1);
	serial_comm_send(power2);
}

inline void receive_motors_command(int16_t* power1, int16_t* power2)
{
	*power1 = serial_comm_receive();
	*power2 = serial_comm_receive();
}

inline void request_distance(int16_t* distance)
{
	serial_comm_send(DISTANCE_REQUEST);
	*distance = serial_comm_receive();
}

inline void send_distance(const int16_t distance)
{
	serial_comm_send(distance);
}

#endif /* COMM_PROTOCOL_H_ */
