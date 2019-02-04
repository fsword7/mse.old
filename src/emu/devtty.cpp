/*
 * devtty.cpp - terminal device handler for Telnet and COM access
 *
 *  Created on: May 7, 2018
 *      Author: Timothy Stark
 */

#include "emu/core.h"
#include "emu/debug.h"
#include "emu/devsys-old.h"
#include "emu/devcpu.h"
#include "emu/devcore.h"
#include "emu/console.h"

ttyDevice::ttyDevice()
: ioDevice(CLS_TTYDEV)
{
}

ttyDevice::~ttyDevice()
{
	// Close all log files
//	log.close(-1);
}

