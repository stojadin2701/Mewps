/*
 * initialization.cpp
 *
 *  Copyright 2017 Vladimir Nikolić
 */

#include "initialization.h"

#include "hardware/serial_comm.h"

void initialize()
{
    serial_comm_initialize();
}

void finalize()
{
    serial_comm_finalize();
}
