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
	while (!ROMENTRY_ISREGIONEND(*entry))
		entry++;

	return !ROMENTRY_ISEND(*entry) ? entry : nullptr;
}

void rom_loader::fillImage(const romEntry_t *entry)
{
	uint32_t	length = ROM_GETLENGTH(*entry);
	uint32_t	skip = ROM_GETSKIP(*entry);
	uint8_t		fillValue = ROM_GETVALUE(*entry);
	uint8_t 	*base = region->base() + ROM_GETOFFSET(*entry);

	if (skip > 0) {
		for (int idx = 0; idx < length; idx += skip+1)
			base[idx] = fillValue;
	} else
		memset(base, fillValue, length);
}

void rom_loader::copyImage(const romEntry_t *entry)
{
	tag_t		*tagSource = ROM_GETNAME(*entry);
	uint32_t	length = ROM_GETLENGTH(*entry);
	uint8_t		*base = region->base() + ROM_GETOFFSET(*entry);


}

void rom_loader::processEntries(tag_t *tagName, const romEntry_t *parent, const romEntry_t *entry, device_t &device)
{

	while (!ROMENTRY_ISREGIONEND(*entry)) {

		if (ROMENTRY_ISFILL(*entry))
			fillImage(entry++);
		else if (ROMENTRY_ISCOPY(*entry))
			copyImage(entry++);
		else if (ROMENTRY_ISFILE(*entry)) {

			cty.printf("%s: Loading ROM file '%s'\n", device.deviceName(), ROM_GETNAME(*entry));

			entry++;
		} else
			entry++;
	}
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
				uint8_t  erase  = 0;

				region = sysMachine->memory().allocateRegion(rgnTagName, rgnLength, width, endian);

				if (ROMREGION_ISERASE(*entry))
					erase = ROMREGION_GETERASEVAL(*entry);
				memset(region->base(), erase, region->size());

				// Now processing ROM entries within region area
				processEntries(device.shortName(), entry, entry+1, device);
			}
		}
	}
}
