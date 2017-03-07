/*
 * devcore.cpp
 *
 *  Created on: Mar 3, 2017
 *      Author: fswor
 */

#include "emu/core.h"
#include "emu/devsys.h"
#include "emu/devcore.h"

appCore::appCore()
{
	devList.clear();
}

appCore::~appCore()
{
}

void appCore::findDevice(std::string devName)
{
	for (auto &&dev : devList) {

	}
}
