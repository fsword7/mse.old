/*
 * romloader.cpp
 *
 *  Created on: Feb 22, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/machine.h"
#include "emu/romloader.h"


rom_loader::rom_loader(machine *sys)
: sysMachine(sys)
{

}

rom_loader::~rom_loader()
{
}

const romEntry *rom_loader::first(device_t &device)
{
	const romEntry *entry = &device.romGetRegions().front();

	return !ROMENTRY_ISEND(*entry) ? entry : nullptr;
}

const romEntry *rom_loader::next(const romEntry *entry)
{
	entry++;

	return !ROMENTRY_ISEND(*entry) ? entry : nullptr;
}

void rom_loader::processRegionList()
{

	device_iterator iters(*sysMachine->systemDevice());
	for (device_t &device : iters) {
		for (const romEntry *entry = first(device); entry != nullptr; entry = next(entry)) {

		}
	}
}
