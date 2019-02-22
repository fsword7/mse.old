/*
 * romloader.cpp
 *
 *  Created on: Feb 22, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/machine.h"
#include "emu/romloader.h"


rom_load_manager::rom_load_manager(machine *sys)
: sysMachine(sys)
{

}

rom_load_manager::~rom_load_manager()
{
}


