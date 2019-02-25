/*
 * romloader.h - ROM file load manager
 *
 *  Created on: Feb 22, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "emu/maprom.h"

class machine;

class rom_loader
{
public:
	rom_loader(machine *sys, const cty_t &cty);
	~rom_loader();

	machine *systemMachine() const { return sysMachine; }


protected:

	void fillImage(const romEntry_t *);
	void copyImage(const romEntry_t *);

	void processEntries(tag_t *tagName, const romEntry_t *parent, const romEntry_t *entry, device_t &device);
	void processRegionList();

	const romEntry_t *first(device_t &device);
	const romEntry_t *next(const romEntry_t *);

private:
	machine *sysMachine;
	mapMemoryRegion *region;
	const cty_t &cty;
};
