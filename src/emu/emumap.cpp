/*
 * emumap.cpp
 *
 *  Created on: Jan 8, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "addrmap.h"


mapManager::mapManager(device_t *dev)
{
}

mapManager::~mapManager()
{
}

mapAddressConfig::mapAddressConfig()
: name(nullptr), dataWidth(0), addrWidth(0),
  addrShift(0), pageShift(0)
{

}
