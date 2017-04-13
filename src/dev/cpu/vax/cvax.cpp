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

cvax_cpuDevice *cvax_cpuDevice::create(std::string devName)
{
	cvax_cpuDevice *cpu = new cvax_cpuDevice();

	if (cpu == nullptr)
		return nullptr;

	cpu->devName = devName;
	cpu->devType = "CVAX";
//	cpu->devDesc = model->desc;
//	cpu->driver  = model->driver;

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
}

int cvax_cpuDevice::boot()
{
	return 0;
}

#define CPU_CVAX
#define CPU_CLASS cvax_cpuDevice
#include "dev/cpu/vax/executes.h"

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
//			vax_ClearTBTable(vax, pReg == PR_SBR);
			break;

		case IPR_nP0LR:
		case IPR_nP1LR:
		case IPR_nSLR:
			ipReg[rn] = data & LR_MASK;
//			vax_ClearTBTable(vax, pReg == PR_SLR);
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
//			pRegs[rn] = data & 1;
		case IPR_nTBIA:
//			vax_ClearTBTable(vax, 1);
			break;

		case IPR_nTBIS:
//			vax_ClearTBEntry(vax, data);
			break;

		case IPR_nTBCHK:
//			if (vax_CheckTBEntry(vax, data))
//				CC |= CC_V;
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

