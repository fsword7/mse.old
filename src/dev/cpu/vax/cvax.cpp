/*
 * cvax.cpp
 *
 *  Created on: Mar 12, 2017
 *      Author: Timothy Stark
 *
 *  CVAX Processor emulation
 *
 */

#include "emu/core.h"
#include "emu/devcpu.h"
#include "dev/cpu/vax/mtpr.h"
#include "dev/cpu/vax/vax.h"
#include "dev/cpu/vax/cvax.h"

#if 0
static int   regSize = 0x40;
static char *regNames[] = {
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
#endif

cvax_cpuDevice::cvax_cpuDevice()
{
}

cvax_cpuDevice::~cvax_cpuDevice()
{
}

void cvax_cpuDevice::reset()
{
	pRegs[PR_SID] = (SID_ID|SID_UCODE);
}

#define CPU_CVAX
#define CPU_CLASS cvax_cpuDevice
#include "dev/cpu/vax/executes.h"

#if 0
// Read Privileged Register
int32 cvax_cpuDevice::readRegister(int32 pReg)
{
	int32 data;

	switch (pReg) {
		case PR_KSP:
			data = (PSL & PSL_IS) ? KSP : SP;
			break;

		case PR_ISP:
			data = (PSL & PSL_IS) ? SP : ISP;
			break;

		case PR_IPL:
			IPL = data = PSL_GETIPL(PSL);
			break;

		case PR_RXCS:
			data = vax_ReadRXCS(vax);
			break;

		case PR_RXDB:
			data = vax_ReadRXDB(vax);
			break;

		case PR_TXCS:
			data = vax_ReadTXCS(vax);
			break;

//		case PR_TXDB:
//			data = vax_ReadTXDB(vax);
//			break;

		default:
			data = PRN(pReg);
			break;
	}

#ifdef DEBUG
	if (dbg_Check(DBG_TRACE|DBG_DATA)) {
		char *name = "Undefined Register";
		if ((pReg < regSize) && regNames[pReg])
			name = regNames[pReg];
		dbg_Printf("KA650: (R) %s (%02X) => %08X\n", name, pReg, data);
	}
#endif /* DEBUG */

	return data;
}

void cvax_cpuDevice::writeRegister(int32 pReg, int32 data)
{
	switch (pReg) {
		case PR_KSP: // Kernel Stack Pointer
			((PSL & PSL_IS) ? KSP : SP) = data;
			break;

		case PR_ESP: // Executive Stack Pointer
		case PR_SSP: // Supervisor Stack Pointer
		case PR_USP: // User Stack Pointer
			PRN(pReg) = data;
			break;

		case PR_ISP: // Interrupt Stack Pointer
			((PSL & PSL_IS) ? SP : ISP) = data;
			break;

		case PR_P0BR:
		case PR_P1BR:
		case PR_SBR:
			PRN(pReg) = data & BR_MASK;
			vax_ClearTBTable(vax, pReg == PR_SBR);
			break;

		case PR_P0LR:
		case PR_P1LR:
		case PR_SLR:
			PRN(pReg) = data & LR_MASK;
			vax_ClearTBTable(vax, pReg == PR_SLR);
			break;

		case PR_PCBB:
		case PR_SCBB:
			PRN(pReg) = data & LALIGN;
			break;

		case PR_IPL:
			IPL = data & PSL_M_IPL;
			PSL = PSL_PUTIPL(IPL) | (PSL & ~PSL_IPL);
			break;

		case PR_SIRR:
			if ((data > 0xF) || (data == 0))
				RSVD_OPND_FAULT;
			SISR |= (1 << data);
			break;

		case PR_SISR:
			SISR = data & SISR_MASK;
			break;

		case PR_ASTLVL:
			if ((uint32)data > AST_MAX)
				RSVD_OPND_FAULT;
			ASTLVL = data;
			break;

		case PR_ICCS:
			// Subset implementation in MicroVAX series
			ICCS = data & ICCS_WMASK;
#ifdef DEBUG
			if (dbg_Check(DBG_INTERRUPT))
				dbg_Printf("KA650: Clock Interrupt Enable: %s\n",
					(data & ICCS_IE) ? "On" : "Off");
#endif /* DEBUG */
			break;

		case PR_RXCS:
//			if (data & RXCS_MBZ)
//				RSVD_OPND_FAULT;
			vax_WriteRXCS(vax, data);
			break;

		case PR_RXDB:
//			vax_WriteRXDB(vax, data);
			break;

		case PR_TXCS:
//			if (data & TXCS_MBZ)
//				RSVD_OPND_FAULT;
			vax_WriteTXCS(vax, data);
			break;

		case PR_TXDB:
			vax_WriteTXDB(vax, data);
			break;

		case PR_CADR:
			PRN(pReg) = (data & CADR_RW) | CADR_MBO;
			break;

		case PR_MSER:
			PRN(pReg) &= MSER_HM;
			break;

		case PR_CONPC:
		case PR_CONPSL:
			PRN(pReg) = data;
			break;

		case PR_IORESET:
			cq_ResetAll(((KA650_DEVICE *)vax)->qba);
			break;

		case PR_MAPEN:
			MAPEN = data & 1;
		case PR_TBIA:
			vax_ClearTBTable(vax, 1);
			break;

		case PR_TBIS:
			vax_ClearTBEntry(vax, data);
			break;

		case PR_TBCHK:
			if (vax_CheckTBEntry(vax, data))
				CC |= CC_V;
			break;
	}

#ifdef DEBUG
	if (dbg_Check(DBG_TRACE|DBG_DATA)) {
		char *name = "Undefined Register";
		if ((pReg < regSize) && regNames[pReg])
			name = regNames[pReg];
		dbg_Printf("KA650: (W) %s (%02X) <= %08X\n", name, pReg, data);
	}
#endif /* DEBUG */
}
#endif

