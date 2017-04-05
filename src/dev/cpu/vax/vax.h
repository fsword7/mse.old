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
#define VAX_nOPCTBL 1024

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

#define REG_AP      gRegs[REG_nAP].l
#define REG_FP      gRegs[REG_nFP].l
#define REG_SP      gRegs[REG_nSP].l
#define REG_PC      gRegs[REG_nPC].l

// Processor flags
#define CPU_INIE    0x80000000 // Interrupt/exception in progress

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
#define PSL_MBZ			 0x3020FF00 // Must Be Zero
#define PSL_CM			 0x80000000 // PDP-11 Compatibility mode
#define PSL_TP			 0x40000000 // Trace Pending
#define PSL_FPD			 0x08000000 // First Part Done
#define PSL_IS			 0x04000000 // Interrupt Stack
#define PSL_CUR			 0x03000000 // Current Mode
#define PSL_PRV			 0x00C00000 // Previous Mode
#define PSL_IPL			 0x001F0000 // Interrupt Priority Level

#define PSL_P_CUR        24   // Position of IPL bits
#define PSL_P_PRV        22   // Position of previous mode bits
#define PSL_P_IPL        16   // Position of current mode bits

#define PSL_M_IPL        0x1F // Interrupt Priority Level Mask
#define PSL_M_ISCUR      0x07 // Access Mode Mask with IS flag
#define PSL_M_MODE       0x03 // Access Mode Mask

#define PSL_GETIPL(ps)   (((ps) >> PSL_P_IPL) & PSL_M_IPL)
#define PSL_GETCUR(ps)   (((ps) >> PSL_P_CUR) & PSL_M_MODE)
#define PSL_GETISCUR(ps) (((ps) >> PSL_P_CUR) & PSL_M_ISCUR)
#define PSL_GETPRV(ps)   (((ps) >> PSL_P_PRV) & PSL_M_MODE)

#define PSL_SETIPL(ipl)  ((ipl) << PSL_P_IPL)
#define PSL_SETCUR(acc)  ((acc) << PSL_P_CUR)
#define PSL_SETPRV(acc)  ((acc) << PSL_P_PRV)

// PSW - Processor Status Word for User mode
#define PSW_MASK         0xFFFF // PSW User Mask
#define PSW_MBZ          0xFF00 // Must be zeroes
#define PSW_DV			 0x0080 // Decimal Overflow Trap Enable
#define PSW_FU			 0x0040 // Floating Underflow Trap Enable
#define PSW_IV			 0x0020 // Integer Overflow Trap Enable
#define PSW_T			 0x0010 // Trace Enable
#define PSW_CC			 0x000F // Condition Codes

// CC - Condition Codes (part of PSW word)
#define CC_N			 0x0008 // Negative
#define CC_Z			 0x0004 // Zero
#define CC_V			 0x0002 // Overflow
#define CC_C			 0x0001 // Carry
#define CC_MASK			 (CC_N|CC_Z|CC_V|CC_C)

// Access Mode (Protection Codes)
#define AM_KERNEL		 0 // Kernel mode for OS kernel
#define AM_EXECUTIVE	 1 // Executive mode for file system
#define AM_SUPERVISOR	 2 // Supervisor mode for DCL (shell)
#define AM_USER			 3 // User mode for normal programs

// Interrupt/Exception Types
#define IE_INT           0 // Interrupt
#define IE_EXC           1 // Exception
#define IE_SVE           2 // Severe Exception

