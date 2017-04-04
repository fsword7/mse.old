/*
 * vax.cpp
 *
 *  Created on: Mar 12, 2017
 *      Author: Timothy Stark
 */

#include "emu/core.h"
#include "emu/devcpu.h"
#include "dev/cpu/vax/mtpr.h"
#include "dev/cpu/vax/vax.h"
#include "dev/cpu/vax/opcodes.h"

vax_cpuDevice::vax_cpuDevice()
{
	buildOpcodes();
//	reset();
}

vax_cpuDevice::~vax_cpuDevice()
{
}

//void vax_cpuDevice::reset()
//{
//	// Initialize all working registers
//	for (int idx = 0; idx < VAX_nGREGS; idx++)
//		gRegs[idx].l = 0;
//}

int  vax_cpuDevice::boot()
{
	return 0;
}

void vax_cpuDevice::init()
{
}

void vax_cpuDevice::assignMemory(uint8_t *mem, uint32_t memSize)
{
	this->mem     = mem;
	this->memSize = memSize;
}

void vax_cpuDevice::setPCAddr(uint32_t pcAddr)
{
	REG_PC = pcAddr;
}

uint32_t vax_cpuDevice::readpr(uint32_t)
{
	return 0;
}

void vax_cpuDevice::writepr(uint32_t, uint32_t)
{
}

char *vax_cpuDevice::stringCC(uint32_t cc)
{
	static char ccstr[5];

	ccstr[0] = (cc & CC_N) ? 'N' : '-';
	ccstr[1] = (cc & CC_Z) ? 'Z' : '-';
	ccstr[2] = (cc & CC_V) ? 'V' : '-';
	ccstr[3] = (cc & CC_C) ? 'C' : '-';
	ccstr[4] = 0;

	return ccstr;
}

int vax_cpuDevice::getBit()
{
	int32_t  pos = opRegs[0];
	int32_t  reg = opRegs[1];
	uint32_t ea, src;

	if (reg >= 0) {
//		if (ZXTL(pos) > 31)
//			throw ESC_RSVD_OPND_FAULT;
		src = gRegs[reg].l;
		printf("%s: R%d %08X<%d> => %d\n", devName.c_str(),
			reg, src, pos, (src >> pos) & 1);
	} else {
		ea   = opRegs[2] + (pos >> 3);
		src  = readv(ea, LN_BYTE, RACC);
		pos &= 7;
		printf("%s: %08X => %02X<%d> => %d\n", devName.c_str(),
			ea, ZXTB(src), pos, (src >> pos) & 1);
	}

	return (src >> pos) & 1;
}

int vax_cpuDevice::setBit(int bit)
{
	int32_t  pos = opRegs[0];
	int32_t  reg = opRegs[1];
	uint32_t ea, src, dst;
	int      obit;

	if (reg >= 0) {
//		if (ZXTL(pos) > 31)
//			throw ESC_RSVD_OPND_FAULT;
		src  = gRegs[reg].l;
		obit = (src >> pos) & 1;
		dst  = bit ? (src | (1u << pos)) : (src & ~(1u << pos));
		gRegs[reg].l = dst;
		printf("%s: R%d %08X<%d> (now: %08X) <= %d (old: %d)\n",
			devName.c_str(), reg, src, dst, pos, bit, obit);
	} else {
		ea   = opRegs[2] + (pos >> 3);
		src  = readv(ea, LN_BYTE, RACC);
		pos &= 7;
		obit = (src >> pos) & 1;
		dst  = bit ? (src | (1u << pos)) : (src & ~(1u << pos));
		writev(ea, dst, LN_BYTE, WACC);
		printf("%s: %08X => %02X<%d> (now: %02X) <= %d (old: %d)\n",
			devName.c_str(), ea, ZXTB(src), ZXTB(dst), pos, bit, obit);
	}

	return obit;
}

const char *ieNames[] =
{
	"Interrupt",
	"Exception",
	"Severe Exception"
};

int vax_cpuDevice::exception(int ie, uint32_t vec, uint32_t ipl)
{
	uint32_t npc, opc = REG_PC;
	uint32_t npsl, opsl = psReg|ccReg;
	uint32_t prv = PSL_GETCUR(psReg);

	// Get SCBB vector from memory
	npc = readpl(PREG_SCBB + vec);
	if (ie == IE_SVE)
		npc |= 1;

	printf("%s: SCB vector %04X  New PC: %08X(%1X) Type: %s\n",
		devName.c_str(), vec, npc & ~03, npc & 03, ieNames[ie]);

	if (npc & 2) {
		return STOP_ILLVEC;
	}

	// Switch SP registers
	if (opsl & PSL_IS)
		npsl = PSL_IS;
	else {
		pRegs[prv] = REG_SP;
		if (npc & 1) {
			npsl   = PSL_IS;
			REG_SP = PREG_ISP;
		} else {
			npsl   = 0;
			REG_SP = PREG_KSP;
		}
	}

	// Update PSL register
	if (ie == IE_INT) {
		psReg = npsl | PSL_SETIPL(ipl);
	} else {
		psReg = npsl | ((npc & 1) ? PSL_IPL : (opsl & PSL_IPL)) |
			PSL_SETPRV(prv);
	}
	ccReg = 0;

	// Reset access mode

	// Save old PC and PSL into kernel/interrupt stack pointer
	writev(REG_SP - (LN_LONG*2), opc, LN_LONG, WACC);
	writev(REG_SP - LN_LONG, opsl, LN_LONG, WACC);
	REG_SP -= (LN_LONG*2);

	REG_PC = npc & SCB_ADDR;

	return 0;
}

int vax_cpuDevice::fault(uint32_t vec)
{
	int rc;

	// Reset fault PC address
	REG_PC = faultAddr;

	switch (vec) {
	case SCB_RESIN|SCB_NOPRIV:
		vec &= ~SCB_NOPRIV;
	case SCB_RESAD:
	case SCB_RESOP:
	case SCB_RESIN:
		if ((rc = exception(IE_EXC, vec, 0)) != 0)
			return rc;
		break;

	}

	return 0;
}

//#define CPU_CLASS vax_cpuDevice
//#include "dev/cpu/vax/executes.h"
