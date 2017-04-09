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

const uint32_t vax_cpuDevice::mskList[] = {
	0x00000000,
	0x00000001, 0x00000003, 0x00000007, 0x0000000F,
	0x0000001F, 0x0000003F, 0x0000007F, 0x000000FF,
	0x000001FF, 0x000003FF, 0x000007FF, 0x00000FFF,
	0x00001FFF, 0x00003FFF, 0x00007FFF, 0x0000FFFF,
	0x0001FFFF, 0x0003FFFF, 0x0007FFFF, 0x000FFFFF,
	0x001FFFFF, 0x003FFFFF, 0x007FFFFF, 0x00FFFFFF,
	0x01FFFFFF, 0x03FFFFFF, 0x07FFFFFF, 0x0FFFFFFF,
	0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFF
};

const uint32_t vax_cpuDevice::sgnList[] = {
	0x00000000,
	0x00000001, 0x00000002, 0x00000004, 0x00000008,
	0x00000010, 0x00000020, 0x00000040, 0x00000080,
	0x00000100, 0x00000200, 0x00000400, 0x00000800,
	0x00001000, 0x00002000, 0x00004000, 0x00008000,
	0x00010000, 0x00020000, 0x00040000, 0x00080000,
	0x00100000, 0x00200000, 0x00400000, 0x00800000,
	0x01000000, 0x02000000, 0x04000000, 0x08000000,
	0x10000000, 0x20000000, 0x40000000, 0x80000000
};

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

// For CALLG/CALLS instruction
void vax_cpuDevice::call(bool stkFlag)
{
	uint32_t npc = ZXTL(opRegs[1]);
	uint32_t mask, entry;
	uint32_t tsp;
	int      stkSize;

	mask = readv(npc, LN_WORD, RACC);
	if (mask & CALL_MBZ)
		throw RSVD_OPND_FAULT;

	// Check write access for page faults
	stkSize = stkFlag ? 24 : 20;
	for (int idx = REG_nR0; idx <= REG_nR11; idx++)
		if ((mask >> idx) & 1)
			stkSize += LN_WORD;

	// Save registers into stack
	if (stkFlag) {
		writev(REG_SP - LN_LONG, opRegs[0], LN_LONG, WACC);
		REG_SP -= LN_LONG;
	}
	entry = ((REG_SP & CALL_SPA) << CALL_P_SPA) |
		    ((stkFlag ? 1u : 0u) << CALL_P_S) |
		    ((psReg|ccReg) & CALL_PSW) |
		    ((mask & CALL_MASK) << CALL_P_MASK);
	tsp = REG_SP & ~CALL_SPA;
	for (int idx = REG_nR11; idx >= REG_nR0; idx--) {
		if ((mask >> idx) & 1) {
			writev(tsp - LN_LONG, gRegs[idx].l, LN_LONG, WACC);
			tsp -= LN_LONG;
		}
	}
	writev(tsp - LN_LONG, REG_PC, LN_LONG, WACC);
	writev(tsp - (LN_LONG*2), REG_FP, LN_LONG, WACC);
	writev(tsp - (LN_LONG*3), REG_AP, LN_LONG, WACC);
	writev(tsp - (LN_LONG*4), entry, LN_LONG, WACC);
	writev(tsp - (LN_LONG*5), 0, LN_LONG, WACC);

	// Set new registers
	REG_AP = stkFlag ? REG_SP : opRegs[0];
	REG_SP = tsp - (LN_LONG*5);
	REG_FP = REG_SP;
	REG_PC = npc + LN_WORD;
	psReg = (psReg & ~(PSW_DV|PSW_FU|PSW_IV)) |
			((mask & CALL_DV) ? PSW_DV : 0) |
			((mask & CALL_IV) ? PSW_IV : 0);
	ccReg = 0;

	// Clear instruction look-ahead
	flushvi();
}

// For RET instruction
void vax_cpuDevice::ret()
{
	uint32_t entry, mask;
	uint32_t npc;
	uint32_t tsp = REG_FP;
	int      stkSize;

	// Get entry mask from stack
	entry = readv(tsp + LN_LONG, LN_LONG, RACC);
	if (entry & CALL_MBZ)
		throw RSVD_OPND_FAULT;
	mask = (entry >> CALL_P_MASK) & CALL_MASK;

	// Check read access for page faults
	stkSize = (entry & CALL_S) ? 23 : 19;
	for (int idx = REG_nR0; idx <= REG_nR11; idx++)
		if ((mask >> idx) & 1)
			stkSize += LN_LONG;

	// Restore PC, FP, AP, and SP registers
	REG_AP = readv(tsp + (LN_LONG*2), LN_LONG, RACC);
	REG_FP = readv(tsp + (LN_LONG*3), LN_LONG, RACC);
	npc    = readv(tsp + (LN_LONG*4), LN_LONG, RACC);
	tsp   += LN_LONG*5;

	// Restore registers from stack
	for (int idx = REG_nR0; idx <= REG_nR11; idx++) {
		if ((mask >> idx) & 1) {
			gRegs[idx].l = readv(tsp, LN_LONG, RACC);
			tsp += LN_LONG;
		}
	}

	// Dealign stack pointer
	REG_SP = tsp + ((entry >> CALL_P_SPA) & CALL_SPA);

	// Pop old argument if CALLS instruction is used
	if (entry & CALL_S) {
		uint32_t tmp = readv(REG_SP, LN_LONG, RACC);
		REG_SP += ((tmp & MSK_BYTE) << 2) + LN_LONG;
	}

	// Reset PSW register
	psReg = (psReg & ~PSW_MASK) | (entry & (PSW_MASK & ~PSW_CC));
	ccReg = entry & PSW_CC;

	// Set new PC register and clear instruction look-ahead
	REG_PC = npc;
	flushvi();
}

