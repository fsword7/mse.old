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

static int   regSize = 0x40;
static const char *regNames[] = {
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
	for (int idx = 0; idx < VAX_nGREGS; idx++)
		gRegs[idx].l = 0;

	// Powerup initialization
	psReg  = PSL_IS | PSL_IPL;
	ccReg  = 0;
	REG_SP = 0x00000000;
	REG_PC = 0; /* ROM_BASE */

	PREG_SID    = (SID_ID|SID_UCODE);
	PREG_CONPC  = 0;
	PREG_CONPSL = PSL_IS | PSL_IPL | CON_PWRUP;
	PREG_MAPEN  = 0;
	PREG_ICCS   = 0;
	PREG_MSER   = 0;
	PREG_CADR   = 0;
	PREG_SISR   = 0;
	PREG_ASTLVL = 0;

	// Set 30-bit physical addressing mask
	paMask  = PA_MASK30;
	// Reset current access mode
	curMode = ACC_MASK(PSL_GETCUR(psReg));
}

int cvax_cpuDevice::boot()
{
	return 0;
}

#define CPU_CVAX
#define CPU_CLASS cvax_cpuDevice
#include "dev/cpu/vax/executes.h"

// Read Privileged Register
uint32_t cvax_cpuDevice::readpr(uint32_t pReg)
{
	uint32_t data;

	switch (pReg) {
		case PR_KSP:
			data = (psReg & PSL_IS) ? pRegs[PR_KSP] : gRegs[REG_nSP].l;
			break;

		case PR_ISP:
			data = (psReg & PSL_IS) ? gRegs[REG_nSP].l : pRegs[PR_ISP];
			break;

		case PR_IPL:
			data = PSL_GETIPL(psReg);
//			pRegs[PR_IPL] = data;
			break;

		case PR_RXCS:
//			data = vax_ReadRXCS(vax);
			break;

		case PR_RXDB:
//			data = vax_ReadRXDB(vax);
			break;

		case PR_TXCS:
//			data = vax_ReadTXCS(vax);
			break;

//		case PR_TXDB:
//			data = vax_ReadTXDB(vax);
//			break;

		default:
			data = (pReg < VAX_nPREGS) ? pRegs[pReg] : 0;
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
	if ((pReg < regSize) && regNames[pReg])
		name = regNames[pReg];
	printf("%s: (R) %s (%02X) => %08X\n", devName.c_str(), name, pReg, data);

	return data;
}

void cvax_cpuDevice::writepr(uint32_t pReg, uint32_t data) //throw(uint32_t)
{
	switch (pReg) {
		case PR_KSP: // Kernel Stack Pointer
			if (psReg & PSL_IS)
				pRegs[PR_KSP] = data;
			else
				gRegs[REG_nSP].l = data;
			break;

		case PR_ESP: // Executive Stack Pointer
		case PR_SSP: // Supervisor Stack Pointer
		case PR_USP: // User Stack Pointer
			pRegs[pReg] = data;
			break;

		case PR_ISP: // Interrupt Stack Pointer
			if (psReg & PSL_IS)
				gRegs[REG_nSP].l = data;
			else
				pRegs[PR_ISP] = data;
			break;

		case PR_P0BR:
		case PR_P1BR:
		case PR_SBR:
			pRegs[pReg] = data & BR_MASK;
//			vax_ClearTBTable(vax, pReg == PR_SBR);
			break;

		case PR_P0LR:
		case PR_P1LR:
		case PR_SLR:
			pRegs[pReg] = data & LR_MASK;
//			vax_ClearTBTable(vax, pReg == PR_SLR);
			break;

		case PR_PCBB:
		case PR_SCBB:
			pRegs[pReg] = data & ALIGN_LONG;
			break;

		case PR_IPL:
			pRegs[PR_IPL] = data & PSL_M_IPL;
			psReg &= ~PSL_IPL;
			psReg |= PSL_SETIPL(data);
			break;

		case PR_SIRR:
//			if ((data > 0xF) || (data == 0))
//				throw EXC_RSVD_OPND_FAULT;
			pRegs[PR_SIRR] |= (1 << data);
			break;

		case PR_SISR:
			pRegs[PR_SISR] = data & SISR_MASK;
			break;

		case PR_ASTLVL:
//			if ((uint32)data > AST_MAX)
//				throw EXC_RSVD_OPND_FAULT;
			pRegs[PR_ASTLVL] = data;
			break;

		case PR_ICCS:
			// Subset implementation in MicroVAX series
//			pRegs[PR_ICCS] = data & ICCS_WMASK;
//#ifdef DEBUG
//			if (dbg_Check(DBG_INTERRUPT))
//				dbg_Printf("KA650: Clock Interrupt Enable: %s\n",
//					(data & ICCS_IE) ? "On" : "Off");
//#endif /* DEBUG */
			break;

		case PR_RXCS:
//			if (data & RXCS_MBZ)
//				throw EXC_RSVD_OPND_FAULT;
//			vax_WriteRXCS(vax, data);
			break;

		case PR_RXDB:
//			vax_WriteRXDB(vax, data);
			break;

		case PR_TXCS:
//			if (data & TXCS_MBZ)
//				throw EXC_RSVD_OPND_FAULT;
//			vax_WriteTXCS(vax, data);
			break;

		case PR_TXDB:
//			vax_WriteTXDB(vax, data);
			break;

		case PR_CADR:
//			pRegs[pReg] = (data & CADR_RW) | CADR_MBO;
			break;

		case PR_MSER:
//			pRegs[pReg] &= MSER_HM;
			break;

		case PR_CONPC:
		case PR_CONPSL:
//			pRegs[pReg] = data;
			break;

		case PR_IORESET:
//			cq_ResetAll(((KA650_DEVICE *)vax)->qba);
			break;

		case PR_MAPEN:
//			pRegs[PR_MAPEN] = data & 1;
		case PR_TBIA:
//			vax_ClearTBTable(vax, 1);
			break;

		case PR_TBIS:
//			vax_ClearTBEntry(vax, data);
			break;

		case PR_TBCHK:
//			if (vax_CheckTBEntry(vax, data))
//				CC |= CC_V;
			break;
	}

//#ifdef DEBUG
//	if (dbg_Check(DBG_TRACE|DBG_DATA)) {
//		char *name = "Undefined Register";
//		if ((pReg < regSize) && regNames[pReg])
//			name = regNames[pReg];
//		dbg_Printf("KA650: (W) %s (%02X) <= %08X\n", name, pReg, data);
//	}
//#endif /* DEBUG */

	const char *name = "Undefined Register";
	if ((pReg < regSize) && regNames[pReg])
		name = regNames[pReg];
	printf("%s: (W) %s (%02X) <= %08X\n", devName.c_str(), name, pReg, data);
}