// System Control Block Vectors
#define SCB_PASSIVE      0x00  // Passive Release
#define SCB_MCHK         0x04  // Machine Check
#define SCB_KSNV         0x08  // Kernel Stack Not Valid
#define SCB_POWER        0x0C  // Power Fail
#define SCB_RESIN        0x10  // Reserved or Privileged Instruction
#define SCB_XFC          0x14  // Customer Reserved Instruction (XFC)
#define SCB_RESOP        0x18  // Reserved Operand
#define SCB_RESAD        0x1C  // Reserved Address Mode
#define SCB_ACV          0x20  // Access-Control Violation
#define SCB_TNV          0x24  // Translation Not Valid
#define SCB_TP           0x28  // Trace Pending
#define SCB_BPT          0x2C  // Breakpoint Instruction
#define SCB_COMPAT       0x30  // Compatibility
#define SCB_ARITH        0x34  // Arithmetic
#define SCB_CHMK         0x40  // Change Mode to Kernel
#define SCB_CHME         0x44  // Change Mode to Executive
#define SCB_CHMS         0x48  // Change Mode to Supervisor
#define SCB_CHMU         0x4C  // Change Mode to User
#define SCB_CRDERR       0x54  // CRD Error Interrupt
#define SCB_MEMERR       0x60  // Memory Error Interrupt
#define SCB_IPLSOFT      0x80  // Software Interrupt Base
#define SCB_SOFTWARE1    0x84  // Software Level 1
#define SCB_SOFTWARE2    0x88  // Software Level 2
#define SCB_SOFTWARE3    0x8C  // Software Level 3
#define SCB_SOFTWARE4    0x90  // Software Level 4
#define SCB_SOFTWARE5    0x94  // Software Level 5
#define SCB_SOFTWARE6    0x98  // Software Level 6
#define SCB_SOFTWARE7    0x9C  // Software Level 7
#define SCB_SOFTWARE8    0xA0  // Software Level 8
#define SCB_SOFTWARE9    0xA4  // Software Level 9
#define SCB_SOFTWARE10   0xA8  // Software Level A
#define SCB_SOFTWARE11   0xAC  // Software Level B
#define SCB_SOFTWARE12   0xB0  // Software Level C
#define SCB_SOFTWARE13   0xB4  // Software Level D
#define SCB_SOFTWARE14   0xB8  // Software Level E
#define SCB_SOFTWARE15   0xBC  // Software Level F
#define SCB_TIMER        0xC0  // Interval Timer
#define SCB_EMULATE      0xC8  // Subset Emulation
#define SCB_EMULFPD      0xCC  // Subset Emulation with FPD flag
#define SCB_CSREAD       0xF0  // Console Storage Read
#define SCB_CSWRITE      0xF4  // Console Storage Write
#define SCB_CTYIN        0xF8  // Console Terminal Read
#define SCB_CTYOUT       0xFC  // COnsole Terminal Write

#define SCB_ADDR         ALIGN_LONG // SCB Address Mask
#define SCB_VECTOR       ALIGN_LONG // SCB Vector Mask
#define SCB_NOPRIV		 1

// SISR register definitions
#define SISR_MASK        0xFFFE
#define SISR_2           (1u << 2)

// ASTLVL register definition
#define AST_MASK         0x00000007
#define AST_MAX          4

#define BR_MASK          ALIGN_LONG
#define LR_MASK          0x003FFFFF

// Read/write memory access
#define CACC             0x80000000         // Console memory access
#define RACC             curMode            // Read memory access
#define WACC             (curMode << 4)     // Write memory access
#define CRA              (CACC|RACC)        // Console write memory access
#define CWA              (CACC|WACC)        // Console read memory access
#define ACC_MASK(am)     (1u << (am))       // Access mode mask

#define PA_MASK30        0x3FFFFFFF         // 30-bit physical addressing

// Interrupt Priority Level
#define IPL_MAX			0x1F
// KA650 IPL levels
#define IPL_MEMERR		0x1D
#define IPL_CRDERR		0x1A
// standard IPL levels
#define IPL_HMAX		0x17
#define IPL_HMIN		0x14
#define IPL_SMAX		0x0F

#define IRQ_TRAP        0x00E0 // Trap requests
#define IRQ_IPL         0x001F // IPL requests
#define IRQ_M_IPL       0x1F
#define IRQ_M_TRAP      0x07
#define IRQ_P_IPL       0
#define IRQ_P_TRAP      5

#define IRQ_GETIPL(irq)        (((irq) >> IRQ_P_IPL) & IRQ_M_IPL)
#define IRQ_GETTRAP(irq)       (((irq) >> IRQ_P_TRAP) & IRQ_M_TRAP)

#define UpdateIRQ() \
	irqFlags = evaluate();

