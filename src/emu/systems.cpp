/*
 * systems.cpp
 *
 *  Created on: Jan 11, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/driver.h"
#include "emu/syslist.h"

extern system_driver vax_syslist;

const system_driver *system_list::syslist[] =
{
	vax_syslist,

	// null terminator
	{ nullptr }
};
