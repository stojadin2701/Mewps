/*
 * serial_comm.c
 *
 *  Created on: Dec 29, 2016
 *      Author: schutzekatze
 */

#include "serial_comm.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <arpa/inet.h>

const char* tty_path = "/dev/ttyACM0";
int tty;

int baud2constant(int baud)
{
	switch(baud)
	{
	case 4800: return B4800;
	case 9600: return B9600;
	case 19200: return B19200;
	case 38400: return B38400;
	case 57600: return B57600;
	}
	return -1;
}

void serial_comm_initialize()
{
	tty = open(tty_path, O_RDWR | O_NOCTTY);
	if (tty == -1)
	{
		perror("Unable to open tty");
		return;
	}
	else
	{
		fcntl(tty, F_SETFL, 0);
	}

	struct termios options;
	tcgetattr(tty, &options);

	// Set baud rate
	cfsetispeed(&options, baud2constant(BAUD_RATE));
	cfsetospeed(&options, baud2constant(BAUD_RATE));

	// Necessary flags
	options.c_cflag |= (CLOCAL | CREAD);

	// Set character size and disable parity
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	// Enable the following line if parity is turned on
	// options.c_iflag |= (INPCK | ISTRIP); //Enable parity check and strip

	// Disable hardware and software flow control respectively
	//options.c_cflag &= ~RTSCTS;
	options.c_iflag &= ~(IXON | IXOFF | IXANY);

	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Enable raw input

	options.c_oflag &= ~OPOST; // Enable raw output

	options.c_cc[VMIN] = 2;
	options.c_cc[VTIME] = 0;

	tcsetattr(tty, TCSANOW, &options);

	usleep(3000*1000);
}

void serial_comm_finalize()
{
	close(tty);
}

void serial_comm_send(const int16_t msg)
{
	int16_t network_msg = htons(msg);
	write(tty, &network_msg, sizeof(network_msg));
}

int16_t serial_comm_receive()
{
	int16_t msg;
	read(tty, &msg, sizeof(msg));
	msg = ntohs(msg);
	return msg;
}
