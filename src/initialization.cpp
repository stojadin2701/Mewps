/*
 * initialization.cpp
 *
 *  Created on: Jan 1, 2017
 *      Author: schutzekatze
 */

#include "initialization.h"

#include "serial_comm.h"

void initialize()
{
	serial_comm_initialize();
}

void finalize()
{
	serial_comm_finalize();
}