int vax_cpuDevice::getBit()
{
	int32_t  pos = opRegs[0];
	int32_t  reg = opRegs[1];
	uint32_t ea, src;

	if (reg >= 0) {
		if (ZXTL(pos) > 31)
			throw RSVD_OPND_FAULT;
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
		if (ZXTL(pos) > 31)
			throw RSVD_OPND_FAULT;
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

int32_t vax_cpuDevice::getField(bool sign)
{
	int32_t  pos  = SXTL(opRegs[0]);
	uint8_t  size = ZXTB(opRegs[1]);
	uint32_t reg  = ZXTL(opRegs[2]);
	uint32_t src1 = ZXTL(opRegs[3]);
	uint32_t src2, ea;

	// If size is zero, do nothing and return zero.
	if (size == 0)
		return 0;

	// If size is more than 32, reserved operand fault.
	if (size > 32)
		throw RSVD_OPND_FAULT;

	// Extract a field from one or two longwords.
	if (reg != OPR_MEM) {
		if ((ZXTL(pos) > 31) && (reg >= REG_nSP))
			throw RSVD_ADDR_FAULT;
		src2 = ZXTL(gRegs[reg+1].l);
	} else {
		ea   = src1 + (pos >> 3);
		pos  = (pos & 07) | ((ea & 03) << 3);
		ea  &= ~03;
		src1 = readv(ea, LN_LONG, RACC);
		if ((pos + size) > 32)
			src2 = readv(ea+LN_LONG, LN_LONG, RACC);
	}
	if (pos > 0)
		src1 = (src1 >> pos) | (src2 << (32-pos));

	// Zero or sign extension
//	printf("%s: %d => %08X %08X\n", devName.c_str(),
//		size, ZXTL(MSK_LONG << size), (1u << size-1));

//	src1 &= ~ZXTL(MSK_LONG << size);
//	if (sign && (src1 & (1u << size-1)))
//		src1 |= ZXTL(MSK_LONG << size);
	src1 &= mskList[size];
	if (sign && (src1 & sgnList[size]))
		src1 |= ~mskList[size];

	// Return result
	return src1;
}

void vax_cpuDevice::putField()
{
	uint32_t src  = ZXTL(opRegs[0]);
	int32_t  pos  = SXTL(opRegs[1]);
	uint8_t  size = ZXTB(opRegs[2]);
	uint32_t reg  = ZXTL(opRegs[3]);
	uint32_t src1, src2;
	uint32_t dst1, dst2;
	uint32_t ea, mask;

	// If size is zero, do nothing and return zero.
	if (size == 0)
		return;

	// If size is more than 32, reserved operand fault.
	if (size > 32)
		throw RSVD_OPND_FAULT;

	// Extract a field from one or two longwords.
	if (reg != OPR_MEM) {
		if (ZXTL(pos) > 31)
			throw RSVD_ADDR_FAULT;
		if (ZXTL(pos + size) > 32) {
			if (reg >= REG_nSP)
				throw RSVD_OPND_FAULT;
			mask = mskList[pos + size - 32];
			src2 = gRegs[reg+1].l;
			dst2 = ((src >> (32 - pos)) & mask) | (src2 & ~mask);
			gRegs[reg+1].l = dst2;
		}
		src1 = gRegs[reg].l;
		mask = mskList[size] << pos;
		dst1 = ((src << pos) & mask) | (src1 & ~mask);
		gRegs[reg].l = dst1;
	} else {
		ea   = opRegs[4] + (pos >> 3);
		pos  = (pos & 07) | ((ea & 03) << 3);
		ea  &= ~03;
		src1 = readv(ea, LN_LONG, RACC);
		if ((pos + size) > 32) {
			src2 = readv(ea+LN_LONG, LN_LONG, RACC);
			mask = mskList[pos + size - 32];
			dst2 = ((src >> (32 - pos)) & mask) | (src2 & ~mask);
			writev(ea+LN_LONG, dst2, LN_LONG, WACC);
		}
		mask = mskList[size] << pos;
		dst1 = ((src << pos) & mask) | (src1 & ~mask);
		writev(ea, dst1, LN_LONG, WACC);
//		printf("%s: Mask=%08X Pos=%d Size=%d\n", devName.c_str(),
//			mask, pos, size);
//		printf("%s: %08X => %08X\n", devName.c_str(), src1, dst1);
	}
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
		throw RSVD_OPND_FAULT;
	if (nacc == AM_KERNEL) {
		// Check validation for kernel mode
		nipl = PSL_GETIPL(npsl);
		if ((npsl & PSL_IS) && (((opsl & PSL_IS) == 0) || (nipl == 0)))
			throw RSVD_OPND_FAULT;
		if (nipl > PSL_GETIPL(opsl))
			throw RSVD_OPND_FAULT;
	} else {
		// Check validation for non-kernel mode
		if ((npsl & (PSL_IS|PSL_IPL)) || nacc > PSL_GETPRV(npsl))
			throw RSVD_OPND_FAULT;
	}
	// Check compatibility mode
	if (npsl & PSL_CM)
		throw RSVD_OPND_FAULT;

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
