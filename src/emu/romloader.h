/*
 * romloader.h - ROM file load manager
 *
 *  Created on: Feb 22, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "emu/maprom.h"

class machine;

class rom_load_manager
{
public:
	rom_load_manager(machine *sys);
	~rom_load_manager();

	machine *getMachine() const { return sysMachine; }

private:
	machine *sysMachine;
};
