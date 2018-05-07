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
#include "emu/debug.h"
#include "emu/devsys.h"
#include "emu/devcpu.h"
#include "dev/cpu/vax/vax.h"
#include "dev/cpu/vax/cvax.h"
#include "dev/comm/dec/cty.h"
#include "sys/vax/vax.h"
#include "sys/vax/ka650.h"


ka650_sysDevice::ka650_sysDevice()
{
}

ka650_sysDevice::~ka650_sysDevice()
{
}

ka650_sysDevice *ka650_sysDevice::create(std::string devName, std::string devType, sysModel *model)
{
	ka650_sysDevice *dev = new ka650_sysDevice();
	uint32_t addr = 0;

	if (dev == nullptr)
		return nullptr;

	dev->devName = devName;
	dev->devType = devType;
	dev->devDesc = model->desc;
	dev->driver  = model->driver;

	// Set up ROM space for SRM firmware
	dev->romData = new uint8_t[ROM_SIZE];
	dev->romSize = ROM_SIZE;

	std::ifstream in("data/fw/vax/ka655x.bin", std::ios::binary);
	while (in.read((char *)&dev->romData[addr], 512))
		addr += 512;
	in.close();

	return dev;
}

void ka650_sysDevice::reset()
{
	// KA650 local I/O registers
	cmReg[CM_nMSZ]    = memSize;
	kaReg[KA_nBDR]    = 0x80;
	ssReg[SSC_nDLEDR] = 0x0F;

	// Reset all CPU devices
	for (auto &cdev : cpu)
		cdev->reset();
}


// **********************************************************

#ifdef ENABLE_DEBUG
static const char *kaNames[] = { "CACR", "BDR" };
#endif /* ENABLE_DEBUG */

uint32_t ka650_sysDevice::readka(uint32_t pAddr, uint32_t acc)
{
	int reg = (pAddr - KA_BASE) >> 2;

#ifdef ENABLE_DEBUG
	if (dbg.checkFlags(DBG_IOREGS))
		dbg.log("%s: (R) %s => %08X\n",
			devName.c_str(), kaNames[reg], kaReg[reg]);
#endif /* ENABLE_DEBUG */

	// Return one of KA registers
	return kaReg[reg];
}

void ka650_sysDevice::writeka(uint32_t pAddr, uint32_t data, uint32_t acc)
{
	int reg = (pAddr - KA_BASE) >> 2;

	if ((reg == 0) && ((pAddr & 3) == 0)) {
		KA_CACR = (KA_CACR & (~data & CACR_W1C)) | CACR_FIXED;
		KA_CACR = (data & CACR_RW) | (KA_CACR & ~CACR_RW);
	}

#ifdef ENABLE_DEBUG
	if (dbg.checkFlags(DBG_IOREGS))
		dbg.log("%s: (W) %s <= %08X (Now: %08X)\n",
			devName.c_str(), kaNames[reg], data, kaReg[reg]);
#endif /* ENABLE_DEBUG */
}

// ************************************************************

#ifdef ENABLE_DEBUG
static const char *cmNames[] =
{
	"CMCNF00", "CMCNF01", "CMCNF02", "CMCNF03",
	"CMCNF04", "CMCNF05", "CMCNF06", "CMCNF07",
	"CMCNF08", "CMCNF09", "CMCNF10", "CMCNF11",
	"CMCNF12", "CMCNF13", "CMCNF14", "CMCNF15",
	"CMERR",   "CMCSR",   "CMSIZE",
};
#endif /* ENABLE_DEBUG */

uint32_t ka650_sysDevice::readcm(uint32_t pAddr, uint32_t acc)
{
	int reg = (pAddr - CM_BASE) >> 2;

#ifdef ENABLE_DEBUG
	if (dbg.checkFlags(DBG_IOREGS))
		dbg.log("%s: (R) %s (%08X) => %08X\n",
			devName.c_str(), cmNames[reg], pAddr, cmReg[reg]);
#endif /* ENABLE_DEBUG */
	return cmReg[reg];
}

