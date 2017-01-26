/*
 * initialization.cpp
 *
 *  Copyright 2017 Vladimir Nikolić
 */

#include "../infrastructure/initialization.h"

#include "../infrastructure/peripherals/hardware/serial_comm.h"

namespace infrastructure
{

void initialize()
{
    serial_comm_initialize();
}

void finalize()
{
    serial_comm_finalize();
}

}  /* namespace infrastructure */
