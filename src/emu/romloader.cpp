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

const romEntry_t *rom_loader::first(device_t &device)
{
	const romEntry_t *entry = device.romGetRegions();

	return !ROMENTRY_ISEND(*entry) ? entry : nullptr;
}

const romEntry_t *rom_loader::next(const romEntry_t *entry)
{
	entry++;

	return !ROMENTRY_ISEND(*entry) ? entry : nullptr;
}

void rom_loader::processRegionList()
{
	uint32_t			rgnLength;
	tag_t				*rgnTagName;
	const romEntry_t	*entry;

	device_iterator iters(*sysMachine->systemDevice());
	for (device_t &device : iters) {
		for (entry = first(device); entry != nullptr; entry = next(entry)) {
			rgnLength  = ROMREGION_GETLENGTH(*entry);
			rgnTagName = ROMREGION_GETNAME(*entry);

			cty.printf("%s: Processing ROM region '%s' length = %d\n",
				device.deviceName(), rgnTagName, rgnLength);

			// Entry must be region type
//			assert(ROMENTRY_ISREGION(*entry));

			if (ROMREGION_ISROMDATA(*entry)) {
				uint8_t  width  = ROMREGION_GETWIDTH(*entry) / 8;
				endian_t endian = ROMREGION_ISBIGENDIAN(*entry) ? endianBig : endianLittle;

			}
		}
	}
}
