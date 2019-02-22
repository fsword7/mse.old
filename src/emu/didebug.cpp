/*
 * didebug.cpp
 *
 *  Created on: Feb 21, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/didebug.h"

di_debug::di_debug(device_t *dev)
: device_interface(dev, "debug")
{
	dev->interfaces().debug = this;
}

di_debug::~di_debug()
{
}
