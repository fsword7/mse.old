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

static const char *ieNames[]  = { "Interrupt", "Exception", "Severe Exception" };
static const char *ieTypes[]  = { "INT", "EXC", "SVE" };
static const char *accNames[] = { "Kernel", "Executive", "Supervisor", "User" };

#define DSPL_CUR(mode) accNames[PSL_GETCUR(mode)]
#define DSPL_PRV(mode) accNames[PSL_GETPRV(mode)]

static const uint32_t swMasks[IPL_SMAX] =
{
	0xFFFE, 0xFFFC, 0xFFF8, 0xFFF0, // IPL 00 - 03
	0xFFE0, 0xFFC0, 0xFF80, 0xFF00, // IPL 04 - 07
	0xFE00, 0xFC00, 0xF800, 0xF000, // IPL 08 - 0B
	0xE000, 0xC000, 0x8000          // IPL 0C - 0E
};

// Evaluate IRQ levels
int vax_cpuDevice::evaluate()
{
	uint32_t ipl = PSL_GETIPL(psReg);

	// Evaluate software interrupt requests
	if (ipl >= IPL_SMAX)
		return 0;
	if (PREG_SISR & swMasks[ipl]) {
		for (int sidx = IPL_SMAX; sidx > 0; sidx--) {
			if (PREG_SISR & swMasks[sidx-1])
				return sidx;
		}
	}

	// No interrupt pending
	return 0;
}

void vax_cpuDevice::interrupt()
{
	uint32_t nipl;
	uint32_t vec;

	if ((nipl = IRQ_GETIPL(irqFlags)) != 0) {
		if (nipl <= IPL_SMAX) {
			// Software interrupts
			PREG_SISR &= ~(1u << nipl);
			vec = SCB_IPLSOFT + (nipl << 2);
		} else
			// Undefined IPL level
			throw STOP_UIPL;

		// perform exception routine
		exception(IE_INT, vec, nipl);
	} else
		irqFlags = 0;
}

int vax_cpuDevice::exception(int ie, uint32_t vec, uint32_t ipl)
{
	uint32_t opsl = psReg|ccReg;
	uint32_t opc  = REG_PC;
	uint32_t osp  = REG_SP;
	uint32_t prv  = PSL_GETCUR(psReg);
	uint32_t npc, npsl;

	// Set INIE flag
	flags |= CPU_INIE;

	// Clear all traps

	// Get SCBB vector from memory
	npc = readpl(PREG_SCBB + vec);
	if (ie == IE_SVE)
		npc |= 1;

	printf("%s: (%s) SCB vector %04X  New PC: %08X(%1X) Type: %s\n", devName.c_str(),
		ieTypes[ie], ZXTW(vec), ZXTL(npc & ~03), ZXTL(npc & 03), ieNames[ie]);

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

	// Set new PSL register
	if (ie == IE_INT) {
		psReg = npsl | PSL_SETIPL(ipl);
	} else {
		psReg = npsl | ((npc & 1) ? PSL_IPL : (opsl & PSL_IPL)) |
			PSL_SETPRV(prv);
	}
	ccReg = 0;

	// Save old PC and PSL in kernel mode
	curMode = ACC_MASK(AM_KERNEL);
	writev(REG_SP - (LN_LONG*2), opc, LN_LONG, WACC);
	writev(REG_SP - LN_LONG, opsl, LN_LONG, WACC);
	REG_SP -= (LN_LONG*2);

	// Set new PC address and reset opcode stream
	REG_PC = npc & SCB_ADDR;
	flushvi();

	// Set new access mode
	curMode = ACC_MASK(PSL_GETCUR(psReg));

	// Clear INIE flag
	flags &= ~CPU_INIE;

	printf("%s: (%s) Old PC=%08X PSL=%08X SP=%08X Access: %s,%s\n", devName.c_str(),
		ieTypes[ie], ZXTL(faultAddr), ZXTL(opsl), ZXTL(osp), DSPL_CUR(opsl), DSPL_PRV(opsl));
	printf("%s: (%s) New PC=%08X PSL=%08X SP=%08X Access: %s,%s\n", devName.c_str(),
		ieTypes[ie], ZXTL(REG_PC), ZXTL(psReg|ccReg), ZXTL(REG_SP), DSPL_CUR(npsl), DSPL_PRV(npsl));

	return 0;
}

// Resume from exception/interrupt routine
void vax_cpuDevice::resume()
{
	uint32_t npc, npsl;
	uint32_t opc, opsl, osp;
	uint32_t nacc, oacc;
	uint32_t nipl, oipl;

	opc  = REG_PC;
	opsl = psReg|ccReg;
	osp  = REG_SP;

	// Get PC and PSL from kernel/interrupt stack
	npc  = readv(REG_SP, LN_LONG, RACC);
	npsl = readv(REG_SP+LN_LONG, LN_LONG, RACC);

	// Get access mode from old and new PSL register
	nacc = PSL_GETCUR(npsl);
	oacc = PSL_GETCUR(psReg);
	oipl = PSL_GETIPL(psReg);

	// Check validation against MBZ and access modes
	if ((npsl & PSL_MBZ) || (nacc < oacc))
		throw EXC_RSVD_OPND_FAULT;
	if (nacc == AM_KERNEL) {
		// Check validation for kernel mode
		nipl = PSL_GETIPL(npsl);
		if ((npsl & PSL_IPL) && (((opsl & PSL_IS) == 0) || (nipl == 0)))
			throw EXC_RSVD_OPND_FAULT;
		if (nipl > PSL_GETIPL(opsl))
			throw EXC_RSVD_OPND_FAULT;
	} else {
		// Check validation for non-kernel mode
		if ((npsl & (PSL_IS|PSL_IPL)) || nacc > PSL_GETPRV(npsl))
			throw EXC_RSVD_OPND_FAULT;
	}
	// Check compatibility mode
	if (npsl & PSL_CM)
		throw EXC_RSVD_OPND_FAULT;

	// All validation check passed...
	REG_SP += (LN_LONG*2);

	// Save current SP register
	if (psReg & PSL_IS)
		PREG_ISP = REG_SP;
	else
		pRegs[oacc] = REG_SP;

	// Set new PSL register
	psReg = (psReg & PSL_TP) | (npsl & ~PSW_CC);
	ccReg = npsl & PSW_CC;

	// Set new PC register and reset opcode stream
	REG_PC = npc;
	flushvi();

	// Set new SP register for new access mode
	// Also check AST delivery request
	if ((psReg & PSL_IS) == 0) {
		REG_SP = pRegs[nacc];
		// Request AST delivery (software IPL 2)
		if (nacc >= PREG_ASTLVL)
			PREG_SISR |= SISR_2;
	}

	// Update current access mode
	curMode = ACC_MASK(PSL_GETCUR(psReg));

	// Update IRQ requests
	UpdateIRQ();

	printf("%s: (REI) Old PC=%08X PSL=%08X SP=%08X Access: %s,%s\n", devName.c_str(),
		ZXTL(opc), ZXTL(opsl), ZXTL(osp), DSPL_CUR(opsl), DSPL_PRV(opsl));
	printf("%s: (REI) New PC=%08X PSL=%08X SP=%08X Access: %s,%s\n", devName.c_str(),
		ZXTL(REG_PC), ZXTL(psReg|ccReg), ZXTL(REG_SP), DSPL_CUR(npsl), DSPL_PRV(npsl));
	if ((psReg & PSL_IS) == 0 && nacc >= PREG_ASTLVL)
		printf("%s: (REI) AST delivered (%d >= %d)\n", devName.c_str(),
			nacc, PREG_ASTLVL);
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
