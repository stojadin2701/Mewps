/*
 * serial_comm.h
 *
 *  Copyright 2017 Vladimir Nikolić
 */

#ifndef SYSTEM_HARDWARE_SERIAL_COMM_H_
#define SYSTEM_HARDWARE_SERIAL_COMM_H_

/*
 * This header declares routines for serial communication between
 * the main computer and the microcontroller.
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

#endif /* SYSTEM_HARDWARE_SERIAL_COMM_H_ */
