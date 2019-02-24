/*
 * romloader.cpp
 *
 *  Created on: Feb 22, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/console.h"
#include "emu/machine.h"
#include "emu/romloader.h"


rom_loader::rom_loader(machine *sys, const cty_t &cty)
: sysMachine(sys), cty(cty)
{
	processRegionList();
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
	uint32_t	rgnLength;
	tag_t		*rgnTagName;

	device_iterator iters(*sysMachine->systemDevice());
	for (device_t &device : iters) {
		for (const romEntry *entry = first(device); entry != nullptr; entry = next(entry)) {
			rgnLength  = ROMREGION_GETLENGTH(*entry);
			rgnTagName = ROMREGION_GETNAME(*entry);

			cty.printf("%s: Processing ROM region '%s' length = %d\n",
				device.deviceName(), rgnTagName, rgnLength);

//			assert(ROMENTRY_ISREGION(*entry));

		}
	}
}
