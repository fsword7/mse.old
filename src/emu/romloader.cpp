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


