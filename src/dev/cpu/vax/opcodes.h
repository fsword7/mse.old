/*
 * opcodes.h
 *
 *  Created on: Mar 14, 2017
 *      Author: Timothy Stark
 */

#pragma once

// Instruction definitons

#define OPC_REGULAR  0x00000000 // Regular Opcode
#define OPC_EMULATE  0x80000000 // Instruction is emulatable
#define OPC_STRING   0x00000080 // String Instructions
#define OPC_PACKED   0x00000040 // Packed Instructions
#define OPC_VECTOR   0x00000020 // Vector Instructions
#define OPC_FLOAT    0x00000010 // Floating Instructions
#define OPC_OCTA     0x00000008 // Octaword/H_Floating Type
#define OPC_QUAD     0x00000004 // Quadword/G_Floating Type
#define OPC_WORD     0x00000002 // Word/D_Floating Type
#define OPC_BYTE     0x00000001 // Byte/F_Floating Type

#define OPC_nEXTEND 0xFD // For two-bytes Instructions

// Operand type definitions
#define OPR_BYTE    1 // Operand is a byte      (1 byte)
#define OPR_WORD    2 // Operand is a word      (2 bytes)
#define OPR_LONG    4 // Operand is a longword  (4 bytes)
#define OPR_QUAD    8 // Operand is a quadword  (8 bytes)
#define OPR_OCTA   16 // Operand is a octaword  (16 bytes)

#define OPR_SCALE    0x000FF // Scale Mask
#define OPR_FLOAT    0x00F00 // Operand is floating point (otherwise - integer)
#define OPR_FFLOAT   0x00100 // Operand is F_floating type
#define OPR_DFLOAT   0x00200 // Operand is D_floating type
#define OPR_GFLOAT   0x00400 // Operand is G_floating type
#define OPR_HFLOAT   0x00800 // Operand is H_floating type
#define OPR_WRITE    0x01000 // Operand is writable (otherwise - readable)
#define OPR_MODIFIED 0x02000 // Operand is modifiable
#define OPR_IMMED    0x04000 // Operand is immediate
#define OPR_ADDR     0x08000 // Operand is address
#define OPR_VADDR    0x10000 // Operand is variable-length address
#define OPR_BRANCH   0x20000 // Operand is a branch displacement

// Read-Only Integer Operands
#define RB    (OPR_BYTE)
#define RW    (OPR_WORD)
#define RL    (OPR_LONG)
#define RQ    (OPR_QUAD)
#define RO    (OPR_OCTA)

// Read/Write Integer Operands
#define MB    (OPR_MODIFIED|OPR_BYTE)
#define MW    (OPR_MODIFIED|OPR_WORD)
#define ML    (OPR_MODIFIED|OPR_LONG)
#define MQ    (OPR_MODIFIED|OPR_QUAD)
#define MO    (OPR_MODIFIED|OPR_OCTA)

// Write-Only Integer Operands
#define WB    (OPR_WRITE|OPR_BYTE)
#define WW    (OPR_WRITE|OPR_WORD)
#define WL    (OPR_WRITE|OPR_LONG)
#define WQ    (OPR_WRITE|OPR_QUAD)
#define WO    (OPR_WRITE|OPR_OCTA)

// Read-Only Floating Operands
#define RF    (OPR_FFLOAT|OPR_LONG)
#define RD    (OPR_DFLOAT|OPR_QUAD)
#define RG    (OPR_GFLOAT|OPR_QUAD)
#define RH    (OPR_HFLOAT|OPR_OCTA)

// Read/Write Floating Operands
#define MF    (OPR_FFLOAT|OPR_MODIFIED|OPR_LONG)
#define MD    (OPR_DFLOAT|OPR_MODIFIED|OPR_QUAD)
#define MG    (OPR_GFLOAT|OPR_MODIFIED|OPR_QUAD)
#define MH    (OPR_HFLOAT|OPR_MODIFIED|OPR_OCTA)

// Write-Only Floating Operands
#define WF    (OPR_FFLOAT|OPR_WRITE|OPR_LONG)
#define WD    (OPR_DFLOAT|OPR_WRITE|OPR_QUAD)
#define WG    (OPR_GFLOAT|OPR_WRITE|OPR_QUAD)
#define WH    (OPR_HFLOAT|OPR_WRITE|OPR_OCTA)

// Address Operands
#define AB    (OPR_ADDR|OPR_BYTE)
#define AW    (OPR_ADDR|OPR_WORD)
#define AL    (OPR_ADDR|OPR_LONG)
#define AQ    (OPR_ADDR|OPR_QUAD)
#define AO    (OPR_ADDR|OPR_OCTA)
#define AF    (OPR_ADDR|OPR_FFLOAT|OPR_LONG)
#define AD    (OPR_ADDR|OPR_DFLOAT|OPR_QUAD)
#define AG    (OPR_ADDR|OPR_GFLOAT|OPR_QUAD)
#define AH    (OPR_ADDR|OPR_HFLOAT|OPR_OCTA)

// Branch/Misc Operands
#define BB    (OPR_BRANCH|OPR_BYTE)
#define BW    (OPR_BRANCH|OPR_WORD)
#define VB    (OPR_VADDR|OPR_BYTE)

// Immediate Operands
#define IB    (OPR_IMMED|OPR_BYTE)
#define IW    (OPR_IMMED|OPR_WORD)
#define IL    (OPR_IMMED|OPR_LONG)

// Operand mode definitions
#define OP_MMASK     0xF0
#define OP_RMASK     0x0F
#define OP_MEM       -1

#define LIT0  0x00 // Short Literal
#define LIT1  0x10
#define LIT2  0x20
#define LIT3  0x30
#define IDX   0x40 // Indexed
#define REG   0x50 // Register
#define REGD  0x60 // Register Deferred
#define ADEC  0x70 // Autodecrement
#define AINC  0x80 // Autoincrement
#define AINCD 0x90 // Autoincrement Deferred
#define BDP   0xA0 // Byte Displacement
#define BDPD  0xB0 // Byte Displacement Deferred
#define WDP   0xC0 // Word Displacement
#define WDPD  0xD0 // Word Displacement Deferred
#define LDP   0xE0 // Longword Displacement
#define LDPD  0xF0 // Longword Displacement Deferred

// Instruction Data Structure Definitions
// Instruction Table for Assembler, Disassembler, and Execution

struct vaxOpcode {
	const char *Name;             // Name of the Instruction
	const char *Desc;             // Description of the Instruction
	uint32_t    Flags;            // Instruction Flags
	uint8_t     opExtend;         // MSB of Instruction (Normally Zero)
	uint8_t     opCode;           // Opcode Value
	uint8_t     nOperands;        // Number of Operands
	uint32_t    oprMode[6];       // Attributes/Scales for Each Operand
	void        (*Execute)();     // Execute Routine
};
