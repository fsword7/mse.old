/*
 * devcore.cpp
 *
 *  Created on: Mar 3, 2017
 *      Author: fswor
 */

#include "emu/core.h"
#include "emu/devsys.h"
#include "emu/devcore.h"

Core::Core()
{
	setSystemDrivers(this);
}

Core::~Core()
{
}

