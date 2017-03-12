/*
 * ibm3x0.h
 *
 *  Created on: Feb 26, 2017
 *      Author: Timothy Stark
 *
 *  Emulates System/360, System/370, System/380, ESA/390 and z900 processor
 *
 */

#pragma once

#define  I3x0_nGREGS  16 // General registers
#define  I3x0_nAREGS  16 // Access registers
#define  I3x0_nFREGS  16 // Floating registers
#define  I3x0_nCREGS  16 // Control registers

// PSW - Processor Status Word definition
//
// System/360 (Basic Code)
//
//  0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |  System Mask  |  Key  |A|M|W|P|       Interruption Code       |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |ILC|CC |Pgm Msk|         Instruction Address                   |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 3 3 3 3 3 3 4 4 4 4 4 4 4 4 4 4 5 5 5 5 5 5 5 5 5 5 6 6 6 6
//  2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
//
// System/370 (Basic Code)
//
//  0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |  System Mask  |  Key  |E|M|W|P|       Interruption Code       |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |ILC|CC |Pgm Msk|         Instruction Address                   |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 3 3 3 3 3 3 4 4 4 4 4 4 4 4 4 4 5 5 5 5 5 5 5 5 5 5 6 6 6 6
//  2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
//
// System/370 (Extended Code)
// ESA/390 (Extended Code)
//
//  0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |0|R|0 0 0|T|I|E|  Key  |1|M|W|P|AS |CC |Pgm Msk|0 0 0 0 0 0 0 0|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |A|                   Instruction Address                       |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 3 3 3 3 3 3 4 4 4 4 4 4 4 4 4 4 5 5 5 5 5 5 5 5 5 5 6 6 6 6
//  2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
//
// z900 (Extended Code)
//
//  0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |0|R|0 0 0|T|I|E|  Key  |1|M|W|P|AS |CC |Pgm Msk|0 0 0 0 0 0 0|E|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |B|0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0|
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
// |                                                               |
// +-+-+-                Instruction Address                  -+-+-+
// |                                                               |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 3 3 3 3 3 3 4 4 4 4 4 4 4 4 4 4 5 5 5 5 5 5 5 5 5 5 6 6 6 6
//  2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3

// PSW - Basic Code
#define PSW_BC_SMASK   0xFF00000000000000LL
#define PSW_BC_KEY     0x00F0000000000000LL
#define PSW_BC_STATE   0x000F000000000000LL
#define PSW_BC_INTCODE 0x0000FFFF00000000LL
#define PSW_BC_ILC     0x00000000C0000000LL
#define PSW_BC_CC      0x0000000030000000LL
#define PSW_BC_PMASK   0x000000000F000000LL
#define PSW_BC_IA      0x0000000000FFFFFFLL

// PSW - Extended Code
#define PSW_EC_SMASK   0xFF00000000000000LL
#define PSW_EC_KEY     0x00F0000000000000LL
#define PSW_EC_STATE   0x000F000000000000LL
#define PSW_EC_ASC     0x0000C00000000000LL
#define PSW_EC_CC      0x0000300000000000LL
#define PSW_EC_PMASK   0x00000F0000000000LL
#define PSW_EC_A31     0x0000000080000000LL
#define PSW_EC_IA      0x000000007FFFFFFFLL

// PSW - Exculsively Condition Code
// Condition Codes - Arithmetic
#define CC_EQUAL      0 // Equal to Zero
#define CC_LESS       1 // Less than Zero
#define CC_GREATER    2 // Greater than Zero
#define CC_OVERFLOW   3 // Overflow
// Condition Codes - Logical
#define CC_ZERO       0 // Zero
#define CC_NZERO      1 // Non-Zero
#define CC_CARRY      2 // With Carry
#define CC_LOW        1 // Low  (Compare)
#define CC_HIGH       2 // High (Compare)
// Condition Codes - Operations
#define CC_SUCCESS    0 // Successful Operation
#define CC_ERROR      3 // Error
// Condition Codes - I/O Devices
#define CC_AVAIL      0 // Device Available
#define CC_PENDING    1 // I/O Interrupt Pending
#define CC_BUSY       2 // Device Busy
#define CC_NFOUND     3 // Device Not Found

class i3x0_cpuDevice : public cpuDevice
{
public:
	i3x0_cpuDevice() {};
	~i3x0_cpuDevice() {};

private:
	// Host/guest registers
	struct regs {
		// Processor registers
		uint64_t gRegs[I3x0_nGREGS];
		uint32_t aRegs[I3x0_nAREGS];
		uint32_t fRegs[I3x0_nFREGS];
		uint32_t cRegs[I3x0_nCREGS];

		// Virtual machine handler
		regs *host;   // Registers for hypervisor process
		regs *guest;  // Registers for guest process
	};

	regs *regs;
};
