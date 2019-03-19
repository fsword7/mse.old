/*
 * ka650new.h
 *
 *  Created on: Jan 9, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "dev/cpu/vax/cvax.h"

// KA650/KA655/KA655X Memory Map
//
// 00000000 +-------------------------------------+
//          |            Main Memory              |
//          |- - - - - - - - - - - - - - - - - - -|
//          |         Up to 64 MB Memory          |
// 04000000 +-------------------------------------+
//          |             Reserved                |
// 10000000 +-------------------------------------+
//          |     Secondary Cache Diagnostic      |
// 14000000 +-------------------------------------+
//          |             Reserved                |
// 20000000 +-------------------------------------+
//          :                                     :
//
// 00000000 +-------------------------------------+
//          |           Main Memory               |
//          |- - - - - - - - - - - - - - - - - - -|
//          |        Up to 512 MB Memory          |
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
// System Memory Map
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


// 30-bit Physical Address Access

// Memory Space (up to 64MB main memory)
//#define RAM_BASE	0
//#define RAM_WIDTH	26
//#define RAM_SIZE	(1u << RAM_WIDTH)
//#define RAM_MASK	(RAM_SIZE - 1)
//#define RAM_END		(RAM_BASE + RAM_SIZE)
//
//// Memory Space (up to 512MB main memory)
//#define RAMX_BASE	0
//#define RAMX_WIDTH	29
//#define RAMX_SIZE	(1u << RAMX_WIDTH)
//#define RAMX_MASK	(RAMX_SIZE - 1)
//#define RAMX_END	(RAMX_BASE + RAMX_SIZE)
//
//// Firmware Space (up to 128K ROM space)
//#define ROM_BASE	0x20040000
//#define ROM_WIDTH	17
//#define ROM_SIZE	(1u << ROM_WIDTH)
//#define ROM_MASK	(ROM_SIZE - 1)
//#define ROM_END		(ROM_BASE + (ROM_SIZE * 2))


// Cache Diagnostic Space
#define CDG_BASE   0x10000000
#define CDG_SIZE   (CDA_SIZE * CTA_SIZE)
#define CDG_END    (CDG_BASE + CDG_SIZE)

#define CDA_WIDTH  16
#define CDA_SIZE   (1u << CDA_WIDTH)
#define CDA_MASK   (CDA_SIZE - 1)

#define CTA_WIDTH  10
#define CTA_SIZE   (1u << CTA_WIDTH)
#define CTA_MASK   (CTA_SIZE - 1)

// Local Register Area (20080000 to 20100000)
#define REG_BASE	0x20080000             // Local Register Base Address
#define REG_WIDTH	19                     //    Bus Width
#define REG_SIZE	(1u << REG_WIDTH)      //    Total Bytes
#define REG_END		(REG_BASE + REG_SIZE)  // End of Local Register Area

// CMCTL - Memory Controller Registers
#define CM_BASE     (REG_BASE + 0x100)     // CMCTL Base Address
//#define CM_NREGS    18                     //    Number of Registers
#define CM_NREGS    19                     //    Number of Registers
#define CM_SIZE     (CM_NREGS << 2)        //    Total Bytes
#define CM_END      (CM_BASE + CM_SIZE)    // End of CMCTL Registers

// KA650 Board Registers
#define KA_BASE    (REG_BASE + 0x4000)    // KA650 Base Address
#define KA_NREGS   2                      //    Number of Registers
#define KA_SIZE    (KA_NREGS << 2)        //    Total Bytes
#define KA_END     (KA_BASE + KA_SIZE)    // End of KA650 Registers

// SSC Registers for MicroVAX III series
#define SSC_BASE    0x20140000             // SSC Base Address
#define SSC_NREGS   84                     //    Number of Registers
#define SSC_SIZE    (SSC_NREGS << 2)       //    Total Bytes
#define SSC_END     (SSC_BASE + SSC_SIZE)  // End of SSC Registers

// Non-Volatile RAM - 1024 bytes
#define NVR_BASE	(SSC_BASE + 0x400)
#define NVR_WIDTH	10
#define NVR_SIZE	(1u << NVR_WIDTH)
#define NVR_MASK	(NVR_SIZE - 1)
#define NVR_END		(NVR_BASE + NVR_SIZE)

// CQ BIC Registers
#define CQBIC_BASE   (REG_BASE) // CQBIC Base Address
#define CQBIC_NREGS  5          //   Number of Registers
#define CQBIC_SIZE   (CQBIC_NREGS << 2) // Total Bytes
#define CQBIC_END    (CQBIC_BASE + CQBIC_SIZE)

// CQBIC Q22-Bus Map Registers
#define CQMAP_BASE   (REG_BASE + 0x8000)
#define CQMAP_WIDTH  15
#define CQMAP_SIZE   (1u << CQMAP_WIDTH)
#define CQMAP_MASK   (CQMAP_SIZE - 1)
#define CQMAP_END    (CQMAP_BASE + CQMAP_SIZE)

// CQBIC Q22-Bus I/O Space Area
#define CQIO_BASE   (0x20000000)
#define CQIO_WIDTH  13
#define CQIO_SIZE   (1u << CQIO_WIDTH)
#define CQIO_MASK   (CQIO_SIZE - 1)
#define CQIO_END    (CQIO_BASE + CQIO_SIZE)

// CQBIC Q22-Bus Memory Space Area
#define CQMEM_BASE   (0x30000000)
#define CQMEM_WIDTH  22
#define CQMEM_SIZE   (1u << CQMEM_WIDTH)
#define CQMEM_MASK   (CQMEM_SIZE - 1)
#define CQMEM_END    (CQMEM_BASE + CQMEM_SIZE)

class ka650_device : public system_device
{
public:
	ka650_device(const system_config &config, const device_type_base &type, tag_t *tag, uint64_t clock)
	: system_device(config, type, tag, clock),
	  cpu(nullptr),
	  cssc(nullptr),
	  cmctl(nullptr),
	  cqbic(nullptr)
	{
	}

	void devStart() override;

	// System creator routines
	void mv3900(system_config &config);
	void mv3900x(system_config &config);

	// Address space mapping
	void mv3900_mem(mapAddress &map);
	void mv3900x_mem(mapAddress &map);

	// Model-specific system initialize routines
	static void mv3900_init();
	static void mv3900x_init();

	cvax_cpu 		*cpu;
	cssc_device		*cssc;
	cmctl_device	*cmctl;
	cqbic_device	*cqbic;
};
