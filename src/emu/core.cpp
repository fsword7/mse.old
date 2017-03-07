/*
 * devcore.cpp
 *
 *  Created on: Mar 3, 2017
 *      Author: fswor
 */

#include "emu/core.h"
#include "emu/devsys.h"
#include "emu/devcore.h"

extern Driver mseDriver;

Core::Core()
{
	setSystemDrivers(this);
	setDriver(&mseDriver);
}

Core::~Core()
{
}

