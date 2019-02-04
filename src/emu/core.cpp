/*
 * devcore.cpp
 *
 *  Created on: Mar 3, 2017
 *      Author: fswor
 */

#include "emu/core.h"
#include "emu/debug.h"
#include "emu/devsys.h"
#include "emu/devcore.h"

//extern Driver mseDriver;

Core::Core()
: Device(CLS_ROOT)
{
//	setSystemDrivers(this);
//	setDriver(&mseDriver);

	// Set device name and description
//	devName = mseDriver.drvName;
//	devType = mseDriver.drvName;
//	devDesc = mseDriver.drvDesc;
}

Core::~Core()
{
}

// Cleaning system up
void Core::release()
{

}