void ka650_sysDevice::writecm(uint32_t pAddr, uint32_t data, uint32_t acc)
{
	int reg = (pAddr - CM_BASE) >> 2;
	int idx;

//	register uint32 ioData;
//	if (acc < LN_LONG) {
//		register uint32 sc   = (pAddr & ALIGN_LONG) << 3;
//		register uint32 mask = ((acc == LN_LONG) ? W_MASK : B_MASK) << sc;
//		ioData = ((data <<= sc) & mask) | (CM_REG(reg) & ~mask);
//	} else
//		ioData = data;

	if (acc < LN_LONG)
		data <<= (pAddr & ~ALIGN_LONG) << 3;

	switch (reg) {
		case CM_nERR: // CMCTL Error Register
			CM_ERR &= ~(data & CMERR_W1C);
			break;

		case CM_nCSR: // CMCTL Control/Status Register
			CM_CSR = data & CMCSR_RW;
			break;

		case CM_nMSZ: // CMCTL Memory Size
//			if ((MCHK_REF & REF_C) == 0)
//				MACH_CHECK(MCHK_WRITE);
			break;

		default:     // CMCTL Configuration Register
			if (data & CMCNF_SRQ) {
				for (idx = reg; idx < (reg + 4); idx++) {
					CM_REG(idx) &= ~CMCNF_SIG;
					if ((idx * MEM_BANK) < memSize)
						CM_REG(idx) |= MEM_SIG;
				}
			}
			CM_REG(reg) = (data & CMCNF_RW) |
				(CM_REG(reg) & ~CMCNF_RW);
			break;
	}

#ifdef ENABLE_DEBUG
	if (dbg.checkFlags(DBG_IOREGS))
		dbg.log("%s: (W) %s (%08X) <= %08X (Now: %08X)\n",
			devName.c_str(), cmNames[reg], pAddr, data, cmReg[reg]);
#endif /* ENABLE_DEBUG */
}

// ************************************************************

#ifdef ENABLE_DEBUG
static const char *sscNames[] =
{
	"BASE",   NULL,     NULL,     NULL,     /* 00-03 */
	"CR",     NULL,     NULL,     NULL,     /* 04-07 */
	"BTO",    NULL,     NULL,     NULL,     /* 08-0B */
	"DLEDR",  NULL,     NULL,     NULL,     /* 0C-0F */
	NULL,     NULL,     NULL,     NULL,     /* 10-13 */
	NULL,     NULL,     NULL,     NULL,     /* 14-17 */
	NULL,     NULL,     NULL,     "TODR",   /* 18-1B */
	"CSRS",   "CSRD",   "CSTS",   "CSTD",   /* 1C-1F */
	"RXCS",   "RXDB",   "TXCS",   "TXDB",   /* 20-23 */
	NULL,     NULL,     NULL,     NULL,     /* 24-27 */
	NULL,     NULL,     NULL,     NULL,     /* 28-2B */
	NULL,     NULL,     NULL,     NULL,     /* 2C-2F */
	NULL,     NULL,     NULL,     NULL,     /* 30-33 */
	NULL,     NULL,     NULL,     NULL,     /* 34-37 */
	NULL,     NULL,     NULL,     NULL,     /* 38-3B */
	NULL,     NULL,     NULL,     NULL,     /* 3C-3F */
	"TCR0",   "TIR0",   "TNIR0",  "TIVR0",  /* 40-43 */
	"TCR1",   "TIR1",   "TNIR1",  "TIVR1",  /* 44-47 */
	NULL,     NULL,     NULL,     NULL,     /* 48-4B */
	"AD0MAT", "AD0MSK", NULL,     NULL,     /* 4C-4F */
	"AD1MAT", "AD1MSK", NULL,     NULL,     /* 50-53 */
};
#endif /* ENABLE_DEBUG */

