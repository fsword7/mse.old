/*
 * cvax.cpp
 *
 *  Created on: Mar 12, 2017
 *      Author: Timothy Stark
 *
 *  CVAX 78034 Processor emulation
 *
 */

#include "emu/core.h"
#include "emu/debug.h"
#include "emu/devsys.h"
#include "emu/devcpu.h"
#include "dev/cpu/vax/mtpr.h"
#include "dev/cpu/vax/vax.h"
#include "dev/cpu/vax/cvax.h"
#include "dev/cpu/vax/opcodes.h"

static int   iprSize = 0x40;
static const char *iprName[] = {
	"KSP",     // (R/W) 00 Kernel Stack Pointer
	"ESP",     // (R/W) 01 Executive Stack Pointer
	"SSP",     // (R/W) 02 Supervisor Stack Pointer
	"USP",     // (R/W) 03 User Stack Pointer
	"ISP",     // (R/W) 04 Interrupt Stack Pointer
	nullptr,   //       05 Unknown
	nullptr,   //       06 Unknown
	nullptr,   //       07 Unknown
	"P0BR",    // (R/W) 08 P0 Base Register
	"P0LR",    // (R/W) 09 P0 Length Register
	"P1BR",    // (R/W) 0A P1 Base Register
	"P1LR",    // (R/W) 0B P1 Length Register
	"SBR",     // (R/W) 0C System Base Register
	"SLR",     // (R/W) 0D System Length Register
	nullptr,   //       0E Unknown
	nullptr,   //       0F Unknown
	"PCBB",    // (R/W) 10 Process Control Block Base
	"SCBB",    // (R/W) 11 System Control Block Base
	"IPL",     // (R/W) 12 Interrupt Priority Level
	"ASTLVL",  // (R/W) 13 AST Level
	"SIRR",    // (W)   14 Software Interrupt Request
	"SISR",    // (R/W) 15 Software Interrupt Summary
	nullptr,   //       16 Unknown
	nullptr,   //       17 Unknown
	"ICCS",    // (R/W) 18 Interval Clock Control
	"NICR",    // (W)   19 Next Interval Count
	"ICR",     // (R)   1A Interval Count
	"TODR",    // (R/W) 1B Time of Year
	nullptr,   //       1C Unknown
	nullptr,   //       1D Unknown
	nullptr,   //       1E Unknown
	nullptr,   //       1F Unknown
	"RXCS",    // (R/W) 20 Console Receiver Status
	"RXDB",    // (R)   21 Console Receiver Data Buffer
	"TXCS",    // (R/W) 22 Console Transmit Status
	"TXDB",    // (W)   23 Console Transmit Data Buffer
	nullptr,   //       24 Unknown
	"CADR",    //       25 Cache Disable Register
	nullptr,   //       26 Unknown
	"MSER",    //       27 Memory System Error Register
	"ACCS",    //       28 *Accelerator Control and Status
	nullptr,   //       29 *Accelerator Maintenance
	"CONPC",   //       2A Console PC
	"CONPSL",  //       2B Console PSL
	nullptr,   //       2C *Writable-Control-Store Address
	nullptr,   //       2D *Writable-Control-Store Data
	nullptr,   //       2E Unknown
	nullptr,   //       2F Unknown
	"SBIFS",   //       30 *SBI Fault Status
	"SBIS",    //       31 *SBI silo
	"SBISC",   //       32 *SBI silo comparator
	"SBIMT",   //       33 *SBI Maintenance
	"SBIER",   //       34 *SBI Error
	"SBITA",   //       35 *SBI Timeout Address
	"SBIQC",   //       36 *SBI Quadword Clear
	nullptr,   //       37 Unknown
	"MAPEN",   // (R/W) 38 Memory Management Enable
	"TBIA",    // (W)   39 Translation Buffer Invalidate All
	"TBIS",    // (W)   3A Translation Buffer Invalidate Single
	nullptr,   //       3B Unknown
	"MBRK",    //       3C Microprogram Breakpoint
	"PME",     // (R/W) 3D Performance Monitor Enable
	"SID",     // (R)   3E System Identification
	"TBCHK",   // (W)   3F Translation Buffer Check
};


cvax_cpuDevice::cvax_cpuDevice()
{
}

cvax_cpuDevice::~cvax_cpuDevice()
{
}

cvax_cpuDevice *cvax_cpuDevice::create(sysDevice *sdev, std::string devName)
{
	cvax_cpuDevice *cpu = new cvax_cpuDevice();

	if (cpu == nullptr)
		return nullptr;

	cpu->devName = devName;
	cpu->devType = "CVAX";
//	cpu->devDesc = model->desc;
//	cpu->driver  = model->driver;

	// Assign system device for I/O access
	cpu->setSystemDevice(sdev);

	// Initialize CPU processor
	cpu->reset();

	// Add CPU device to system device
	sdev->addCPUDevice(cpu);

	return cpu;
}