// Exception Codes - Stop
#define STOP_HALT		-1		// HALT opcode
#define STOP_UOPC		-2		// Unimplemented opcode
#define STOP_ILLVEC		-3		// Illegal vector
#define STOP_UIPL       -4      // Undefined IPL level

// Exception Codes - Fault
#define EXC_RSVD_INST_FAULT   1 // Reserved instruction fault
#define EXC_RSVD_ADDR_FAULT   2 // Reserved address fault
#define EXC_RSVD_OPND_FAULT   3 // Reserved operand fault
#define EXC_PRIV_INST_FAULT   4 // Privileged instruction fault

// Store data macro routines for instructions
#define StoreB(op0, op1, d)   \
	if (op0 != OPR_MEM)       \
		gRegs[op0].b  = d;    \
	else                      \
		writev(op1, d, LN_BYTE, WACC);

#define StoreW(op0, op1, d)   \
	if (op0 != OPR_MEM)       \
		gRegs[op0].w  = d;    \
	else                      \
		writev(op1, d, LN_WORD, WACC);

#define StoreL(op0, op1, d)   \
	if (op0 != OPR_MEM) {       \
		gRegs[op0].l  = d;    \
		printf("reg: (w) %08X => R%d\n", d, op0); \
	} else {                      \
		writev(op1, d, LN_LONG, WACC); \
		printf("mem: (w) %08X => %08X\n", d, op1); \
    }

#define StoreQ(op0, op1, dl, dh)        \
	if (op0 != OPR_MEM) {               \
		gRegs[op0].l  = dl;             \
		gRegs[op0+1].l = dh;            \
	} else {                            \
		writev(op1+LN_LONG, dh, LN_LONG, WACC); \
		writev(op1, dl, LN_LONG, WACC);         \
	}

#define UpdateCC_Z1ZP(cc) \
	cc = CC_Z | ((cc) & CC_C);


#define UpdateCC_IIZZ_B(cc, d)            \
	if (SXTB(d) < 0)           cc = CC_N; \
	else if (SXTB(d) == 0)     cc = CC_Z; \
	else                       cc = 0;

#define UpdateCC_IIZZ_W(cc, d)            \
	if (SXTW(d) < 0)           cc = CC_N; \
	else if (SXTW(d) == 0)     cc = CC_Z; \
	else                       cc = 0;

#define UpdateCC_IIZZ_L(cc, d)            \
	if (SXTL(d) < 0)           cc = CC_N; \
	else if (SXTL(d) == 0)     cc = CC_Z; \
	else                       cc = 0;

#define UpdateCC_IIZZ_Q(cc, dl, dh)       \
	if (SXTL(dh) < 0)          cc = CC_N; \
	else if (((dl)|(dh)) == 0) cc = CC_Z; \
	else                       cc = 0;

#define UpdateCC_IIZZ_64(cc, d)           \
	if (SXTQ(d) < 0LL)         cc = CC_N; \
	else if ((d) == 0LL)       cc = CC_Z; \
	else                       cc = 0;


#define UpdateCC_IIZP_B(cc, d)                          \
	if (int8_t(d) < 0)         cc = CC_N | (cc & CC_C); \
	else if (int8_t(d) == 0)   cc = CC_Z | (cc & CC_C); \
	else                       cc = cc & CC_C;

#define UpdateCC_IIZP_W(cc, d)                          \
	if (int16_t(d) < 0)        cc = CC_N | (cc & CC_C); \
	else if (int16_t(d) == 0)  cc = CC_Z | (cc & CC_C); \
	else                       cc = cc & CC_C;

#define UpdateCC_IIZP_L(cc, d)                          \
	if (int32_t(d) < 0)        cc = CC_N | (cc & CC_C); \
	else if (int32_t(d) == 0)  cc = CC_Z | (cc & CC_C); \
	else                       cc = cc & CC_C;

#define UpdateCC_IIZP_Q(cc, dl, dh)                     \
	if (int32_t(dh) < 0)       cc = CC_N | (cc & CC_C); \
	else if (((dl)|(dh)) == 0) cc = CC_Z | (cc & CC_C); \
	else                       cc = cc & CC_C;

