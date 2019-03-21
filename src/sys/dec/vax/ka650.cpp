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
#include "dev/chip/dec/vax/cssc.h"
#include "dev/chip/dec/vax/cmctl.h"
#include "dev/chip/dec/vax/cqbic.h"
#include "sys/dec/vax/ka650.h"

using namespace std;

void ka650_device::devStart()
{
}

// Create system routines
void ka650_device::mv3900(system_config &config)
{

	cpu = CVAX(config, "cvax", 0);
	cpu->setAddressMap(AS_PROGRAM, &ka650_device::mv3900_mem);
	cpu->setDeviceName("cpu");

	cssc = CSSC(config, "cssc", 0);
//	cpu->setSystemSupport(cssc);
	cmctl = CMCTL(config, "cmctl", 0);
	cqbic = CQBIC(config, "cqbic", 0);

	cssc->setDeviceName("cssc");
	cmctl->setDeviceName("cmctl");
	cqbic->setDeviceName("cqbic");

	cout << "This is MicroVAX 3900 system" << endl;

}

void ka650_device::mv3900x(system_config &config)
{

	cpu = CVAX(config, "cvax", 0);
	cpu->setAddressMap(AS_PROGRAM, &ka650_device::mv3900x_mem);
	cpu->setDeviceName("cpu");

	cssc = CSSC(config, "cssc", 0);
//	cpu->setSystemSupport(cssc);
	cmctl = CMCTL(config, "cmctl", 0);
	cqbic = CQBIC(config, "cqbic", 0);

	cssc->setDeviceName("cssc");
	cmctl->setDeviceName("cmctl");
	cqbic->setDeviceName("cqbic");

	cout << "This is MicroVAX 3800x system" << endl;
}

void ka650_device::mv3900_mem(mapAddress &map)
{
	map.setGlobalMask(0x3FFFFFFF);
	map(0x00000000, 0x03FFFFFF).ram(); // Up to 64MB main memory
//	map(0x10000000, 0x103FFFFF); // Cache registers
//	map(0x20000000, 0x20001FFF); // CQBIC Q22-bus I/O device space
	map(0x20040000, 0x2007FFFF).rom().mirror(0x20000).region("ka655x-srm");
//	map(0x20080000, 0x2008001F)rw("cqbic", FUNC(cqbic_device::read), FUNC(cqbic_device::write)); // CQBIC I/O registers
//	map(0x20080100, 0x200801FF)rw("cmctl", FUNC(cmctl_device::read), FUNC(cmctl_device::write)); // CMCTL registers
//	map(0x20084000, 0x20084007); // KA650 board registers
//	map(0x20088000, 0x2008FFFF); // CQBIC Q22-bus I/O map registers
//	map(0x20140000, 0x201403FF)rw("cssc", FUNC(cssc_device::read), FUNC(cssc_device::write)); // CSSC registers
//	map(0x20140400, 0x201407FF); // NVRAM memory space
//	map(0x30000000, 0x303FFFFF); // CQBIC Q22-bus I/O memory space
}

void ka650_device::mv3900x_mem(mapAddress &map)
{
	map.setGlobalMask(0x3FFFFFFF);
	map(0x00000000, 0x1FFFFFFF).ram(); // Up to 512MB main memory
//	map(0x20000000, 0x20001FFF); // CQBIC Q22-bus I/O device space
	map(0x20040000, 0x2007FFFF).rom().mirror(0x20000).region("ka655x-srm");
//	map(0x20080000, 0x2008001F).rw("cqbic", FUNC(cqbic_device::read), FUNC(cqbic_device::write)); // CQBIC I/O registers
//	map(0x20080100, 0x200801FF).rw("cmctl", FUNC(cmctl_device::read), FUNC(cmctl_device::write)); // CMCTL registers
//	map(0x20084000, 0x20084007); // KA650 board registers
//	map(0x20088000, 0x2008FFFF); // CQBIC Q22-bus I/O map registers
//	map(0x20140000, 0x201403FF).rw("cssc", FUNC(cssc_device::read), FUNC(cssc_device::write)); // CSSC registers
//	map(0x20140400, 0x201407FF); // NVRAM memory space
//	map(0x30000000, 0x303FFFFF); // CQBIC Q22-bus I/O memory space
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
		ROM_REGION32_LE("ka655x-srm", 0x20000, 0),
		ROM_LOAD("ka655x.bin", 0x00000, 0x20000, nullptr, 0),
		ROM_END
};

#define rom_mv3900x rom_mv3900

COMP(mv3900,  nullptr, vax, KA650, ka650_device, mv3900,  mv3900_init,  "DEC", "MicroVAX 3900")
COMP(mv3900x, mv3900,  vax, KA650, ka650_device, mv3900x, mv3900x_init, "DEC", "MicroVAX 3900x")
