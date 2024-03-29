/*
 * romloader.cpp
 *
 *  Created on: Feb 22, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/syslist.h"
#include "emu/machine.h"
#include "emu/emufile.h"
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

emuFile *rom_loader::processImageFile(tag_t *pathName, const romEntry_t *entry, osdFile::error &ferr)
{
	auto imageFile = new emuFile("fw", OPEN_FLAG_READ);
	std::string fullPathName;

	fullPathName = std::string(pathName) + std::string("/") + std::string(ROM_GETNAME(*entry));
//	cty.printf("Path: %s\n", fullPathName.c_str());
	ferr = imageFile->open(fullPathName);

	// For attempted open failure
	if (ferr != osdFile::NONE) {
		delete imageFile;
		imageFile = nullptr;
	}

//	imageFile->close();
//	delete imageFile;
//	imageFIle = nullptr;

	return imageFile;
}

int rom_loader::openImageFile(tag_t *tagName, const romEntry_t *entry)
{
	system_list sysList;
	uint32_t romSize = ROM_GETLENGTH(*entry);
	const system_driver *driver;
	std::string pathName;
	std::string triedNames = "";
	osdFile::error ferr;


	file = nullptr;
	for (driver = &sysMachine->driver(); file == nullptr && driver != nullptr; driver = sysList.clone(driver)) {
		pathName = driver->section;
		pathName += "/";
		pathName += driver->name;

		if (triedNames.length() > 0)
			triedNames += ",";
		triedNames += driver->name;

		file = processImageFile(pathName.c_str(), entry, ferr);
	}
//	cty.printf("Tried names: %s\n", triedNames.c_str());

	return 0;
}

int rom_loader::freadImageData(uint8_t *buffer, int length, const romEntry_t *parent)
{
	uint32_t actual;

	if (file != nullptr) {
		actual = file->read(buffer, length);
//		cty.printf("Loading %d bytes\n", actual);
		return actual;
	}
	return 0;
}

int rom_loader::readImageData(const romEntry_t *parent, const romEntry_t *entry)
{
	int			length = ROM_GETLENGTH(*entry);
	int			skip   = ROM_GETSKIP(*entry);
	int			dShift = ROM_GETBITSHIFT(*entry);
	int			dMask  = ((1 << ROM_GETBITWIDTH(*entry)) - 1) << dShift;
	int			gsize	= ROM_GETGROUPSIZE(*entry);
	int			reversed = ROM_ISREVERSED(*entry);
	int			ngroups = (length + gsize - 1) / gsize;
	uint8_t 	*base = region->base() + ROM_GETOFFSET(*entry);
	uint32_t	bufSize;

	cty.printf("%s: Loading ROM data: off=%X len=%X mask=%02X group=%d skip=%d reverse=%d\n",
		ROM_GETNAME(*entry), ROM_GETOFFSET(*entry), length, dMask, gsize, skip, reversed);

	if (dMask == 0xFF && (gsize == 1 || !reversed) && skip == 0)
		return freadImageData(base, length, parent);

//	bufSize = std::min(1024*1024*1024, length);
//	std::vector<uint8_t> buffer(bufSize);
//
//	skip += gsize;
//	while (length > 0) {
//
//	}

	return 0;
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
//		cty.printf("ROM Type: %d\n", ROM_GETFLAGS(*entry) & ROM_TYPE_MASK);

		if (ROMENTRY_ISFILL(*entry))
			fillImage(entry++);
		else if (ROMENTRY_ISCOPY(*entry))
			copyImage(entry++);
		else if (ROMENTRY_ISFILE(*entry)) {

			const romEntry_t *baserom = entry;
			int romLength = 0;

			cty.printf("%s: Loading ROM file '%s'... \n", device.deviceName(), ROM_GETNAME(*entry));
			openImageFile(tagName, entry);

			do {
				do {
					romEntry_t mentry = *entry++;

					// handle flag inheritance
//					if (!ROM_HASINHERITFLAG(&mentry))
//						lastFlags = ROM_GETFLAGS(mentry);
//					else
//						ROM_SETFLAG(mentry, ROM_GETFLAGS(mentry) & ROM_INHERITFLAGS) | lastFlags);

					if (!ROMENTRY_ISIGNORE(mentry))
						readImageData(parent, &mentry);
					romLength += ROM_GETLENGTH(mentry);

				} while (ROMENTRY_ISCONTINUE(*entry) || ROMENTRY_ISIGNORE(*entry));

				// Reset the file offset back to zero (starting).
				if (file != nullptr)
					file->seek(0, SEEK_SET);

			} while (ROMENTRY_ISRELOAD(*entry));

			if (file != nullptr) {
				cty.printf("done.\n");
				file->close();
				delete file;
				file == nullptr;
			}
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
