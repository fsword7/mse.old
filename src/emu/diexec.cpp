/*
 * diexec.cpp
 *
 *  Created on: Feb 4, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/diexec.h"

di_execute::di_execute(device_t *dev)
: device_interface(dev, "execute")
{
}

di_execute::~di_execute()
{
}
