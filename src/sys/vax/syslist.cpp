/*
 * syslist.cpp - system model list for VAX systems
 *
 *  Created on: Jan 11, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/devsys2.h"
#include "emu/driver.h"
#include "sys/vax/ka650new.h"

system_driver vax_syslist[] = {

	// MicroVAX III series
	COMP(mv3900,  nullptr, ka650_device, mv3900_init,  "MicroVAX 3900"),
	COMP(mv3900x, nullptr, ka650_device, mv3900x_init, "MicroVAX 3900x"),

	// null terminator
	{ nullptr }
};
