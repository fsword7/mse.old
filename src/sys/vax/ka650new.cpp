/*
 * ka650new.cpp - KA650 system device
 *
 *  Created on: Jan 9, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/devsys2.h"
#include "emu/driver.h"
#include "sys/vax/ka650new.h"

void ka650_device::mv3900_init()
{

}


COMP(mv3900, nullptr, ka650_device, mv3900_init, "MicroVAX 3900")