void cvax_cpuDevice::reset()
{
	// Initialize all working registers
	for (int idx = 0; idx < CPU_nGREGS; idx++)
		gpReg[idx].l = 0;

	// Powerup initialization
	psReg  = PSL_IS | PSL_IPL;
	ccReg  = 0;
	REG_SP = 0x00000000;
	REG_PC = 0; /* ROM_BASE */

	IPR_SID    = (SID_ID|SID_UCODE);
	IPR_CONPC  = 0;
	IPR_CONPSL = PSL_IS | PSL_IPL | CON_PWRUP;
	IPR_MAPEN  = 0;
	IPR_ICCS   = 0;
	IPR_MSER   = 0;
	IPR_CADR   = 0;
	IPR_SISR   = 0;
	IPR_ASTLVL = 0;

	// Set 30-bit physical addressing mask
	paMask  = PA_MASK30;
	// Reset current access mode
	curMode  = AM_MASK(PSL_GETCUR(psReg));
	irqFlags = 0;
	scbCode  = -1;

	// Clear all process/system TLB entries
	cleartlb(true);
}

int cvax_cpuDevice::boot()
{
	return 0;
}

#define CPU_CVAX
#define CPU_CLASS cvax_cpuDevice
#include "dev/cpu/vax/executes.h"

void cvax_cpuDevice::mfpr()
{
	uint32_t ipr = opReg[0];
	uint32_t dst;

	// Must be in kernel mode.
	if (PSL_GETCUR(psReg) != AM_KERNEL)
		throw PRIV_INST_FAULT;

	// Standard IPR Registers
	switch (ipr) {
		case IPR_nKSP: // Kernel Stack Register
			dst = ((psReg & PSL_IS) ? ipReg[ipr] : REG_SP);
			break;

		case IPR_nESP: // Executive Stack Register
		case IPR_nSSP: // Supervisor Stack Register
		case IPR_nUSP: // User Stack Register
			dst = ipReg[ipr];
			break;

		case IPR_nISP: // Interrupt Stack Register
			dst = ((psReg & PSL_IS) ? REG_SP : ipReg[ipr]);
			break;

		case IPR_nIPL: // Interrupt Priority Level
			dst = PSL_GETIPL(psReg);
			break;

		case IPR_nPME:
			dst = ipReg[ipr];
			break;

		default:
			dst = (ipr < iprSize) ? ipReg[ipr] : 0;
			// Model-Specific IPR Registers
			// Otherwise, reserved operand fault trap.
//			if (cpu->ReadIPR)
//				dst = cpu->ReadIPR(cpu, ipr);
			break;
	}

	// Write results back.
	storel(opReg[1], dst);

	// Update Condition Codes
	SetNZ(ccReg, SXTL(dst), 0, (ccReg & CC_C));

	// Evaluate hardware/software interrupts.
	UpdateIRQ();

#ifdef DEBUG
//	if (DBG_CHECKALL(cpu, LOG_TRACE|LOG_DATA) || DBG_CHECKANY(cpu, LOG_IOREGS))
//	if (DBG_CHECKALL(cpu, LOG_TRACE|LOG_DATA))
//		PrintLog3(LOG_TRACE|LOG_IOREGS, NULL,
//			"%s: (MFPR) IPR %04X (%s) => %08X: %s\n",
//				IO_DEVNAME(cpu), ipr, IPR_NAME(ipr), dst, ShowCC(VAX_CC));
#endif /* DEBUG */

	const char *name = "Undefined Register";
	if ((ipr < iprSize) && iprName[ipr])
		name = iprName[ipr];
	printf("%s: (R) %s (%02X) => %08X: %s\n", devName.c_str(),
		name, ipr, dst, stringCC(ccReg));
}

