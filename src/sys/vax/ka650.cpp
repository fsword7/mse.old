/*
 * ka650new.cpp - KA650 system device
 *
 *  Created on: Jan 9, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/devsys.h"
#include "emu/driver.h"
#include "sys/vax/ka650.h"

using namespace std;

DEFINE_DEVICE_TYPE(KA650, ka650_device, "KA650", "KA650")

// Create system routines
void ka650_device::mv3900()
{
	cvax_cpu *cpu;

	cfgSetAddressMap(cpu, AS_PROGRAM, mv3900_mem);

	cout << "This is MicroVAX 3900 system" << endl;

}

void ka650_device::mv3900x()
{
	cvax_cpu *cpu;

	cpu = new cvax_cpu("cvax", sysConfig, nullptr, 0);

	cfgSetAddressMap(cpu, AS_PROGRAM, mv3900x_mem);

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

//COMPX(mv3900,  nullptr, KA650, ka650_device, mv3900,  mv3900_init,  "MicroVAX 3900")
//COMPX(mv3900x, nullptr, KA650, ka650_device, mv3900x, mv3900x_init, "MicroVAX 3900x")

