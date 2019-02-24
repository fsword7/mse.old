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
	rom_loader(machine *sys);
	~rom_loader();

	machine *systemMachine() const { return sysMachine; }


protected:
	void processRegionList();

	const romEntry *first(device_t &device);
	const romEntry *next(const romEntry *);

private:
	machine *sysMachine;
};
