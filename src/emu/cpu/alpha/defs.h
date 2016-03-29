
#pragma once

#include "src/main/main.h"

#define AXP_nGREGS  32
#define AXP_nFPREGS 32

#define REG_nR0     0
#define REG_nR1     1
#define REG_nR2     2
#define REG_nR30    30
#define REG_nR31    31

#define REG_nSP     REG_nR30
#define REG_nZERO   REG_nR31

#define REG_nF0     0
#define REG_nF1     1
#define REG_nF2     2
#define REG_nF3     3
#define REG_nF31    31
#define REG_nFZERO  REG_nF31

// Architecture Variants
#define IMPL_EV4    0x0     // 21064 (EV4)
#define IMPL_EV5    0x1     // 21164 (EV5)
#define IMPL_EV6    0x2     // 21264 (EV6)
#define IMPL_EV7    0x3     // 21364 (EV7)

#define IMP_BWX     0x0001  // Byte/Word
#define IMP_FIX     0x0002  // SQRT/FLT-INT/MOVES
#define IMP_CIX     0x0004  // Counts
#define IMP_MVI     0x0100  // Multimedia
#define IMP_PRC     0x0200  // Precise Exceptions
#define IMP_PFM     0x1000  // Prefetch w/Modify

// Instruction Field
#define OPP_CODE   26
#define OPM_CODE   0x3F
#define OPP_RA     21
#define OPM_RA     0x1F
#define OPP_RB     16
#define OPM_RB     0x1F
#define OPP_RC     0
#define OPM_RC     0x1F
#define OPP_MDSP   0
#define OPM_MDSP   0xFFFF
#define OPP_BDSP   0
#define OPM_BDSP   0x1FFFFF
#define OPP_LIT8   13
#define OPM_LIT8   0xFF
#define OPP_ILIT   12

#define OPP_IFNC   5
#define OPM_IFNC   0x3F
#define OPP_FFNC   5
#define OPM_FFNC   0x3F
#define OPP_FRND   11
#define OPM_FRND   0x3
#define OPP_PALCD  0
#define OPM_PALCD  0x3FFFFFF

#define OPP_FTRP   13
#define OPM_FTRP   0x7

#define OP_GETCD(x)   ((x) >> OPP_CODE) & OPM_CODE)
#define OP_GETRA(x)   ((x) >> OPP_RA) & OPM_RA)
#define OP_GETRB(x)   ((x) >> OPP_RB) & OPM_RB)
#define OP_GETRC(x)   ((x) >> OPP_RC) & OPM_RC)
#define OP_GETPAL(x)  ((x) >> OPP_PALCD) & OPM_PALCD)
#define OP_GETIFNC(x) ((x) >> OPP_IFNC) & OPM_IFNC)
#define OP_GETFFNC(x) ((x) >> OPP_FFNC) & OPM_FFNC)
#define OP_GETFRND(x) ((x) >> OPP_FRND) & OPM_FRND)
#define OP_GETLIT8(x) ((x) >> OPP_LIT8) & OPM_LIT8)
#define OP_GETMDSP(x) ((x) >> OPP_MDSP) & OPM_MDSP)
#define OP_GETBDSP(x) ((x) >> OPP_BDSP) & OPM_BDSP)

// PALcode Mode
#define PAL_UNDF   0  // Undefined
#define PAL_VMS    1  // VMS
#define PAL_UNIX   2  // UNIX
#define PAL_NT     3  // Windows NT

// Access Mode
#define ACC_K      0  // Kernel
#define ACC_E      1  // Executive
#define ACC_S      2  // Supervisor
#define ACC_U      3  // User

enum opCodes {
	OPC_PAL,  OPC_01,    OPC_02,    OPC_03,
	OPC_04,   OPC_05,    OPC_06,    OPC_07,
	OPC_LDA,  OPC_LDAH,  OPC_LDBU,  OPC_LDQ_U,
	OPC_LDWU, OPC_STW,   OPC_STB,   OPC_STQ_U,
	OPC_IALU, OPC_ILOG,  OPC_ISHFT, OPC_IMUL,
	OPC_IFLT, OPC_VAX,   OPC_IEEE,  OPC_FP,
	OPC_MISC, OPC_PAL19, OPC_JMP,   OPC_PAL1B,
	OPC_FLTI, OPC_PAL1D, OPC_PAL1E, OPC_PAL1F,
	OPC_LDF,  OPC_LOG,   OPC_LDS,   OPC_LDT,
	OPC_STF,  OPC_STG,   OPC_STS,   OPC_STT,
	OPC_LDL,  OPC_LDQ,   OPC_LDL_L, OPC_LDQ_L,
	OPC_STL,  OPC_STQ,   OPC_STL_C, OPC_STQ_C,
	OPC_BR,   OPC_FBEQ,  OPC_FBLT,  OPC_FBLE,
	OPC_BSR,  OPC_FBNE,  OPC_FBGE,  OPC_FBGT,
	OPC_BLBC, OPC_BEQ,   OPC_BLT,   OPC_BLE,
	OPC_BLBS, OPC_BNE,   OPC_BGE,   OPC_BGT
};

class Process_Alpha
{
private:
	uint64 gRegs[AXP_nGREGS];
	uint64 fpRegs[AXP_nFPREGS];
	uint64 pcReg;
};
