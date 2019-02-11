/*
 * systems.cpp
 *
 *  Created on: Jan 11, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/driver.h"
#include "emu/syslist.h"

#include "sys/vax/extern.h"

const system_driver *system_list::syslist[] =
{

	// Digital Electric Corporation (DEC)
	&SYSTEM_NAME(mv3900),
	&SYSTEM_NAME(mv3900x),

	// null terminator
	nullptr
};