void cvax_cpuDevice::mtpr()
{
	uint32_t src = opReg[0];
	uint32_t ipr = opReg[1];
	uint32_t irq;

	// Must be in kernel mode.
	if (PSL_GETCUR(psReg) != AM_KERNEL)
		throw PRIV_INST_FAULT;

	// Update Condition Codes
	SetNZ(ccReg, SXTL(src), 0, (ccReg & CC_C));

	// Update Standard IPR Registers
	switch (ipr) {
		case IPR_nKSP: // Kernel Stack Register
			if (psReg & PSL_IS)
				ipReg[ipr] = src;
			else
				gpReg[REG_nSP].l = src;
			break;
		case IPR_nESP: // Executive Stack Register
		case IPR_nSSP: // Supervisor Stack Register
		case IPR_nUSP: // User Stack Register
			ipReg[ipr] = src;
			break;
		case IPR_nISP: // Interrupt Stack Register
			if (psReg & PSL_IS)
				gpReg[REG_nSP].l = src;
			else
				ipReg[ipr] = src;
			break;

		case IPR_nP0BR:
		case IPR_nP1BR:
		case IPR_nSBR:
			ipReg[ipr] = src & BR_MASK;
			cleartlb(ipr == IPR_nSBR);
			break;

		case IPR_nP0LR:
		case IPR_nP1LR:
		case IPR_nSLR:
			ipReg[ipr] = src & LR_MASK;
			cleartlb(ipr == IPR_nSLR);
			break;

		case IPR_nPCBB:
		case IPR_nSCBB:
			ipReg[ipr] = src & ALIGN_LONG;
			break;

		case IPR_nIPL:
			ipReg[ipr] = src & IPL_MASK;
			psReg = PSL_SETIPL(src) | (psReg & ~PSL_IPL);
			break;

		case IPR_nSIRR:
//			if ((src > IPL_SMAX) || (src == 0))
//				RSVD_OPND_FAULT;
			if (irq = (src & SIRR_MASK))
				IPR_SISR |= (1u << irq);
			break;

		case IPR_nSISR:
			ipReg[ipr] = src & SISR_MASK;
			break;

		case IPR_nASTLVL:
			if (src > AST_MAX)
				throw RSVD_OPND_FAULT;
			ipReg[ipr] = src;
			break;

		case IPR_nMAPEN:
			ipReg[ipr] = src & 1;
		case IPR_nTBIA:
			cleartlb(true);
			break;

		case IPR_nTBIS:
			cleartlb(src);
			break;

		case IPR_nTBCHK:
			if (checktlb(src))
				ccReg |= CC_V;
			break;

		case IPR_nPME:
			ipReg[ipr] = src & 1;
			break;

		default:
			// Update Model-Specific IPR Registers
			// Otherwise, reserved operand fault trap.
//			cpu->WriteIPR(cpu, ipr, src);
			break;
	}

	// Evaluate hardware/software interrupts.
	UpdateIRQ();

#ifdef DEBUG
//	if (DBG_CHECKALL(cpu, LOG_TRACE|LOG_DATA) || DBG_CHECKANY(cpu, LOG_IOREGS))
//	if (DBG_CHECKALL(cpu, LOG_TRACE|LOG_DATA))
//		PrintLog3(LOG_TRACE|LOG_IOREGS, NULL,
//			"%s: (MTPR) IPR %04X (%s) <= %08X (Now: %08X): %s\n",
//				IO_DEVNAME(cpu), ipr, IPR_NAME(ipr), src, DPY_IPR(ipr),
//				ShowCC(VAX_CC));
#endif /* DEBUG */

	const char *name = "Undefined Register";
	if ((ipr < iprSize) && iprName[ipr])
		name = iprName[ipr];
	printf("%s: (W) %s (%02X) <= %08X: %s\n", devName.c_str(),
		name, ipr, src, stringCC(ccReg));
}

// Read Privileged Register
uint32_t cvax_cpuDevice::readpr(uint32_t rn)
{
	uint32_t data;

	switch (rn) {
		case IPR_nKSP:
			data = (psReg & PSL_IS) ? ipReg[IPR_nKSP] : gpReg[REG_nSP].l;
			break;

		case IPR_nISP:
			data = (psReg & PSL_IS) ? gpReg[REG_nSP].l : ipReg[IPR_nISP];
			break;

		case IPR_nIPL:
			data = PSL_GETIPL(psReg);
//			ipReg[IPR_nIPL] = data;
			break;

		case IPR_nRXCS:
//			data = vax_ReadRXCS(vax);
			break;

		case IPR_nRXDB:
//			data = vax_ReadRXDB(vax);
			break;

		case IPR_nTXCS:
//			data = vax_ReadTXCS(vax);
			break;

//		case PR_nTXDB:
//			data = vax_ReadTXDB(vax);
//			break;

		default:
			data = (rn < CPU_nPREGS) ? ipReg[rn] : 0;
			break;
	}

//#ifdef DEBUG
//	if (dbg_Check(DBG_TRACE|DBG_DATA)) {
//		char *name = "Undefined Register";
//		if ((pReg < regSize) && regNames[pReg])
//			name = regNames[pReg];
//		dbg_Printf("KA650: (R) %s (%02X) => %08X\n", name, pReg, data);
//	}
//#endif /* DEBUG */

	const char *name = "Undefined Register";
	if ((rn < iprSize) && iprName[rn])
		name = iprName[rn];
	printf("%s: (R) %s (%02X) => %08X\n", devName.c_str(), name, rn, data);

	return data;
}