#define UpdateCC_CMP_I(cc, s1, s2)         \
	if ((s1) < (s2))          cc = CC_N;   \
	else if ((s1) == (s2))    cc = CC_Z;   \
	else                      cc = 0;      \
	if (ZXTL(s1) < ZXTL(s2))  cc |= CC_C;

#define UpdateCC_CMP_B(cc, s1, s2)   UpdateCC_CMP_I(cc, s1, s2)
#define UpdateCC_CMP_W(cc, s1, s2)   UpdateCC_CMP_I(cc, s1, s2)
#define UpdateCC_CMP_L(cc, s1, s2)   UpdateCC_CMP_I(cc, s1, s2)

#define UpdateV_ADD_B(cc, d, s1, s2) \
	if (((~(s1) ^ (s2)) & ((s1) ^ (d))) & SIGN_B) \
		cc |= CC_V;

#define UpdateV_ADD_W(cc, d, s1, s2) \
	if (((~(s1) ^ (s2)) & ((s1) ^ (d))) & SIGN_W) \
		cc |= CC_V;

#define UpdateV_ADD_L(cc, d, s1, s2) \
	if (((~(s1) ^ (s2)) & ((s1) ^ (d))) & SIGN_L) \
		cc |= CC_V;

#define UpdateCC_ADD(cc, d, s1, s2) \
	if (ZXTL(d) < ZXTL(s2)) cc |= CC_C;

#define UpdateCC_ADD_B(cc, d, s1, s2) \
	UpdateCC_IIZZ_B(cc, d);           \
	UpdateV_ADD_B(cc, d, s1, s2);     \
	UpdateCC_ADD(cc, d, s1, s2)

#define UpdateCC_ADD_W(cc, d, s1, s2) \
	UpdateCC_IIZZ_W(cc, d);           \
	UpdateV_ADD_W(cc, d, s1, s2);     \
	UpdateCC_ADD(cc, d, s1, s2)

#define UpdateCC_ADD_L(cc, d, s1, s2) \
	UpdateCC_IIZZ_L(cc, d);           \
	UpdateV_ADD_L(cc, d, s1, s2);     \
	UpdateCC_ADD(cc, d, s1, s2)


#define UpdateV_SUB_B(cc, d, s1, s2) \
	if ((((s1) ^ (s2)) & (~(s1) ^ (d))) & SIGN_B) \
		cc |= CC_V;

#define UpdateV_SUB_W(cc, d, s1, s2) \
	if ((((s1) ^ (s2)) & (~(s1) ^ (d))) & SIGN_W) \
		cc |= CC_V;

#define UpdateV_SUB_L(cc, d, s1, s2) \
	if ((((s1) ^ (s2)) & (~(s1) ^ (d))) & SIGN_L) \
		cc |= CC_V;

#define UpdateCC_SUB(cc, d, s1, s2) \
	if (ZXTL(s2) < ZXTL(s1)) cc |= CC_C;

#define UpdateCC_SUB_B(cc, d, s1, s2) \
	UpdateCC_IIZZ_B(cc, d);           \
	UpdateV_SUB_B(cc, d, s1, s2);     \
	UpdateCC_SUB(cc, d, s1, s2)

#define UpdateCC_SUB_W(cc, d, s1, s2) \
	UpdateCC_IIZZ_W(cc, d);           \
	UpdateV_SUB_W(cc, d, s1, s2);     \
	UpdateCC_SUB(cc, d, s1, s2)

#define UpdateCC_SUB_L(cc, d, s1, s2) \
	UpdateCC_IIZZ_L(cc, d);           \
	UpdateV_SUB_L(cc, d, s1, s2);     \
	UpdateCC_SUB(cc, d, s1, s2)


struct vaxOpcode {
	const char *opName;           // Name of the Instruction
	const char *opDesc;           // Description of the Instruction
	uint32_t    flags;            // Instruction Flags
	uint8_t     opExtend;         // MSB of Instruction (Normally Zero)
	uint16_t    opCode;           // Extend/Opcode Value
	uint8_t     nOperands;        // Number of Operands
	uint32_t    oprMode[6];       // Attributes/Scales for Each Operand
	void        (*execute)();     // Execute Routine
};