uint32_t ka650_sysDevice::readssc(uint32_t pAddr, uint32_t acc)
{
	int      reg = (pAddr - SSC_BASE) >> 2;
	uint32_t data = 0;

	switch (reg) {
		case SSC_nTIR0:
//			data = ka650clk_ReadTIR((KA650_CPU *)cpu, 0, 0);
			break;
		case SSC_nTIR1:
//			data = ka650clk_ReadTIR((KA650_CPU *)cpu, 1, 0);
			break;
		default:
			data = ssReg[reg];
			break;
	}

#ifdef ENABLE_DEBUG
	if (dbg.checkFlags(DBG_IOREGS))
		dbg.log("%s: (R) %s => %08X\n",
			devName.c_str(), (sscNames[reg] ? sscNames[reg] : "<Unknown>"),
			data);
#endif /* ENABLE_DEBUG */

	return data;
}

void ka650_sysDevice::writessc(uint32_t pAddr, uint32_t data, uint32_t acc)
{
	int       reg = (pAddr - SSC_BASE) >> 2;
//	UQ_MAPIO *io;
	uint32_t  ioData;

	if (acc < LN_LONG) {
		uint32_t sc   = (pAddr & ~ALIGN_LONG) << 3;
		uint32_t mask = ((acc == LN_WORD) ? MSK_WORD : MSK_BYTE) << sc;
		ioData = ((data << sc) & mask) | (ssReg[reg] & ~mask);
	} else
		ioData = data;

	switch(reg) {
		case SSC_nBASE:
			SSC_xBASE = (ioData & BASE_ADDR) | BASE_MBO;
			break;

		case SSC_nCR:
			SSC_CR &= ~(ioData & CNF_W1C);
			SSC_CR  = (ioData & CNF_RW) | (SSC_CR & ~CNF_RW);
			break;

		case SSC_nBTO:
			SSC_BTO &= ~(ioData & BTO_W1C);
			SSC_BTO  = (ioData & BTO_RW) | (SSC_BTO & ~BTO_RW);
			break;

		case SSC_nDLEDR:
			SSC_LED = ioData & LED_MASK;
			break;

		case SSC_nTCR0:
//			ka650clk_WriteTCR((KA650_CPU *)cpu, 0, data);
			break;
		case SSC_nTNIR0:
//			SSC_TNIR0 = data;
			break;
		case SSC_nTIVR0:
//			SSC_TIVR0 = data & TIVR_VEC;
//			io = &KA650_REG(cpu, ioTimer[0]);
//			io->irqVector = SSC_TIVR0;
//			io->SetVector(io, SSC_TIVR0);
			break;

		case SSC_nTCR1:
//			ka650clk_WriteTCR((KA650_CPU *)cpu, 1, data);
			break;
		case SSC_nTNIR1:
//			SSC_TNIR1 = data;
			break;
		case SSC_nTIVR1:
//			SSC_TIVR1 = data & TIVR_VEC;
//			io = &KA650_REG(cpu, ioTimer[1]);
//			io->irqVector = SSC_TIVR1;
//			io->SetVector(io, SSC_TIVR1);
			break;

		case SSC_nAD0MAT:
		case SSC_nAD0MSK:
		case SSC_nAD1MAT:
		case SSC_nAD1MSK:
			ssReg[reg] = data & ADS_MASK;
			break;
	}

#ifdef ENABLE_DEBUG
	if (dbg.checkFlags(DBG_IOREGS))
		dbg.log("%s: (W) %s <= %08X (Now: %08X)\n",
			devName.c_str(), (sscNames[reg] ? sscNames[reg] : "<Unknown>"),
			ioData, ssReg[reg]);
#endif /* ENABLE_DEBUG */
}

// **************************************************************

uint32_t ka650_sysDevice::readcdg(uint32_t pAddr, uint32_t acc)
{
	uint32_t row  = CDG_GETROW(pAddr);
	uint32_t tag  = CDG_GETTAG(pAddr);
	uint32_t data = CDG_DATA(row);

	KA_CACR &= ~CACR_DRO;
	KA_CACR |= (getParity(ZXTB(data >> 24), 1) << (CACR_P_DPAR + 3)) |
	           (getParity(ZXTB(data >> 16), 0) << (CACR_P_DPAR + 2)) |
	           (getParity(ZXTB(data >> 8),  1) << (CACR_P_DPAR + 1)) |
	           (getParity(ZXTB(data >> 0),  0) << (CACR_P_DPAR + 0));

	return data;
}

