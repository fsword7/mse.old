/*
 * pdp11.h
 *
 *  Created on: Mar 7, 2017
 *      Author: Timothy Stark
 */

#pragma once

#define CPU_nGREGS  8

// Register List
#define REG_nR0		0
#define REG_nR1		1
#define REG_nR2		2
#define REG_nR3		3
#define REG_nR4		4
#define REG_nR5		5
#define REG_nR6		6
#define REG_nR7		7

// PSW - Processor Status Register
//
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// | CM  | PM  |RS|0  0 |SI|   IPL  |T |N |Z |V |C |
// +--^--+--+--^--+--+--^--+--+--^--+--+--^--+--+--+
//  15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00

#define PSW_CM  0140000 // Current Mode
#define PSW_PM  0030000 // Previous Mode
#define PSW_RS  0004000 // General Register Set     (J11 only)
#define PSW_SI  0000400 // Supsended Information    (F11 only)
#define PSW_IPL 0000340 // Interrupt Priority Level
#define PSW_T   0000020 // Trace/Trap Mode
#define PSW_N   0000010 // Negative
#define PSW_Z   0000004 // Zero
#define PSW_V   0000002 // Overflow
#define PSW_C   0000001 // Carry
#define PSW_RW  0174357 // Writable Bits for J11

#define PSW_CC  (PSW_N|PSW_Z|PSW_V|PSW_C)

// Condition Code Flags
#define CC_SET 0000020 // Set bit in CC instructions
#define CC_N   PSW_N   // Negative Flag
#define CC_Z   PSW_Z   // Negative Flag
#define CC_V   PSW_V   // Overflow Flag
#define CC_C   PSW_C   // Carry Flag
#define CC_ALL (CC_N|CC_Z|CC_V|CC_C)

#define PSW_P_CM 14
#define PSW_P_PM 12
#define PSW_MODE 03

#define PSW_GETCUR(psw) ((psw >> PSW_P_CM) & PSW_MODE)
#define PSW_GETPRV(psw) ((psw >> PSW_P_PM) & PSW_MODE)

// Access Mode (Protection Codes)
#define AM_KERNEL  0  // Kernel mode
#define AM_SUPER   1  // Supervisor mode
#define AM_ILLEGAL 2  // Illegal mode
#define AM_USER    3  // User mode

// Memory Management Registers
#define MMR_nREGS  4

// APR File Registers
#define APR_nREGS  64

class pdp11_cpuDevice
{
public:
	pdp11_cpuDevice() {};
	~pdp11_cpuDevice() {};

protected:
	uint16_t wkRegs[CPU_nGREGS];  // Working registers (R0-R7, SP and PC)
	uint16_t gpRegs[6][2];        // register set (R0-R5, R0'-R5')
	uint16_t stkRegs[4];          // Stack registers (KSP, SSP, and USP)

	// Memory management registers
	uint16_t mmRegs[MMR_nREGS];   // Memory management registers (MMR0-MMR3)
	uint16_t apRegs[APR_nREGS];   // APR file registers
};
