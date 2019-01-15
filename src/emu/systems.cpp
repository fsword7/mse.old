/*
 * systems.cpp
 *
 *  Created on: Jan 11, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/driver.h"
#include "emu/syslist.h"

//extern const system_driver vax_syslist;

SYSTEM_EXTERN(mv3900);
SYSTEM_EXTERN(mv3900x);

const system_driver *system_list::syslist[] =
{
//	&vax_syslist,

	&driver_mv3900,
	&driver_mv3900x,

	// null terminator
	nullptr
};
