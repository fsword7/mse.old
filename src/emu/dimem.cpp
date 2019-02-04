/*
 * dimem.cpp
 *
 *  Created on: Feb 4, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/dimem.h"

di_memory::di_memory(device_t *dev)
: device_interface(dev, "memory")
{
}

di_memory::~di_memory()
{
}
