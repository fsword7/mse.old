/*
 * vax.cpp
 *
 *  Created on: Mar 2, 2017
 *      Author: Timothy Stark
 */

#include "emu/core.h"
#include "emu/console.h"
#include "emu/devsys.h"
#include "sys/vax/vax.h"

vax_sysDevice::vax_sysDevice()
: memSize(0), mem(nullptr)
{
}

vax_sysDevice::~vax_sysDevice()
{
	if (mem != nullptr)
		delete mem;
}

int vax_sysDevice::load(std::string fname)
{
	uint32_t addr, sAddr, eAddr;

	sAddr = 0;

	std::ifstream in(fname, std::ios::binary);

	addr = sAddr;
	while (in.read((char *)&mem[addr], 512)) {
		addr += 512;
	}
	eAddr = addr;

	in.close();

	std::cout << fname << ": Loaded " << std::hex << sAddr << " to " << eAddr << std::endl;
	return CMD_OK;
}

//Device *vaxCreate()
//{
//	return new vax_sysDevice();
//}

Driver vax_sysDriver {
	"VAX",
	"DEC VAX-11 Series"
	__FILE__,
	nullptr,

	// Command handlers
	nullptr,
	nullptr,
	nullptr,

};


extern Driver ka655_sysDriver;
extern Driver ka655x_sysDriver;

sysModel vax_sysModels[] =
{
	SYSTEM(vax,   "VAX minicomputer",    &vax_sysDriver),

//	MODEL(11/730,   vax, "VAX-11/730",       ka730_sysDriver),
//	MODEL(11/750,   vax, "VAX-11/750",       ka750_sysDriver),
//	MODEL(11/780,   vax, "VAX-11/780",       ka780_sysDriver),
//	MODEL(11/785,   vax, "VAX-11/785",       ka780_sysDriver),

//	MODEL(8200,     vax, "VAX 8200",         ka820_sysDriver),
//	MODEL(8250,     vax, "VAX 8250",         ka820_sysDriver),
//	MODEL(8300,     vax, "VAX 8300",         ka820_sysDriver),
//	MODEL(8350,     vax, "VAX 8350",         ka820_sysDriver),
//	MODEL(8600,     vax, "VAX 8600",         ka86_sysDriver),
//	MODEL(8650,     vax, "VAX 8650",         ka86_sysDriver),

	// MicroVAX model list
//	MODEL(mv3300,   vax, "MicroVAX 3300",    ka640_sysDriver),
//	MODEL(mv3400,   vax, "MicroVAX 3400",    ka640_sysDriver),
//	MODEL(mv3500,   vax, "MicroVAX 3500",    ka650_sysDriver),
//	MODEL(mv3600,   vax, "MicroVAX 3600",    ka650_sysDriver),
	MODEL(mv3800,   vax, "MicroVAX 3800",    &ka655_sysDriver),
	MODEL(mv3800x,  vax, "MicroVAX 3800X",   &ka655x_sysDriver),
	MODEL(mv3900,   vax, "MicroVAX 3900",    &ka655_sysDriver),
	MODEL(mv3900x,  vax, "MicroVAX 3900X",   &ka655x_sysDriver),

	// VAXserver model list
//	MODEL(vs3300,   vax, "VAXserver 3300",   ka640_sysDriver),
//	MODEL(vs3400,   vax, "VAXserver 3400",   ka640_sysDriver),
//	MODEL(vs3500,   vax, "VAXserver 3500",   ka650_sysDriver),
//	MODEL(vs3600,   vax, "VAXserver 3600",   ka650_sysDriver),
	MODEL(vs3800,   vax, "VAXserver 3800",   &ka655_sysDriver),
	MODEL(vs3800x,  vax, "VAXserver 3800X",  &ka655x_sysDriver),
	MODEL(vs3900,   vax, "VAXserver 3900",   &ka655_sysDriver),
	MODEL(vs3900x,  vax, "VAXserver 3900X",  &ka655x_sysDriver),

	// VAXstation model list
//	MODEL(vs3200g,  vax, "VAXstation 3300",  ka640_sysDriver),
//	MODEL(vs3500g,  vax, "VAXstation 3500",  ka650_sysDriver),
//	MODEL(vs3520g,  vax, "VAXstation 3520",  ka60_sysDriver),
//	MODEL(vs3540g,  vax, "VAXstation 3540",  ka60_sysDriver),

	// null terminator
	{ nullptr }
};
