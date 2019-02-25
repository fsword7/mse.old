/*
 * ka650new.cpp - KA650 system device
 *
 *  Created on: Jan 9, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/devsys.h"
#include "emu/maprom.h"
#include "emu/driver.h"
#include "sys/dec/vax/ka650.h"

using namespace std;

// Create system routines
void ka650_device::mv3900(system_config &config)
{
	cvax_cpu *cpu;

	cpu = CVAX(config, "cvax", 0);
	cpu->setAddressMap(AS_PROGRAM, &ka650_device::mv3900_mem);
	cpu->setDeviceName("CPU");

	cout << "This is MicroVAX 3900 system" << endl;

}

void ka650_device::mv3900x(system_config &config)
{
	cvax_cpu *cpu;

	cpu = CVAX(config, "cvax", 0);
	cpu->setAddressMap(AS_PROGRAM, &ka650_device::mv3900x_mem);
	cpu->setDeviceName("CPU");

	cout << "This is MicroVAX 3800x system" << endl;
}

void ka650_device::mv3900_mem(mapAddress &map)
{
}

void ka650_device::mv3900x_mem(mapAddress &map)
{
	map(0x00000000, 0x1FFFFFFF).ram();
	map(0x20040000, 0x2007FFFF).rom().mirror(0x1FFFF);
}

// Initialize system routines
void ka650_device::mv3900_init()
{

}

void ka650_device::mv3900x_init()
{

}

static const romEntry_t ROM_NAME(mv3900)[] =
{
		ROM_REGION32_LE("srm", 0x20000, 0),
		ROM_LOAD("ka655x.bin", 0x00000, 0x20000, nullptr, 0),
		ROM_END
};

#define rom_mv3900x rom_mv3900

COMP(mv3900,  nullptr, KA650, ka650_device, mv3900,  mv3900_init,  "DEC", "MicroVAX 3900")
COMP(mv3900x, nullptr, KA650, ka650_device, mv3900x, mv3900x_init, "DEC", "MicroVAX 3900x")