void cvax_cpuDevice::writepr(uint32_t rn, uint32_t data)
{
	uint32_t irq;

	switch (rn) {
		case IPR_nKSP: // Kernel Stack Pointer
			if (psReg & PSL_IS)
				ipReg[IPR_nKSP] = data;
			else
				gpReg[REG_nSP].l = data;
			break;

		case IPR_nESP: // Executive Stack Pointer
		case IPR_nSSP: // Supervisor Stack Pointer
		case IPR_nUSP: // User Stack Pointer
			ipReg[rn] = data;
			break;

		case IPR_nISP: // Interrupt Stack Pointer
			if (psReg & PSL_IS)
				gpReg[REG_nSP].l = data;
			else
				ipReg[IPR_nISP] = data;
			break;

		case IPR_nP0BR:
		case IPR_nP1BR:
		case IPR_nSBR:
			ipReg[rn] = data & BR_MASK;
			cleartlb(rn == IPR_nSBR);
			break;

		case IPR_nP0LR:
		case IPR_nP1LR:
		case IPR_nSLR:
			ipReg[rn] = data & LR_MASK;
			cleartlb(rn == IPR_nSLR);
			break;

		case IPR_nPCBB:
		case IPR_nSCBB:
			ipReg[rn] = data & ALIGN_LONG;
			break;

		case IPR_nIPL:
			ipReg[rn] = data & PSL_M_IPL;
			psReg &= ~PSL_IPL;
			psReg |= PSL_SETIPL(data);
			break;

		case IPR_nSIRR:
//			if ((data > 0xF) || (data == 0))
//				throw RSVD_OPND_FAULT;
			if (irq = (data & SIRR_MASK))
				IPR_SISR |= (1u << irq);
			break;

		case IPR_nSISR:
			ipReg[rn] = data & SISR_MASK;
			break;

		case IPR_nASTLVL:
			if (ZXTL(data) > AST_MAX)
				throw RSVD_OPND_FAULT;
			ipReg[rn] = data;
			break;

		case IPR_nICCS:
			// Subset implementation in MicroVAX series
//			pRegs[PR_ICCS] = data & ICCS_WMASK;
//#ifdef DEBUG
//			if (dbg_Check(DBG_INTERRUPT))
//				dbg_Printf("KA650: Clock Interrupt Enable: %s\n",
//					(data & ICCS_IE) ? "On" : "Off");
//#endif /* DEBUG */
			break;

		case IPR_nRXCS:
//			if (data & RXCS_MBZ)
//				throw RSVD_OPND_FAULT;
//			vax_WriteRXCS(vax, data);
			break;

		case IPR_nRXDB:
//			vax_WriteRXDB(vax, data);
			break;

		case IPR_nTXCS:
//			if (data & TXCS_MBZ)
//				throw RSVD_OPND_FAULT;
//			vax_WriteTXCS(vax, data);
			break;

		case IPR_nTXDB:
//			vax_WriteTXDB(vax, data);
			break;

		case IPR_nCADR:
//			pRegs[rn] = (data & CADR_RW) | CADR_MBO;
			break;

		case IPR_nMSER:
//			pRegs[rn] &= MSER_HM;
			break;

		case IPR_nCONPC:
		case IPR_nCONPSL:
//			pRegs[rn] = data;
			break;

		case IPR_nIORESET:
//			cq_ResetAll(((KA650_DEVICE *)vax)->qba);
			break;

		case IPR_nMAPEN:
			ipReg[rn] = data & 1;
		case IPR_nTBIA:
			cleartlb(true);
			break;

		case IPR_nTBIS:
			cleartlb(data);
			break;

		case IPR_nTBCHK:
			if (checktlb(data))
				ccReg |= CC_V;
			break;
	}

	// Evaluate hardware/software interrupts
	UpdateIRQ();

//#ifdef DEBUG
//	if (dbg_Check(DBG_TRACE|DBG_DATA)) {
//		char *name = "Undefined Register";
//		if ((pReg < regSize) && regNames[pReg])
//			name = regNames[pReg];
//		dbg_Printf("KA650: (W) %s (%02X) <= %08X\n", name, pReg, data);
//	}
//#endif /* DEBUG */

	const char *name = "Undefined Register";
	if ((rn < iprSize) && iprName[rn])
		name = iprName[rn];
	printf("%s: (W) %s (%02X) <= %08X\n", devName.c_str(), name, rn, data);
}

