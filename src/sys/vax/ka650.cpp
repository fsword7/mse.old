/*
 * ka650.cpp
 *
 *  Created on: Mar 12, 2017
 *      Author: Timothy Stark
 *
 *  MicroVAX 3500/3600 system (KA650 system)
 *  MicroVAX 3800/3900 system (KA655 system)
 *
 *  KA655X is SIMH version of KA655 with 512MB memory expansion support and designed for emulators.
 *
 */

#include "emu/core.h"
#include "emu/devsys.h"
#include "sys/vax/vax.h"
#include "sys/vax/ka650.h"

// KA650/KA655 Memory Map
//
// 00000000 +-------------------------------------+
//          |           Main Memory               |
//          |- - - - - - - - - - - - - - - - - - -|
//          |       Up to 64/512 MB Memory        |
// 01000000 +-------------------------------------+
//          |             Reserved                |
// 20000000 +-------------------------------------+
//          |           QBus I/O space            |
// 20002000 +-------------------------------------+
//          |             Reserved                |
// 20040000 +-------------------------------------+
//          |               ROM                   |
// 20080000 +-------------------------------------+
//          |      Local I/O Register Area        |
// 20200000 +-------------------------------------+
//          |             Reserved                |
// 30000000 +-------------------------------------+
//          |          QBus Memory Space          |
// 303FFFFF +-------------------------------------+
//          |             Reserved                |
// 3FFFFFFF +-------------------------------------+
//
// Systen Memory Map
//
//   0000 0000 - 03FF FFFF  KA650/KA655 Main Memory (Up to 64 MB)
//   1000 0000 - 13FF FFFF  KA650/KA655 Secondary Cache Diagnostic Space
//   0000 0000 - 1FFF FFFF  KA655X Main Memory (Up to 512 MB)
//
//   2000 0000 - 2000 1FFF  Qbus I/O Page
//   2004 0000 - 2005 FFFF  ROM Space, Halt Protected
//   2006 0000 - 2007 FFFF  ROM Space, Halt Unprotected
//   2008 0000 - 201F FFFF  Local Register Space
//   3000 0000 - 303F FFFF  Qbus Memory Space

ka650_sysDevice::ka650_sysDevice()
{
}

ka650_sysDevice::~ka650_sysDevice()
{
}

ka650_sysDevice *ka650_sysDevice::create(std::string devName, std::string devType, sysModel *model)
{
	ka650_sysDevice *dev = new ka650_sysDevice();

	if (dev == nullptr)
		return nullptr;

	dev->devName = devName;
	dev->devType = devType;
	dev->devDesc = model->desc;

	return dev;
}

// ******************************************************************************

// Memory configuration for KA650/KA655/KA655X
cfgMemory ka650_cfgMemory[] =
{
	"16M",   (1u << 24),
	"32M",   (1u << 25),
	"48M",   (1u << 25)+(1u << 24),
	"64M",   (1u << 26),
	// null terminator
	{ nullptr }
};

cfgMemory ka655x_cfgMemory[] =
{
	"16M",   (1u << 24),
	"32M",   (1u << 25),
	"48M",   (1u << 25)+(1u << 24),
	"64M",   (1u << 26),
	"128M",  (1u << 27),
	"256M",  (1u << 28),
	"512M",  (1u << 29),
	// null terminator
	{ nullptr }
};


static Device *create(std::string devName, std::string devType, sysModel *model)
{
	ka650_sysDevice *dev;

	dev = ka650_sysDevice::create(devName, devType, model);

	return dev;
}

Driver ka640_sysDriver {
	"KA640",
	"KA640 CPU Model",
	__FILE__,
	nullptr,

	// Command handlers
	nullptr,
	nullptr,
	nullptr,

	// Function calls
	create
};

Driver ka650_sysDriver {
	"KA650",
	"KA650 CPU Model",
	__FILE__,
	nullptr,

	// Command handlers
	nullptr,
	nullptr,
	nullptr,

	// Function calls
	create
};

Driver ka655_sysDriver {
	"KA655",
	"KA655 CPU Model",
	__FILE__,
	nullptr,

	// Command handlers
	nullptr,
	nullptr,
	nullptr,

	// Function calls
	create
};

Driver ka655x_sysDriver {
	"KA655X",
	"KA655X CPU Model",
	__FILE__,
	nullptr,

	// Command handlers
	nullptr,
	nullptr,
	nullptr,

	// Function calls
	create
};
