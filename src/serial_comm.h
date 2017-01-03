/*
 * serial_comm.h
 *
 *  Created on: Dec 29, 2016
 *      Author: schutzekatze
 */

#ifndef SERIAL_COMM_H_
#define SERIAL_COMM_H_

/*
 * The communication protocol between the main computer and the microcontroller.
 *
 * The mode used is 8N1, 8 bit character size, no parity and 1 stop bit.
 * All messages are fixed size of 16 bits.
 * Both sides of the communication should include this header, and implement
 * the initialize, finalize, send and receive functions specific to their platform.
 */

const int BAUD_RATE = 9600;

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// These functions require platform specific implementations to use serial communication.
// The messages should be sent in network byte order, that is, big endian.
void serial_comm_initialize();
void serial_comm_finalize();
void serial_comm_send(const int16_t msg);
int16_t serial_comm_receive();

#ifdef __cplusplus
}
#endif

// Unique preambles to identify message type
const int16_t MICROPHONE_REQUEST = 0;
const int16_t MOTOR_COMMAND = 1;
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

inline void issue_motor_command(const int16_t motor, const int16_t power)
{
	serial_comm_send(MOTOR_COMMAND);
	serial_comm_send(motor);
	serial_comm_send(power);
}

inline void receive_motor_command(int16_t* motor, int16_t* power)
{
	*motor = serial_comm_receive();
	*power = serial_comm_receive();
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

#endif /* SERIAL_COMM_H_ */
