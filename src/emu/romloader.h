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

private:
	machine *sysMachine;
};