void ka650_sysDevice::writecdg(uint32_t pAddr, uint32_t data, uint32_t acc)
{
	uint32_t row = CDG_GETROW(pAddr);
	uint32_t ioData;

	if (acc < LN_LONG) {
		uint32_t sc   = (pAddr & ALIGN_LONG) << 3;
		uint32_t mask = ((acc == LN_WORD) ? MSK_WORD : MSK_BYTE) << sc;
		ioData = ((data << sc) & mask) | (CDG_DATA(row) & ~mask);
	} else
		ioData = data;

	CDG_DATA(row) = ioData;
}

// **************************************************************

#define ROMB(addr) ((uint8_t *)romData)[addr]
#define ROMW(addr) ((uint16_t *)romData)[addr >> 1]
#define ROML(addr) ((uint32_t *)romData)[addr >> 2]

#define NVRB(addr) ((uint8_t *)nvrData)[addr]
#define NVRW(addr) ((uint16_t *)nvrData)[addr >> 1]
#define NVRL(addr) ((uint32_t *)nvrData)[addr >> 2]

// Aligned I/O read access
uint32_t ka650_sysDevice::readio(uint32_t pAddr, uint32_t acc)
{
	uint32_t data;

	if ((pAddr >= ROM_BASE) && (pAddr < ROM_END)) {
		pAddr &= ROM_MASK;
		if (acc == LN_LONG)
			return ROML(pAddr);
		else if (acc == LN_WORD)
			return ROMW(pAddr);
		else
			return ROMB(pAddr);
	} else if ((pAddr >= NVR_BASE) && (pAddr < NVR_END)) {
		pAddr &= NVR_MASK;
		if (acc == LN_LONG)
			return NVRL(pAddr);
		else if (acc == LN_WORD)
			return NVRW(pAddr);
		else
			return NVRB(pAddr);
	}
//	else if ((pAddr >= CQIO_BASE) && (pAddr < CQIO_END))
//		return ka650cpu_ReadCQIO(cpu, pAddr, acc);
//	else if ((pAddr >= CQMAP_BASE) && (pAddr < CQMAP_END))
//		data = ka650qba_ReadMAP(KA650_REG(cpu, qbaUnit), pAddr, acc);
	else if ((pAddr >= SSC_BASE) && (pAddr < SSC_END))
		data = readssc(pAddr, acc);
	else if ((pAddr >= KA_BASE) && (pAddr < KA_END))
		data = readka(pAddr, acc);
//	else if ((pAddr >= CQBIC_BASE) && (pAddr < CQBIC_END))
//		data = ka650qba_ReadBIC(KA650_REG(cpu, qbaUnit), pAddr, acc);
	else if ((pAddr >= CM_BASE) && (pAddr < CM_END))
		data = readcm(pAddr, acc);
//	else if ((pAddr >= CQMEM_BASE) && (pAddr < CQMEM_END))
//		data = ka650qba_ReadMEM(KA650_REG(cpu, qbaUnit), pAddr, acc);
	else if ((pAddr >= CDG_BASE) && (pAddr < CDG_END))
		data = readcdg(pAddr, acc);
	else
		goto NXM;

	if (acc == LN_LONG)
		return data;
	else if (acc == LN_WORD)
		return (data >> ((pAddr & 2) ? 16 : 0)) & MSK_WORD;
	else
		return (data >> ((pAddr & 3) << 3)) & MSK_BYTE;

NXM:
#ifdef ENABLE_DEBUG
//	if (dbg.checkFlags(DBG_PAGEFAULT))
//		dbg.log("%s: (R) CPU Non-Existant Memory on %08X at PC %08X\n",
//			devName.c_str(), pAddr, 0);
#endif /* ENABLE_DEBUG */
//	if (MCHK_REF & REF_C)
//		return 0;
//	// Go to machine check exception.
//	SSC_BTO |= (BTO_BTO|BTO_RWT);
//	MACH_CHECK(MCHK_READ);
	return 0;
}

