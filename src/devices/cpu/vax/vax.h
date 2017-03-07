/*
 * vax.h
 *
 *  Created on: Feb 26, 2017
 *      Author: fswor
 */

#pragma once

#define VAX_nGREGS  16
#define VAX_nPREGS  256
#define VAX_nOPREGS 9

// Register List
#define REG_nR0		0
#define REG_nR1		1
#define REG_nR2		2
#define REG_nR3		3
#define REG_nR4		4
#define REG_nR5		5
#define REG_nR6		6
#define REG_nR7		7
#define REG_nR8		8
#define REG_nR9		9
#define REG_nR10	10
#define REG_nR11	11
#define REG_nR12	12
#define REG_nR13	13
#define REG_nR14	14
#define REG_nR15	15

#define REG_nAP		REG_nR12 // Argument Pointer
#define REG_nFP		REG_nR13 // Frame Pointer
#define REG_nSP		REG_nR14 // Stack Pointer
#define REG_nPC		REG_nR15 // Program Counter


// Processor Status Register definition
//
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---~
// |CM |TP | 0   0 |FP |IS |  CUR  |  PRV  | 0 |        IPL        |
// +---+---+---+---^---+---+---+---^---+---+---+---^---+---+---+---~
//  31  30  29  28  27  26  25  24  23  22  21  20  19  18  17  16
//
//     ~---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
//     | 0   0   0   0   0   0   0   0 |DV |FU |IV | T | N | Z | V | C |
//     ~---+---+---+---^---+---+---+---^---+---+---+---^---+---+---+---+
//      15  14  13  12  11  10  09  08  07  06  05  04  03  02  01  00
//

// PSL - Processor Status Longword for Kernel mode
#define PSL_MBZ			0x3020FF00 // Must Be Zero
#define PSL_CM			0x80000000 // PDP-11 Compatibility mode
#define PSL_TP			0x40000000 // Trace Pending
#define PSL_FPD			0x08000000 // First Part Done
#define PSL_IS			0x04000000 // Interrupt Stack
#define PSL_CUR			0x03000000 // Current Mode
#define PSL_PRV			0x00C00000 // Previous Mode
#define PSL_IPL			0x001F0000 // Interrupt Priority Level

// PSW - Processor Status Word for User mode
#define PSW_MASK        0xFFFF // PSW User Mask
#define PSW_DV			0x0080 // Decimal Overflow Trap Enable
#define PSW_FU			0x0040 // Floating Underflow Trap Enable
#define PSW_IV			0x0020 // Integer Overflow Trap Enable
#define PSW_T			0x0010 // Trace Enable
#define PSW_CC			0x000F // Condition Codes

// CC - Condition Codes (part of PSW word)
#define CC_N			0x0008 // Negative
#define CC_Z			0x0004 // Zero
#define CC_V			0x0002 // Overflow
#define CC_C			0x0001 // Carry
#define CC_MASK			(CC_N|CC_Z|CC_V|CC_C)

// Access Mode (Protection Codes)
#define AM_KERNEL		0 // Kernel mode for OS kernel
#define AM_EXECUTIVE	1 // Executive mode for file system
#define AM_SUPERVISOR	2 // Supervisor mode for DCL (shell)
#define AM_USER			3 // User mode for normal programs


class vax_cpuDevice : public cpuDevice
{
public:
	vax_cpuDevice() {};
	~vax_cpuDevice() {};

protected:
	uint32_t gRegs[VAX_nGREGS]; // General registers
	uint32_t pRegs[VAX_nPREGS]; // Processor registers
};