class vax_cpuDevice : public cpuDevice
{
public:
	vax_cpuDevice();
	virtual ~vax_cpuDevice();

	void buildOpcodes();

	// CPU function calls
	virtual void init();
//	virtual void reset();
	virtual int  boot();
//	void execute();

	int disasmOperand(char **ptr, uint32_t &vAddr, const vaxOpcode *opc, int opn, bool idxFlag);
	int disasm(uint32_t pcAddr);

	void assignMemory(uint8_t *mem, uint32_t memSize);
	void setPCAddr(uint32_t pcAddr);
	char *stringCC(uint32_t cc);
	int getBit();
	int setBit(int bit);

	// Interrupt/exception services
	int  evaluate();
	void interrupt();
	int  exception(int ie, uint32_t vec, uint32_t ipl);
	int  fault(uint32_t vec);
	void resume();

	// Memory access routines
	uint32_t readp(uint32_t addr, int size);                   // Read access (aligned)
	uint32_t readpl(uint32_t pAddr);                           // Longword read access (aligned)
	void     writep(uint32_t addr, uint32_t data, int size);   // Write access (aligned)
	void     writepl(uint32_t pAddr, uint32_t data);           // Longword write access (aligned)

	uint8_t  readvb(uint32_t vAddr, uint32_t acc);
	uint16_t readvw(uint32_t vAddr, uint32_t acc);
	uint32_t readvl(uint32_t vAddr, uint32_t acc);
	uint32_t readv(uint32_t vAddr, uint32_t size, uint32_t acc);

	void     writevb(uint32_t vAddr, uint8_t data, uint32_t acc);
	void     writevw(uint32_t vAddr, uint16_t data, uint32_t acc);
	void     writevl(uint32_t vAddr, uint32_t data, uint32_t acc);
	void     writev(uint32_t vAddr, uint32_t data, uint32_t size, uint32_t acc);

	void     flushvi();
	uint32_t readvi(int size); // Instruction read access

	// Read/write privileged register access
	virtual uint32_t readpr(uint32_t pReg);
	virtual void     writepr(uint32_t pReg, uint32_t data);

//	inline void storeb(uint32_t op0, uint32_t op1, uint8_t data)
//	{
//		if (op0 >= 0)
//			gRegs[op0].b = data;
//		else
//			writevb(op1, data, WA);
//	}

	// Console memory access routines (unaligned)
	void flushci();
	int  readci(uint32_t addr, uint32_t *data, int size); // Instruction read access
	int  readc(uint32_t addr, uint32_t *data, int size);  // Data read access

protected:
	scale32_t gRegs[VAX_nGREGS];   // General registers
	uint32_t  pRegs[VAX_nPREGS];   // Processor registers
	uint32_t  opRegs[VAX_nOPREGS]; // Operand registers
	uint32_t  psReg;               // Processor status register
	uint32_t  ccReg;               // Condition Code register (part of PSL register)

	uint32_t  flags;               // Processor flags

	// Opcode table for operand decoding and disassembler
	const vaxOpcode *opCodes[VAX_nOPCTBL];

	// Mirror of system memory (from system device)
	uint32_t  memSize;
	uint8_t  *mem;

	// Instruction buffer (look-ahead buffer)
	uint32_t  ibData[2]; // IB Aligned data buffer
	uint32_t  ibpAddr;   // IB physical Address
	uint32_t  ibCount;   // IB Count

	// Console instruction buffer (look-ahead buffer)
	uint32_t  cibData[2]; // IB Aligned data buffer
	uint32_t  cibAddr;    // IB Address
	uint32_t  cibCount;   // IB Count
	uint32_t  cvAddr;     // virtual address

	// Memory management unit
	uint32_t  curMode;    // Current access mode
	uint32_t  prvMode;    // Previous access mode

	uint32_t  paMask;     // Physical addressing mask

	// Interrupt/exception services
	uint32_t  faultAddr;   // Faulting PC address
	uint32_t  irqFlags;

private:
#ifdef ENABLE_DEBUG
	uint32_t dbgFlags;
	Debug    dbg;
#endif /* ENABLE_DEBUG */
};