// Aligned I/O write access
void ka650_sysDevice::writeio(uint32_t pAddr, uint32_t data, uint32_t acc)
{
	if ((pAddr >= NVR_BASE) && (pAddr < NVR_END)) {
		pAddr &= NVR_MASK;
		if (acc == LN_LONG)
			NVRL(pAddr) = data;
		else if (acc == LN_WORD)
			NVRW(pAddr) = data;
		else
			NVRB(pAddr) = data;
	}
//	else if ((pAddr >= CQIO_BASE) && (pAddr < CQIO_END))
//		ka650cpu_WriteCQIO(cpu, pAddr, data, acc);
//	else if ((pAddr >= CQMAP_BASE) && (pAddr < CQMAP_END))
//		ka650qba_WriteMAP(KA650_REG(cpu, qbaUnit), pAddr, data, acc);
	else if ((pAddr >= SSC_BASE) && (pAddr < SSC_END))
		writessc(pAddr, data, acc);
	else if ((pAddr >= KA_BASE) && (pAddr < KA_END))
		writeka(pAddr, data, acc);
//	else if ((pAddr >= CQBIC_BASE) && (pAddr < CQBIC_END))
//		ka650qba_WriteBIC(KA650_REG(cpu, qbaUnit), pAddr, data, acc);
//	else if ((pAddr >= CQMEM_BASE) && (pAddr < CQMEM_END))
//		ka650qba_WriteMEM(KA650_REG(cpu, qbaUnit), pAddr, data, acc);
	else if ((pAddr >= CM_BASE) && (pAddr < CM_END))
		writecm(pAddr, data, acc);
	else if ((pAddr >= CDG_BASE) && (pAddr < CDG_END))
		writecdg(pAddr, data, acc);
	else {
#ifdef DEBUG
//		if (dbg.checkFlags(DBG_PAGEFAULT))
//			dbg.log("%s: (W) CPU Non-Existant Memory on %08X at PC %08X\n",
//				devName.c_str(), pAddr, CPU_PCADDR);
#endif /* DEBUG */
//		if (MCHK_REF & REF_C)
//			return;
//		// Go to machine check exception.
//		SSC_BTO |= (BTO_BTO|BTO_RWT);
//		MACH_CHECK(MCHK_WRITE);
	}

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

cfgMemory ka650x_cfgMemory[] =
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
	cvax_cpuDevice *cpu;
	dec::ctyDevice *cty;

	dev = ka650_sysDevice::create(devName, devType, model);

	cpu = cvax_cpuDevice::create(dev, "cpu0");
	cty = dec::ctyDevice::create(dev, "cty0");

	return dev;
}

extern Command vaxCommands[];
extern Command vaxSetCommands[];
extern Command vaxShowCommands[];
extern Command vaxListCommands[];

Driver ka640_sysDriver {
	"KA640",
	"KA640 CPU Model",
	__FILE__,
	nullptr,

	// Configurations
	ka650_cfgMemory,

	// Command handlers
	vaxCommands,
	vaxSetCommands,
	vaxShowCommands,
	vaxListCommands,

	// Function calls
	create
};

Driver ka650_sysDriver {
	"KA650",
	"KA650 CPU Model",
	__FILE__,
	nullptr,

	// Configurations
	ka650_cfgMemory,

	// Command handlers
	vaxCommands,
	vaxSetCommands,
	vaxShowCommands,
	vaxListCommands,

	// Function calls
	create
};

Driver ka655_sysDriver {
	"KA655",
	"KA655 CPU Model",
	__FILE__,
	nullptr,

	// Configurations
	ka650_cfgMemory,

	// Command handlers
	vaxCommands,
	vaxSetCommands,
	vaxShowCommands,
	vaxListCommands,

	// Function calls
	create
};

Driver ka655x_sysDriver {
	"KA655X",
	"KA655X CPU Model",
	__FILE__,
	nullptr,

	// Configurations
	ka650x_cfgMemory,

	// Command handlers
	vaxCommands,
	vaxSetCommands,
	vaxShowCommands,
	vaxListCommands,

	// Function calls
	create
};
