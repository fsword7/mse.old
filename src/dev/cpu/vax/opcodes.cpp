/*
 * vax_opc.cpp
 *
 *  Created on: Mar 14, 2017
 *      Author: Timothy Stark
 *
 *  VAX instruction table
 *
 */

#include "emu/core.h"
#include "emu/debug.h"
#include "emu/devcpu.h"
#include "dev/cpu/vax/vax.h"
#include "dev/cpu/vax/opcodes.h"

// New VAX opcode table (working in progress)
#define UOPC     OPF_RSVD
#define OPC_REG  OPF_REGULAR
#define OPC_EMU  OPF_EMULATE
#define OPC_VEC  OPF_VECTOR

#define OPR(...) __VA_ARGS__
#define OPC(name, opcode, nopr, opr, flags) \
	{ name, flags, opcode, nopr, opr }

const vaxOpcode vaxOpcodes[] = {
	OPC("UOPC",     OPC_nUOPC,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),

	// Opcode 0000 - 00FF (00 - FF)
	OPC("HALT",     OPC_nHALT,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),
	OPC("NOP",      OPC_nNOP,      0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),
	OPC("REI",      OPC_nREI,      0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BPT",      OPC_nBPT,      0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),
	OPC("RET",      OPC_nRET,      0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),
	OPC("RSB",      OPC_nRSB,      0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),
	OPC("LDPCTX",   OPC_nLDPCTX,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),
	OPC("SVPCTX",   OPC_nSVPCTX,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),

	OPC("CVTPS",    OPC_nCVTPS,    4, OPR({ RW, AB, RW, AB, 0,  0  }), OPC_REG|OPF_FPD),
	OPC("CVTSP",    OPC_nCVTSP,    4, OPR({ RW, AB, RW, AB, 0,  0  }), OPC_REG|OPF_FPD),
	OPC("INDEX",    OPC_nINDEX,    6, OPR({ RL, RL, RL, RL, RL, WL }), OPC_REG),
	OPC("CRC",      OPC_nCRC,      4, OPR({ AB, RL, RW, AB, 0,  0  }), OPC_REG|OPF_FPD),
	OPC("PROBER",   OPC_nPROBER,   3, OPR({ RB, RW, AB, 0,  0,  0  }), OPC_REG),
	OPC("PROBEW",   OPC_nPROBEW,   3, OPR({ RB, RW, AB, 0,  0,  0  }), OPC_REG),
	OPC("INSQUE",   OPC_nINSQUE,   2, OPR({ AB, AB, 0,  0,  0,  0  }), OPC_REG),
	OPC("REMQUE",   OPC_nREMQUE,   2, OPR({ AB, WL, 0,  0,  0,  0  }), OPC_REG),

	OPC("BSBB",     OPC_nBSBB,     1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BRB",      OPC_nBRB,      1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BNEQ",     OPC_nBNEQ,     1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BNEQU",    OPC_nBNEQ,     1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BEQL",     OPC_nBEQL,     1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BEQLU",    OPC_nBEQL,     1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BGTR",     OPC_nBGTR,     1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BLEQ",     OPC_nBLEQ,     1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("JSB",      OPC_nJSB,      1, OPR({ AB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("JMP",      OPC_nJMP,      1, OPR({ AB, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("BGEQ",     OPC_nBGEQ,     1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BLSS",     OPC_nBLSS,     1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BGTRU",    OPC_nBGTRU,    1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BLEQU",    OPC_nBLEQU,    1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BVC",      OPC_nBVC,      1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BVS",      OPC_nBVS,      1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BCC",      OPC_nBCC,      1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BGEQU",    OPC_nBCC,      1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BCS",      OPC_nBCS,      1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BLSSU",    OPC_nBCS,      1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("ADDP4",    OPC_nADDP4,    4, OPR({ RW, AB, RW, AB, 0,  0  }), OPC_REG|OPF_FPD),
	OPC("ADDP6",    OPC_nADDP6,    6, OPR({ RW, AB, RW, AB, RW, AB }), OPC_REG|OPF_FPD),
	OPC("SUBP4",    OPC_nSUBP4,    4, OPR({ RW, AB, RW, AB, 0,  0  }), OPC_REG|OPF_FPD),
	OPC("SUBP6",    OPC_nSUBP6,    6, OPR({ RW, AB, RW, AB, RW, AB }), OPC_REG|OPF_FPD),
	OPC("CVTPT",    OPC_nCVTPT,    5, OPR({ RW, AB, AB, RW, AB, 0  }), OPC_REG|OPF_FPD),
	OPC("MULP",     OPC_nMULP6,    6, OPR({ RW, AB, RW, AB, RW, AB }), OPC_REG|OPF_FPD),
	OPC("CVTTP",    OPC_nCVTTP,    5, OPR({ RW, AB, AB, RW, AB, 0  }), OPC_REG|OPF_FPD),
	OPC("DIVP",     OPC_nDIVP6,    6, OPR({ RW, AB, RW, AB, RW, AB }), OPC_REG|OPF_FPD),

	OPC("MOVC3",    OPC_nMOVC3,    3, OPR({ RW, AB, AB, 0,  0,  0  }), OPC_REG|OPF_FPD),
	OPC("CMPC3",    OPC_nCMPC3,    3, OPR({ RW, AB, AB, 0,  0,  0  }), OPC_REG|OPF_FPD),
	OPC("SCANC",    OPC_nSCANC,    4, OPR({ RW, AB, AB, RB, 0,  0  }), OPC_REG|OPF_FPD),
	OPC("SPANC",    OPC_nSPANC,    4, OPR({ RW, AB, AB, RB, 0,  0  }), OPC_REG|OPF_FPD),
	OPC("MOVC5",    OPC_nMOVC5,    5, OPR({ RW, AB, RB, RW, AB, 0  }), OPC_REG|OPF_FPD),
	OPC("CMPC5",    OPC_nCMPC5,    5, OPR({ RW, AB, RB, RW, AB, 0  }), OPC_REG|OPF_FPD),
	OPC("MOVTC",    OPC_nMOVTC,    6, OPR({ RW, AB, RB, AB, RW, AB }), OPC_REG|OPF_FPD),
	OPC("MOVTUC",   OPC_nMOVTUC,   6, OPR({ RW, AB, RB, AB, RW, AB }), OPC_REG|OPF_FPD),

	OPC("BSBW",     OPC_nBSBW,     1, OPR({ BW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BRW",      OPC_nBRW,      1, OPR({ BW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("CVTWL",    OPC_nCVTWL,    2, OPR({ RW, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("CVTWB",    OPC_nCVTWB,    2, OPR({ RW, WB, 0,  0,  0,  0  }), OPC_REG),
	OPC("MOVP",     OPC_nMOVP,     3, OPR({ RW, AB, AB, 0,  0,  0  }), OPC_REG|OPF_FPD),
	OPC("CMPP3",    OPC_nCMPP3,    3, OPR({ RW, AB, AB, 0,  0,  0  }), OPC_REG|OPF_FPD),
	OPC("CVTPL",    OPC_nCVTPL,    3, OPR({ RW, AB, WL, 0,  0,  0  }), OPC_REG|OPF_FPD),
	OPC("CMPP4",    OPC_nCMPP4,    4, OPR({ RW, AB, RW, AB, 0,  0  }), OPC_REG|OPF_FPD),

	OPC("EDITPC",   OPC_nEDITPC,   4, OPR({ RW, AB, AB, AB, 0,  0  }), OPC_REG|OPF_FPD),
	OPC("MATCGC",   OPC_nMATCHC,   4, OPR({ RW, AB, RW, AB, 0,  0  }), OPC_REG|OPF_FPD),
	OPC("LOCC",     OPC_nLOCC,     3, OPR({ RB, RW, AB, 0,  0,  0  }), OPC_REG|OPF_FPD),
	OPC("SKPC",     OPC_nSKPC,     3, OPR({ RB, RW, AB, 0,  0,  0  }), OPC_REG|OPF_FPD),
	OPC("MOVZWL",   OPC_nMOVZWL,   2, OPR({ RW, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("ACBW",     OPC_nACBW,     4, OPR({ RW, RW, MW, BW, 0,  0  }), OPC_REG),
	OPC("MOVAW",    OPC_nMOVAW,    2, OPR({ AW, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("PUSHAW",   OPC_nPUSHAW,   1, OPR({ AW, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("ADDF2",    OPC_nADDF2,    2, OPR({ RF, MF, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("ADDF3",    OPC_nADDF3,    3, OPR({ RF, RF, WF, 0,  0,  0  }), OPC_REG|UOPC),
	OPC("SUBF2",    OPC_nSUBF2,    2, OPR({ RF, MF, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("SUBF3",    OPC_nSUBF3,    3, OPR({ RF, RF, WF, 0,  0,  0  }), OPC_REG|UOPC),
	OPC("MULF2",    OPC_nMULF2,    2, OPR({ RF, MF, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("MULF3",    OPC_nMULF3,    3, OPR({ RF, RF, WF, 0,  0,  0  }), OPC_REG|UOPC),
	OPC("DIVF2",    OPC_nDIVF2,    2, OPR({ RF, MF, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("DIVF3",    OPC_nDIVF3,    3, OPR({ RF, RF, WF, 0,  0,  0  }), OPC_REG|UOPC),

	OPC("CVTFB",    OPC_nCVTFB,    2, OPR({ RF, WB, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTFW",    OPC_nCVTFW,    2, OPR({ RF, WW, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTFL",    OPC_nCVTFL,    2, OPR({ RF, WL, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTRFL",   OPC_nCVTRFL,   2, OPR({ RF, WL, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTBF",    OPC_nCVTBF,    2, OPR({ RB, WF, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTWF",    OPC_nCVTWF,    2, OPR({ RW, WF, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTLF",    OPC_nCVTLF,    2, OPR({ RL, WF, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("ACBF",     OPC_nACBF,     4, OPR({ RF, RF, MF, BW, 0,  0  }), OPC_REG|UOPC),

	OPC("MOVF",     OPC_nMOVF,     2, OPR({ RF, WF, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CMPF",     OPC_nCMPF,     2, OPR({ RF, RF, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("MNEGF",    OPC_nMNEGF,    2, OPR({ RF, WF, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("TSTF",     OPC_nTSTF,     1, OPR({ RF, 0,  0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("EMODF",    OPC_nEMODF,    5, OPR({ RF, RB, RF, WL, WF, 0  }), OPC_REG|UOPC),
	OPC("POLYF",    OPC_nPOLYF,    3, OPR({ RF, RW, AB, 0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTFD",    OPC_nCVTFD,    2, OPR({ RF, WD, 0,  0,  0,  0  }), OPC_REG|UOPC),

	OPC("ADAWI",    OPC_nADAWI,    2, OPR({ RW, WW, 0,  0,  0,  0  }), OPC_REG),
	OPC("INSQHI",   OPC_nINSQHI,   2, OPR({ AB, AQ, 0,  0,  0,  0  }), OPC_REG),
	OPC("INSQTI",   OPC_nINSQTI,   2, OPR({ AB, AQ, 0,  0,  0,  0  }), OPC_REG),
	OPC("REMQHI",   OPC_nREMQHI,   2, OPR({ AQ, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("REMQTI",   OPC_nREMQTI,   2, OPR({ AQ, WL, 0,  0,  0,  0  }), OPC_REG),

	OPC("ADDD2",    OPC_nADDD2,    2, OPR({ RD, MD, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("ADDD3",    OPC_nADDD3,    3, OPR({ RD, RD, WD, 0,  0,  0  }), OPC_REG|UOPC),
	OPC("SUBD2",    OPC_nSUBD2,    2, OPR({ RD, MD, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("SUBD3",    OPC_nSUBD3,    3, OPR({ RD, RD, WD, 0,  0,  0  }), OPC_REG|UOPC),
	OPC("MULD2",    OPC_nMULD2,    2, OPR({ RD, MD, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("MULD3",    OPC_nMULD3,    3, OPR({ RD, RD, WD, 0,  0,  0  }), OPC_REG|UOPC),
	OPC("DIVD2",    OPC_nDIVD2,    2, OPR({ RD, MD, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("DIVD3",    OPC_nDIVD3,    3, OPR({ RD, RD, WD, 0,  0,  0  }), OPC_REG|UOPC),

	OPC("CVTDB",    OPC_nCVTDB,    2, OPR({ RD, WB, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTDW",    OPC_nCVTDW,    2, OPR({ RD, WW, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTDL",    OPC_nCVTDL,    2, OPR({ RD, WL, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTRDL",   OPC_nCVTRDL,   2, OPR({ RD, WL, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTBD",    OPC_nCVTBD,    2, OPR({ RB, WD, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTWD",    OPC_nCVTWD,    2, OPR({ RW, WD, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTLD",    OPC_nCVTLD,    2, OPR({ RL, WD, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("ACBD",     OPC_nACBD,     4, OPR({ RD, RD, MD, BW, 0,  0  }), OPC_REG|UOPC),

	OPC("MOVD",     OPC_nMOVD,     2, OPR({ RD, WD, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CMPD",     OPC_nCMPD,     2, OPR({ RD, RD, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("MNEGD",    OPC_nMNEGD,    2, OPR({ RD, WD, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("TSTD",     OPC_nTSTD,     1, OPR({ RD, 0,  0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("EMODD",    OPC_nEMODD,    5, OPR({ RD, RB, RD, WL, WD, 0  }), OPC_REG|UOPC),
	OPC("POLYD",    OPC_nPOLYD,    3, OPR({ RD, RW, AB, 0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTDF",    OPC_nCVTDF,    2, OPR({ RD, WF, 0,  0,  0,  0  }), OPC_REG|UOPC),

	OPC("ASHL",     OPC_nASHL,     3, OPR({ RB, RL, WL, 0,  0,  0  }), OPC_REG),
	OPC("ASHQ",     OPC_nASHQ,     3, OPR({ RB, RQ, WQ, 0,  0,  0  }), OPC_REG),
	OPC("EMUL",     OPC_nEMUL,     4, OPR({ RL, RL, RL, WQ, 0,  0  }), OPC_REG),
	OPC("EDIV",     OPC_nEDIV,     4, OPR({ RL, RQ, WL, WL, 0,  0  }), OPC_REG),
	OPC("CLRQ",     OPC_nCLRQ,     1, OPR({ WQ, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("CLRD",     OPC_nCLRQ,     1, OPR({ WD, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("CLRG",     OPC_nCLRQ,     1, OPR({ WG, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("MOVQ",     OPC_nMOVQ,     2, OPR({ RQ, WQ, 0,  0,  0,  0  }), OPC_REG),
	OPC("MOVAQ",    OPC_nMOVAQ,    2, OPR({ AQ, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("MOVAD",    OPC_nMOVAQ,    2, OPR({ AD, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("MOVAG",    OPC_nMOVAQ,    2, OPR({ AG, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("PUSHAQ",   OPC_nPUSHAQ,   1, OPR({ AQ, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("PUSHAD",   OPC_nPUSHAQ,   1, OPR({ AD, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("PUSHAG",   OPC_nPUSHAQ,   1, OPR({ AG, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("ADDB2",    OPC_nADDB2,    2, OPR({ RB, MB, 0,  0,  0,  0  }), OPC_REG),
	OPC("ADDB3",    OPC_nADDB3,    3, OPR({ RB, RB, WB, 0,  0,  0  }), OPC_REG),
	OPC("SUBB2",    OPC_nSUBB2,    2, OPR({ RB, MB, 0,  0,  0,  0  }), OPC_REG),
	OPC("SUBB3",    OPC_nSUBB3,    3, OPR({ RB, RB, WB, 0,  0,  0  }), OPC_REG),
	OPC("MULB2",    OPC_nMULB2,    2, OPR({ RB, MB, 0,  0,  0,  0  }), OPC_REG),
	OPC("MULB3",    OPC_nMULB3,    3, OPR({ RB, RB, WB, 0,  0,  0  }), OPC_REG),
	OPC("DIVB2",    OPC_nDIVB2,    2, OPR({ RB, MB, 0,  0,  0,  0  }), OPC_REG),
	OPC("DIVB3",    OPC_nDIVB3,    3, OPR({ RB, RB, WB, 0,  0,  0  }), OPC_REG),

	OPC("BISB2",    OPC_nBISB2,    2, OPR({ RB, MB, 0,  0,  0,  0  }), OPC_REG),
	OPC("BISB3",    OPC_nBISB3,    3, OPR({ RB, RB, WB, 0,  0,  0  }), OPC_REG),
	OPC("BICB2",    OPC_nBICB2,    2, OPR({ RB, MB, 0,  0,  0,  0  }), OPC_REG),
	OPC("BICB3",    OPC_nBICB3,    3, OPR({ RB, RB, WB, 0,  0,  0  }), OPC_REG),
	OPC("XORB2",    OPC_nXORB2,    2, OPR({ RB, MB, 0,  0,  0,  0  }), OPC_REG),
	OPC("XORB3",    OPC_nXORB3,    3, OPR({ RB, RB, WB, 0,  0,  0  }), OPC_REG),
	OPC("MNEGB",    OPC_nMNEGB,    2, OPR({ RB, WB, 0,  0,  0,  0  }), OPC_REG),
	OPC("CASEB",    OPC_nCASEB,    3, OPR({ RB, RB, RB, 0,  0,  0  }), OPC_REG),

	OPC("MOVB",     OPC_nMOVB,     2, OPR({ RB, WB, 0,  0,  0,  0  }), OPC_REG),
	OPC("CMPB",     OPC_nCMPB,     2, OPR({ RB, RB, 0,  0,  0,  0  }), OPC_REG),
	OPC("MCOMB",    OPC_nMCOMB,    2, OPR({ RB, WB, 0,  0,  0,  0  }), OPC_REG),
	OPC("BITB",     OPC_nBITB,     2, OPR({ RB, RB, 0,  0,  0,  0  }), OPC_REG),
	OPC("CLRB",     OPC_nCLRB,     1, OPR({ WB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("TSTB",     OPC_nTSTB,     1, OPR({ RB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("INCB",     OPC_nINCB,     1, OPR({ MB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("DECB",     OPC_nDECB,     1, OPR({ MB, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("CVTBL",    OPC_nCVTBL,    2, OPR({ RB, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("CVTBW",    OPC_nCVTBW,    2, OPR({ RB, WW, 0,  0,  0,  0  }), OPC_REG),
	OPC("MOVZBL",   OPC_nMOVZBL,   2, OPR({ RB, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("MOVZBW",   OPC_nMOVZBW,   2, OPR({ RB, WW, 0,  0,  0,  0  }), OPC_REG),
	OPC("ROTL",     OPC_nROTL,     3, OPR({ RB, RL, WL, 0,  0,  0  }), OPC_REG),
	OPC("ACBB",     OPC_nACBB,     4, OPR({ RB, RB, MB, BW, 0,  0  }), OPC_REG),
	OPC("MOVAB",    OPC_nMOVAB,    2, OPR({ AB, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("PUSHAB",   OPC_nPUSHAB,   1, OPR({ AB, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("ADDW2",    OPC_nADDW2,    2, OPR({ RW, MW, 0,  0,  0,  0  }), OPC_REG),
	OPC("ADDW3",    OPC_nADDW3,    3, OPR({ RW, RW, WW, 0,  0,  0  }), OPC_REG),
	OPC("SUBW2",    OPC_nSUBW2,    2, OPR({ RW, MW, 0,  0,  0,  0  }), OPC_REG),
	OPC("SUBW3",    OPC_nSUBW3,    3, OPR({ RW, RW, WW, 0,  0,  0  }), OPC_REG),
	OPC("MULW2",    OPC_nMULW2,    2, OPR({ RW, MW, 0,  0,  0,  0  }), OPC_REG),
	OPC("MULW3",    OPC_nMULW3,    3, OPR({ RW, RW, WW, 0,  0,  0  }), OPC_REG),
	OPC("DIVW2",    OPC_nDIVW2,    2, OPR({ RW, MW, 0,  0,  0,  0  }), OPC_REG),
	OPC("DIVW3",    OPC_nDIVW3,    3, OPR({ RW, RW, WW, 0,  0,  0  }), OPC_REG),

	OPC("BISW2",    OPC_nBISW2,    2, OPR({ RW, MW, 0,  0,  0,  0  }), OPC_REG),
	OPC("BISW3",    OPC_nBISW3,    3, OPR({ RW, RW, WW, 0,  0,  0  }), OPC_REG),
	OPC("BICW2",    OPC_nBICW2,    2, OPR({ RW, MW, 0,  0,  0,  0  }), OPC_REG),
	OPC("BICW3",    OPC_nBICW3,    3, OPR({ RW, RW, WW, 0,  0,  0  }), OPC_REG),
	OPC("XORW2",    OPC_nXORW2,    2, OPR({ RW, MW, 0,  0,  0,  0  }), OPC_REG),
	OPC("XORW3",    OPC_nXORW3,    3, OPR({ RW, RW, WW, 0,  0,  0  }), OPC_REG),
	OPC("MNEGW",    OPC_nMNEGW,    2, OPR({ RW, WW, 0,  0,  0,  0  }), OPC_REG),
	OPC("CASEW",    OPC_nCASEW,    3, OPR({ RW, RW, RW, 0,  0,  0  }), OPC_REG),

	OPC("MOVW",     OPC_nMOVW,     2, OPR({ RW, WW, 0,  0,  0,  0  }), OPC_REG),
	OPC("CMPW",     OPC_nCMPW,     2, OPR({ RW, RW, 0,  0,  0,  0  }), OPC_REG),
	OPC("MCOMW",    OPC_nMCOMW,    2, OPR({ RW, WW, 0,  0,  0,  0  }), OPC_REG),
	OPC("BITW",     OPC_nBITW,     2, OPR({ RW, RW, 0,  0,  0,  0  }), OPC_REG),
	OPC("CLRW",     OPC_nCLRW,     1, OPR({ WW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("TSTW",     OPC_nTSTW,     1, OPR({ RW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("INCW",     OPC_nINCW,     1, OPR({ MW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("DECW",     OPC_nDECW,     1, OPR({ MW, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("BISPSW",   OPC_nBISPSW,   1, OPR({ RW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BICPSW",   OPC_nBICPSW,   1, OPR({ RW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("POPR",     OPC_nPOPR,     1, OPR({ RW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("PUSHR",    OPC_nPUSHR,    1, OPR({ RW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("CHMK",     OPC_nCHMK,     1, OPR({ RW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("CHME",     OPC_nCHME,     1, OPR({ RW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("CHMS",     OPC_nCHMS,     1, OPR({ RW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("CHMU",     OPC_nCHMU,     1, OPR({ RW, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("ADDL2",    OPC_nADDL2,    2, OPR({ RL, ML, 0,  0,  0,  0  }), OPC_REG),
	OPC("ADDL3",    OPC_nADDL3,    3, OPR({ RL, RL, WL, 0,  0,  0  }), OPC_REG),
	OPC("SUBL2",    OPC_nSUBL2,    2, OPR({ RL, ML, 0,  0,  0,  0  }), OPC_REG),
	OPC("SUBL3",    OPC_nSUBL3,    3, OPR({ RL, RL, WL, 0,  0,  0  }), OPC_REG),
	OPC("MULL2",    OPC_nMULL2,    2, OPR({ RL, ML, 0,  0,  0,  0  }), OPC_REG),
	OPC("MULL3",    OPC_nMULL3,    3, OPR({ RL, RL, WL, 0,  0,  0  }), OPC_REG),
	OPC("DIVL2",    OPC_nDIVL2,    2, OPR({ RL, ML, 0,  0,  0,  0  }), OPC_REG),
	OPC("DIVL3",    OPC_nDIVL3,    3, OPR({ RL, RL, WL, 0,  0,  0  }), OPC_REG),

	OPC("BISL2",    OPC_nBISL2,    2, OPR({ RL, ML, 0,  0,  0,  0  }), OPC_REG),
	OPC("BISL3",    OPC_nBISL3,    3, OPR({ RL, RL, WL, 0,  0,  0  }), OPC_REG),
	OPC("BICL2",    OPC_nBICL2,    2, OPR({ RL, ML, 0,  0,  0,  0  }), OPC_REG),
	OPC("BICL3",    OPC_nBICL3,    3, OPR({ RL, RL, WL, 0,  0,  0  }), OPC_REG),
	OPC("XORL2",    OPC_nXORL2,    2, OPR({ RL, ML, 0,  0,  0,  0  }), OPC_REG),
	OPC("XORL3",    OPC_nXORL3,    3, OPR({ RL, RL, WL, 0,  0,  0  }), OPC_REG),
	OPC("MNEGL",    OPC_nMNEGL,    2, OPR({ RL, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("CASEL",    OPC_nCASEL,    3, OPR({ RL, RL, RL, 0,  0,  0  }), OPC_REG),

	OPC("MOVL",     OPC_nMOVL,     2, OPR({ RL, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("CMPL",     OPC_nCMPL,     2, OPR({ RL, RL, 0,  0,  0,  0  }), OPC_REG),
	OPC("MCOML",    OPC_nMCOML,    2, OPR({ RL, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("BITL",     OPC_nBITL,     2, OPR({ RL, RL, 0,  0,  0,  0  }), OPC_REG),
	OPC("CLRL",     OPC_nCLRL,     1, OPR({ WL, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("CLRF",     OPC_nCLRL,     1, OPR({ WF, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("TSTL",     OPC_nTSTL,     1, OPR({ RL, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("INCL",     OPC_nINCL,     1, OPR({ ML, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("DECL",     OPC_nDECL,     1, OPR({ ML, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("ADWC",     OPC_nADWC,     2, OPR({ RL, ML, 0,  0,  0,  0  }), OPC_REG),
	OPC("SBWC",     OPC_nSBWC,     2, OPR({ RL, ML, 0,  0,  0,  0  }), OPC_REG),
	OPC("MTPR",     OPC_nMTPR,     2, OPR({ RL, RL, 0,  0,  0,  0  }), OPC_REG),
	OPC("MFPR",     OPC_nMFPR,     2, OPR({ RL, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("MOVPSL",   OPC_nMOVPSL,   1, OPR({ WL, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("PUSHL",    OPC_nPUSHL,    1, OPR({ RL, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("MOVAL",    OPC_nMOVAL,    2, OPR({ AL, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("MOVAF",    OPC_nMOVAL,    2, OPR({ AF, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("PUSHAL",   OPC_nPUSHAL,   1, OPR({ AL, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("PUSHAF",   OPC_nPUSHAL,   1, OPR({ AF, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("BBS",      OPC_nBBS,      3, OPR({ RL, VB, BB, 0,  0,  0  }), OPC_REG),
	OPC("BBC",      OPC_nBBC,      3, OPR({ RL, VB, BB, 0,  0,  0  }), OPC_REG),
	OPC("BBSS",     OPC_nBBSS,     3, OPR({ RL, VB, BB, 0,  0,  0  }), OPC_REG),
	OPC("BBCS",     OPC_nBBCS,     3, OPR({ RL, VB, BB, 0,  0,  0  }), OPC_REG),
	OPC("BBSC",     OPC_nBBSC,     3, OPR({ RL, VB, BB, 0,  0,  0  }), OPC_REG),
	OPC("BBCC",     OPC_nBBCC,     3, OPR({ RL, VB, BB, 0,  0,  0  }), OPC_REG),
	OPC("BBSSI",    OPC_nBBSSI,    3, OPR({ RL, VB, BB, 0,  0,  0  }), OPC_REG),
	OPC("BBCCI",    OPC_nBBCCI,    3, OPR({ RL, VB, BB, 0,  0,  0  }), OPC_REG),

	OPC("BLBS",     OPC_nBLBS,     2, OPR({ RL, BB, 0,  0,  0,  0  }), OPC_REG),
	OPC("BLBC",     OPC_nBLBC,     2, OPR({ RL, BB, 0,  0,  0,  0  }), OPC_REG),
	OPC("FFS",      OPC_nFFS,      4, OPR({ RL, RB, VB, WL, 0,  0  }), OPC_REG),
	OPC("FFC",      OPC_nFFC,      4, OPR({ RL, RB, VB, WL, 0,  0  }), OPC_REG),
	OPC("CMPV",     OPC_nCMPV,     4, OPR({ RL, RB, VB, RL, 0,  0  }), OPC_REG),
	OPC("CMPZV",    OPC_nCMPZV,    4, OPR({ RL, RB, VB, RL, 0,  0  }), OPC_REG),
	OPC("EXTV",     OPC_nEXTV,     4, OPR({ RL, RB, VB, WL, 0,  0  }), OPC_REG),
	OPC("EXTZV",    OPC_nEXTZV,    4, OPR({ RL, RB, VB, WL, 0,  0  }), OPC_REG),

	OPC("INSV",     OPC_nINSV,     4, OPR({ RL, RL, RB, VB, 0 , 0  }), OPC_REG),
	OPC("ACBL",     OPC_nACBL,     4, OPR({ RL, RL, ML, BW, 0 , 0  }), OPC_REG),
	OPC("AOBLSS",   OPC_nAOBLSS,   3, OPR({ RL, ML, BB, 0,  0,  0  }), OPC_REG),
	OPC("AOBLEQ",   OPC_nAOBLEQ,   3, OPR({ RL, ML, BB, 0,  0,  0  }), OPC_REG),
	OPC("SOBGEQ",   OPC_nSOBGEQ,   2, OPR({ ML, BB, 0,  0,  0,  0  }), OPC_REG),
	OPC("SOBGTR",   OPC_nSOBGTR,   2, OPR({ ML, BB, 0,  0,  0,  0  }), OPC_REG),
	OPC("CVTLB",    OPC_nCVTLB,    2, OPR({ RL, WB, 0,  0,  0,  0  }), OPC_REG),
	OPC("CVTLW",    OPC_nCVTLW,    2, OPR({ RL, WW, 0,  0,  0,  0  }), OPC_REG),

	OPC("ASHP",     OPC_nASHP,     6, OPR({ RB, RW, AB, RB, RW, AB }), OPC_REG|OPF_FPD),
	OPC("CVTLP",    OPC_nCVTLP,    3, OPR({ RL, RW, AB, 0 , 0 , 0  }), OPC_REG|OPF_FPD),
	OPC("CALLG",    OPC_nCALLG,    2, OPR({ AB, AB, 0,  0,  0,  0  }), OPC_REG),
	OPC("CALLS",    OPC_nCALLS,    2, OPR({ RL, AB, 0,  0,  0,  0  }), OPC_REG),
	OPC("XFC",      OPC_nXFC,      0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),

	// Opcode 0100 - 01FF (00FD - FFFD)
	OPC("WAIT",     OPC_nWAIT,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG|UOPC),

	OPC("MFVP",     OPC_nMFVP,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("CVTDH",    OPC_nCVTDH,    2, OPR({ RD, WH, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTGF",    OPC_nCVTGF,    2, OPR({ RG, WF, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("VLDL",     OPC_nVLDL,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VGATHL",   OPC_nVGATHL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VLDQ",     OPC_nVLDQ,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VGATHQ",   OPC_nVGATHQ,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("ADDG2",    OPC_nADDG2,    2, OPR({ RG, MG, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("ADDG3",    OPC_nADDG3,    3, OPR({ RG, RG, WG, 0,  0,  0  }), OPC_REG|UOPC),
	OPC("SUBG2",    OPC_nSUBG2,    2, OPR({ RG, MG, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("SUBG3",    OPC_nSUBG3,    3, OPR({ RG, RG, WG, 0,  0,  0  }), OPC_REG|UOPC),
	OPC("MULG2",    OPC_nMULG2,    2, OPR({ RG, MG, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("MULG3",    OPC_nMULG3,    3, OPR({ RG, RG, WG, 0,  0,  0  }), OPC_REG|UOPC),
	OPC("DIVG2",    OPC_nDIVG2,    2, OPR({ RG, MG, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("DIVG3",    OPC_nDIVG3,    3, OPR({ RG, RG, WG, 0,  0,  0  }), OPC_REG|UOPC),

	OPC("CVTGB",    OPC_nCVTGB,    2, OPR({ RG, WB, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTGW",    OPC_nCVTGW,    2, OPR({ RG, WW, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTGL",    OPC_nCVTGL,    2, OPR({ RG, WL, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTRGL",   OPC_nCVTRGL,   2, OPR({ RG, WL, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTBG",    OPC_nCVTBG,    2, OPR({ RB, WG, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTWG",    OPC_nCVTWG,    2, OPR({ RW, WG, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTLG",    OPC_nCVTLG,    2, OPR({ RL, WG, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("ACBG",     OPC_nACBG,     4, OPR({ RG, RG, MG, BW, 0,  0  }), OPC_REG|UOPC),

	OPC("MOVG",     OPC_nMOVG,     2, OPR({ RG, WG, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CMPG",     OPC_nCMPG,     2, OPR({ RG, RG, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("MNEGG",    OPC_nMNEGG,    2, OPR({ RG, WG, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("TSTG",     OPC_nTSTG,     1, OPR({ RG, 0,  0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("EMODG",    OPC_nEMODG,    5, OPR({ RG, RB, RG, WL, WG, 0  }), OPC_REG|UOPC),
	OPC("POLYG",    OPC_nPOLYG,    3, OPR({ RG, RW, AB, 0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTGH",    OPC_nCVTGH,    2, OPR({ RG, WH, 0,  0,  0,  0  }), OPC_REG|UOPC),

	OPC("ADDH2",    OPC_nADDG2,    2, OPR({ RH, MH, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("ADDH3",    OPC_nADDG3,    3, OPR({ RH, RH, WH, 0,  0,  0  }), OPC_REG|UOPC),
	OPC("SUBH2",    OPC_nSUBG2,    2, OPR({ RH, MH, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("SUBH3",    OPC_nSUBG3,    3, OPR({ RH, RH, WH, 0,  0,  0  }), OPC_REG|UOPC),
	OPC("MULH2",    OPC_nMULG2,    2, OPR({ RH, MH, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("MULH3",    OPC_nMULG3,    3, OPR({ RH, RH, WH, 0,  0,  0  }), OPC_REG|UOPC),
	OPC("DIVH2",    OPC_nDIVG2,    2, OPR({ RH, MH, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("DIVH3",    OPC_nDIVG3,    3, OPR({ RH, RH, WH, 0,  0,  0  }), OPC_REG|UOPC),

	OPC("CVTHB",    OPC_nCVTHB,    2, OPR({ RH, WB, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTHW",    OPC_nCVTHW,    2, OPR({ RH, WW, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTHL",    OPC_nCVTHL,    2, OPR({ RH, WL, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTRHL",   OPC_nCVTRHL,   2, OPR({ RH, WL, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTBH",    OPC_nCVTBH,    2, OPR({ RB, WH, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTWH",    OPC_nCVTWH,    2, OPR({ RW, WH, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTLH",    OPC_nCVTLH,    2, OPR({ RL, WH, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("ACBH",     OPC_nACBH,     4, OPR({ RH, RH, MH, BW, 0,  0  }), OPC_REG|UOPC),

	OPC("MOVH",     OPC_nMOVH,     2, OPR({ RH, WH, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CMPH",     OPC_nCMPH,     2, OPR({ RH, RG, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("MNEGH",    OPC_nMNEGH,    2, OPR({ RH, WH, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("TSTH",     OPC_nTSTH,     1, OPR({ RH, 0,  0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("EMODH",    OPC_nEMODH,    5, OPR({ RH, RB, RH, WL, WH, 0  }), OPC_REG|UOPC),
	OPC("POLYH",    OPC_nPOLYH,    3, OPR({ RH, RW, AB, 0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTHG",    OPC_nCVTHG,    2, OPR({ RH, WG, 0,  0,  0,  0  }), OPC_REG|UOPC),

	OPC("CLRO",     OPC_nCLRO,     1, OPR({ WO, 0,  0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CLRH",     OPC_nCLRO,     1, OPR({ WO, 0,  0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("MOVO",     OPC_nMOVO,     2, OPR({ RO, WO, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("MOVAO",    OPC_nMOVAO,    2, OPR({ AO, WL, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("MOVAH",    OPC_nMOVAO,    2, OPR({ AH, WL, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("PUSHAO",   OPC_nPUSHAO,   1, OPR({ AO, 0,  0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("PUSHAH",   OPC_nPUSHAO,   1, OPR({ AH, 0,  0,  0,  0,  0  }), OPC_REG|UOPC),

	OPC("VVADDL",   OPC_nVVADDL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSADDL",   OPC_nVSADDL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVADDG",   OPC_nVVADDG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSADDG",   OPC_nVSADDG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVADDF",   OPC_nVVADDF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSADDF",   OPC_nVSADDF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVADDD",   OPC_nVVADDD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSADDD",   OPC_nVSADDD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("VVSUBL",   OPC_nVVSUBL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSSUBL",   OPC_nVVSUBL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVSUBG",   OPC_nVVSUBG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSSUBG",   OPC_nVSSUBG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVSUBF",   OPC_nVVSUBF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSSUBF",   OPC_nVSSUBF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVSUBD",   OPC_nVVSUBD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSSUBD",   OPC_nVSSUBD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("CVTFH",    OPC_nCVTFH,    2, OPR({ RF, WH, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTFG",    OPC_nCVTFG,    2, OPR({ RF, WG, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("PROBEVMR", OPC_nPROBEVMR, 0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("PROBEVMW", OPC_nPROBEVMW, 0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("VSTL",     OPC_nVSTL,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSCATL",   OPC_nVSCATL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSTQ",     OPC_nVSTQ,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSCATQ",   OPC_nVSCATQ,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("VVMULL",   OPC_nVVMULL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSMULL",   OPC_nVSMULL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVMULG",   OPC_nVVMULG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSMULG",   OPC_nVSMULG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVMULF",   OPC_nVVMULF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSMULF",   OPC_nVSMULF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVMULD",   OPC_nVVMULD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSMULD",   OPC_nVSMULD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("VSYNC",    OPC_nVSYNC,    0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("MTVP",     OPC_nMTVP,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVDIVG",   OPC_nVVDIVG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSDIVG",   OPC_nVSDIVG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVDIVF",   OPC_nVVDIVF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSDIVF",   OPC_nVSDIVF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVDIVD",   OPC_nVVDIVD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSDIVD",   OPC_nVSDIVD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("VVCMPL",   OPC_nVVCMPL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSCMPL",   OPC_nVSCMPL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVCMPG",   OPC_nVVCMPG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSCMPG",   OPC_nVSCMPG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVCMPF",   OPC_nVVCMPF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSCMPF",   OPC_nVSCMPF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVCMPD",   OPC_nVVCMPD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSCMPD",   OPC_nVSCMPD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("VVBISL",   OPC_nVVBISL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSBISL",   OPC_nVSBISL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVBICL",   OPC_nVVBICL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSBICL",   OPC_nVSBICL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("VVSRLL",   OPC_nVVSRLL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSSRLL",   OPC_nVSSRLL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVSLLL",   OPC_nVVSLLL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSSLLL",   OPC_nVSSLLL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("VVXORL",   OPC_nVVXORL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSXORL",   OPC_nVSXORL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVCVT",    OPC_nVVCVT,    0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("IOTA",     OPC_nIOTA,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("CVTHF",    OPC_nCVTHF,    2, OPR({ RH, WF, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CVTHD",    OPC_nCVTHD,    2, OPR({ RH, WD, 0,  0,  0,  0  }), OPC_REG|UOPC),

	// Opcode 9200 - 02FF (00FE - FFFE)

	// Opcode 0300 - 03FF (00FF - FFFF)
	OPC("BUGL",     OPC_nBUGL,     1, OPR({ IL, 0,  0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("BUGW",     OPC_nBUGW,     1, OPR({ IL, 0,  0,  0,  0,  0  }), OPC_REG|UOPC),

	// null terminator
	{ nullptr }
};

void vax_cpuDevice::buildOpcodes()
{
	const vaxOpcode *opc;
	dopc_t    *dopc;
	uint16_t   opCode;

	// Initialize all opcode table as default unimplemented opcode
	for (int idx = 0; idx < CPU_nOPCTBL; idx++)
		opCodes[idx] = &vaxOpcodes[0];

	// Initialize opcode table
	for (int idx = 1; vaxOpcodes[idx].opName; idx++) {
		opCode = vaxOpcodes[idx].opCode;
		if (opCodes[opCode] == &vaxOpcodes[0])
			opCodes[opCode] = &vaxOpcodes[idx];
	}

	// Initialize decode opcode table with default illegal instruction
	// Set flag to issue RSVD_INSTRUCTION_FAULT as default.
	for (int idx = 0; idx < CPU_nOPCTBL; idx++) {
		dopCode[idx].opCode = nullptr;
		dopCode[idx].flag   = OPF_RSVD;
		dopCode[idx].nModes = 0;
		for (int oidx = 0; oidx < 6; oidx++)
			dopCode[idx].opMode[oidx] = 0;
	}

	// Initialize decode opcode table for each valid instruction
	for (int idx = 1; vaxOpcodes[idx].opName; idx++) {
		opc    = &vaxOpcodes[idx];
		opCode = opc->opCode;
		dopc   = &dopCode[opCode];

		dopc->opCode = opc;
		dopc->flag   = opc->flags;
		dopc->nModes = opc->nOperands;
		for (int oidx = 0; oidx < dopc->nModes; oidx++)
			dopc->opMode[oidx] = opc->oprMode[oidx];
	}
}

#if 0
#define DEF_NAME(cpu, opc) nullptr

const vaxOpcode vax_oldOpcodes[] =
{
	{
		"HALT", "Halt",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nHALT,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, HALT) // Opcode Function
	},

	{
		"NOP", "No operation",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nNOP,				// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, NOP) // Opcode Function
	},

	{
		"REI", "Return from exception or interrupt",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nREI,				// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, REI) // Opcode Function
	},

	{
		"BPT", "Break point trap",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBPT,				// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BPT) // Opcode Function
	},

	{
		"RET", "Return from called procedure",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nRET,				// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, RET) // Opcode Function
	},

	{
		"RSB", "Return from subroutine",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nRSB,				// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, RSB) // Opcode Function
	},

	{
		"LDPCTX", "Load program context",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nLDPCTX,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, LDPCTX) // Opcode Function
	},

	{
		"SVPCTX", "Save program context",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nSVPCTX,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, SVPCTX) // Opcode Function
	},

	{
		"CVTPS", "Convert packed to leading separate",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nCVTPS,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RW, AB, RW, AB, 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CVTSP", "Convert leading separate to packed",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nCVTSP,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RW, AB, RW, AB, 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"INDEX", "Index calculation",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nINDEX,			// Opcode (Extended + Normal)
		6,							// Number of Operands
		{ RL, RL, RL, RL, RL, WL }, // Operand Scale/Mode

		DEF_NAME(vax, INDEX) // Opcode Function
	},

	{
		"CRC", "Calcuate cyclic redundancy check",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nCRC,				// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ AB, RL, RW, AB, 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"PROBER", "Probe read access",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nPROBER,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RB, RW, AB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, PROBER) // Opcode Function
	},

	{
		"PROBEW", "Probe write access",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nPROBEW,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RB, RW, AB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, PROBEW) // Opcode Function
	},

	{
		"INSQUE", "Insert into queue",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nINSQUE,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ AB, AB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, INSQUE) // Opcode Function
	},

	{
		"REMQUE", "Remove from queue",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nREMQUE,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ AB, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, REMQUE) // Opcode Function
	},

	{
		"BSBB", "Branch to subroutine with byte displacment",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBSBB,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BSBB) // Opcode Function
	},

	{
		"BRB", "Branch with byte displacement",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBRB,				// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BRB) // Opcode Function
	},

	{
		"BNEQ", "Branch on not equal",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBNEQ,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BNEQ) // Opcode Function
	},

	{
		"BNEQU", "Branch on not equal unsigned",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBNEQ,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BNEQ) // Opcode Function
	},

	{
		"BEQL", "Branch on equal",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBEQL,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BEQL) // Opcode Function
	},

	{
		"BEQLU", "Branch on equal unsigned",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBEQL,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BEQL) // Opcode Function
	},

	{
		"BGTR", "Branch on greater",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBGTR,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BGTR) // Opcode Function
	},

	{
		"BLEQ", "Branch on less or equal",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBLEQ,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BLEQ) // Opcode Function
	},

	{
		"JSB", "Jump to subroutine",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nJSB,				// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ AB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, JSB) // Opcode Function
	},

	{
		"JMP", "Jump",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nJMP,				// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ AB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, JMP) // Opcode Function
	},

	{
		"BGEQ", "Branch on greater or equal",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBGEQ,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BGEQ) // Opcode Function
	},

	{
		"BLSS", "Branch on less",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBLSS,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BLSS) // Opcode Function
	},

	{
		"BGTRU", "Branch on greater unsigned",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBGTRU,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BGTRU) // Opcode Function
	},

	{
		"BLEQU", "Branch on less or equal unsigned",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBLEQU,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BLEQU) // Opcode Function
	},

	{
		"BVC", "Branch on overflow clear",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBVC,				// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BVC) // Opcode Function
	},

	{
		"BVS", "Branch on overflow set",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBVS,				// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BVS) // Opcode Function
	},

	{
		"BCC", "Branch on carry clear",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBCC,				// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BCC) // Opcode Function
	},

	{
		"BGEQU", "Branch on greater or equal unsigned",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBCC,				// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BCC) // Opcode Function
	},

	{
		"BCS", "Branch on carry set",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBCS,				// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BCS) // Opcode Function
	},

	{
		"BLSSU", "Branch on less unsigned",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBCS,				// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BCS) // Opcode Function
	},

	{
		"ADDP4", "Add packed 4 operand",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nADDP4,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RW, AB, RW, AB, 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"ADDP6", "Add packed 6 operand",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nADDP6,			// Opcode (Extended + Normal)
		6,							// Number of Operands
		{ RW, AB, RW, AB, RW, AB }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"SUBP4", "Subtract packed 4 operand",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nSUBP4,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RW, AB, RW, AB, 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"SUBP6", "Subtract packed 6 operand",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nSUBP6,			// Opcode (Extended + Normal)
		6,							// Number of Operands
		{ RW, AB, RW, AB, RW, AB }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CVTPT", "Convert packed to trailing",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nCVTPT,			// Opcode (Extended + Normal)
		5,							// Number of Operands
		{ RW, AB, AB, RW, AB, 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"MULP6", "Multiply packed",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nMULP6,			// Opcode (Extended + Normal)
		6,							// Number of Operands
		{ RW, AB, RW, AB, RW, AB }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CVTTP", "Convert trailing to packed",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nCVTTP,			// Opcode (Extended + Normal)
		5,							// Number of Operands
		{ RW, AB, AB, RW, AB, 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"DIVP6", "Divide packed",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nDIVP6,			// Opcode (Extended + Normal)
		6,							// Number of Operands
		{ RW, AB, RW, AB, RW, AB }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"MOVC3", "Move character 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMOVC3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RW, AB, AB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVC3) // Opcode Function
	},

	{
		"CMPC3", "Compare character 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCMPC3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RW, AB, AB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CMPC3) // Opcode Function
	},

	{
		"SCANC", "Scan for character",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nSCANC,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RW, AB, AB, RB, 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, SCANC) // Opcode Function
	},

	{
		"SPANC", "Span characters",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nSPANC,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RW, AB, AB, RB, 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, SPANC) // Opcode Function
	},

	{
		"MOVC5", "Move character 5 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMOVC5,			// Opcode (Extended + Normal)
		5,							// Number of Operands
		{ RW, AB, RB, RW, AB, 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVC5) // Opcode Function
	},

	{
		"CMPC5", "Compare character 5 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCMPC5,			// Opcode (Extended + Normal)
		5,							// Number of Operands
		{ RW, AB, RB, RW, AB, 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CMPC5) // Opcode Function
	},

	{
		"MOVTC", "Move translated characters",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nMOVTC,			// Opcode (Extended + Normal)
		6,							// Number of Operands
		{ RW, AB, RB, AB, RW, AB }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"MOVTUC", "Move translated until character",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nMOVTUC,			// Opcode (Extended + Normal)
		6,							// Number of Operands
		{ RW, AB, RB, AB, RW, AB }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"BSBW", "Branch to subroutine with word displacment",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBSBW,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BW, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BSBW) // Opcode Function
	},

	{
		"BRW", "Branch with word displacement",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBRW,				// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ BW, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BRW) // Opcode Function
	},

	{
		"CVTWL", "Convert word to longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTWL,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTWL) // Opcode Function
	},

	{
		"CVTWB", "Convert word to byte",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTWB,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, WB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTWB) // Opcode Function
	},

	{
		"MOVP", "Move packed",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nMOVP,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RW, AB, AB, 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CMPP3", "Compare packed 3 operand",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nCMPP3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RW, AB, AB, 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CVTPL", "Convert packed to longword",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nCVTPL,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RW, AB, WL, 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CMPP4", "Compare packed 4 operand",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nCMPP4,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RW, AB, RW, AB, 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"EDITPC", "Edit packed to character",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nEDITPC,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RW, AB, AB, AB, 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"MATCHC", "Match characters",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nMATCHC,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RW, AB, RW, AB, 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"LOCC", "Locate character",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nLOCC,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RB, RW, AB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, LOCC) // Opcode Function
	},

	{
		"SKPC", "Skip character",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nSKPC,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RB, RW, AB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, SKPC) // Opcode Function
	},

	{
		"MOVZWL", "Move zero-extended word to longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMOVZWL,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVZWL) // Opcode Function
	},

	{
		"ACBW", "Add compare and branch word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nACBW,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RW, RW, MW, BW, 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ACBW) // Opcode Function
	},

	{
		"MOVAW", "Move address of word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMOVAW,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ AW, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVL) // Opcode Function
	},

	{
		"PUSHAW", "Push address of word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nPUSHAW,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ AW, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, PUSHL) // Opcode Function
	},

	{
		"ADDF2", "Add F_floating 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nADDF2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RF, MF, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ADDF) // Opcode Function
	},

	{
		"ADDF3", "Add F_floating 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nADDF3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RF, RF, WF, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ADDF) // Opcode Function
	},

	{
		"SUBF2", "Subtract F_floating 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nSUBF2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RF, MF, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, SUBF) // Opcode Function
	},

	{
		"SUBF3", "Subtract F_floating 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nSUBF3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RF, RF, WF, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, SUBF) // Opcode Function
	},

	{
		"MULF2", "Multiply F_floating 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMULF2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RF, MF, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MULF) // Opcode Function
	},

	{
		"MULF3", "Multiply F_floating 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMULF3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RF, RF, WF, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MULF) // Opcode Function
	},

	{
		"DIVF2", "Divide F_floating 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nDIVF2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RF, MF, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, DIVF) // Opcode Function
	},

	{
		"DIVF3", "Divide F_floating 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nDIVF3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RF, RF, WF, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, DIVF) // Opcode Function
	},

	{
		"CVTFB", "Convert F_floating to byte",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTFB,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RF, WB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTFB) // Opcode Function
	},

	{
		"CVTFW", "Convert F_floating to word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTFW,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RF, WW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTFW) // Opcode Function
	},

	{
		"CVTFL", "Convert F_floating to longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTFL,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RF, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTFL) // Opcode Function
	},

	{
		"CVTRFL", "Convert rounded F_floating to longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTRFL,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RF, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTRFL) // Opcode Function
	},

	{
		"CVTBF", "Convert byte to F_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTBF,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, WF, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTBF) // Opcode Function
	},

	{
		"CVTWF", "Convert word to F_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTWF,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, WF, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTWF) // Opcode Function
	},

	{
		"CVTLF", "Convert longword to F_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTLF,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, WF, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTLF) // Opcode Function
	},

	{
		"ACBF", "Add compare and branch F_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nACBF,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RF, RF, MF, BW, 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ACBF) // Opcode Function
	},

	{
		"MOVF", "Move F_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMOVF,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RF, WF, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVF) // Opcode Function
	},

	{
		"CMPF", "Compare F_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCMPF,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RF, RF, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CMPF) // Opcode Function
	},

	{
		"MNEGF", "Move negated F_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMNEGF,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RF, WF, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MNEGF) // Opcode Function
	},

	{
		"TSTF", "Test F_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nTSTF,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ RF, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, TSTF) // Opcode Function
	},

	{
		"EMODF", "Extended modulus F_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nEMODF,			// Opcode (Extended + Normal)
		5,							// Number of Operands
		{ RF, RB, RF, WL, WF, 0  }, // Operand Scale/Mode

		DEF_NAME(vax, EMODF) // Opcode Function
	},

	{
		"POLYF", "Evaluate polynomial F_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nPOLYF,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RF, RW, AB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, POLYF) // Opcode Function
	},

	{
		"CVTFD", "Convert F_floating to D_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTFD,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RF, WD, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTFD) // Opcode Function
	},

	{
		"ADAWI", "Add aligned word interlocked",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nADAWI,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, WW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ADAWI) // Opcode Function
	},

	{
		"INSQHI", "Insert into queue at head, interlocked",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nINSQHI,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ AB, AQ, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, INSQHI) // Opcode Function
	},

	{
		"INSQTI", "Insert into queue at tail, interlocked",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nINSQTI,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ AB, AQ, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, INSQTI) // Opcode Function
	},

	{
		"REMQHI", "Remove from queue at head, interlocked",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nREMQHI,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ AQ, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, REMQHI) // Opcode Function
	},

	{
		"REMQTI", "Remove from queue at tail, interlocked",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nREMQTI,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ AQ, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, REMQTI) // Opcode Function
	},

	{
		"ADDD2", "Add D_floating 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nADDD2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RD, MD, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ADDD) // Opcode Function
	},

	{
		"ADDD3", "Add D_floating 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nADDD3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RD, RD, WD, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ADDD) // Opcode Function
	},

	{
		"SUBD2", "Subtract D_floating 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nSUBD2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RD, MD, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, SUBD) // Opcode Function
	},

	{
		"SUBD3", "Subtract D_floating 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nSUBD3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RD, RD, WD, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, SUBD) // Opcode Function
	},

	{
		"MULD2", "Multiply D_floating 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMULD2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RD, MD, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MULD) // Opcode Function
	},

	{
		"MULD3", "Multiply D_floating 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMULD3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RD, RD, WD, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MULD) // Opcode Function
	},

	{
		"DIVD2", "Divide D_floating 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nDIVD2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RD, MD, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, DIVD) // Opcode Function
	},

	{
		"DIVD3", "Divide D_floating 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nDIVD3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RD, RD, WD, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, DIVD) // Opcode Function
	},

	{
		"CVTDB", "Convert D_floating to byte",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTDB,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RD, WB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTDB) // Opcode Function
	},

	{
		"CVTDW", "Convert D_floating to word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTDW,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RD, WW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTDW) // Opcode Function
	},

	{
		"CVTDL", "Convert D_floating to longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTDL,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RD, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTDL) // Opcode Function
	},

	{
		"CVTRDL", "Convert rounded D_floating to longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTRDL,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RD, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTRDL) // Opcode Function
	},

	{
		"CVTBD", "Convert byte to D_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTBD,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, WD, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTBD) // Opcode Function
	},

	{
		"CVTWD", "Convert word to D_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTWD,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, WD, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTWD) // Opcode Function
	},

	{
		"CVTLD", "Convert longword to D_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTLD,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, WD, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTLD) // Opcode Function
	},

	{
		"ACBD", "Add compare and branch D_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nACBD,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RD, RD, MD, BW, 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ACBD) // Opcode Function
	},

	{
		"MOVD", "Move D_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMOVD,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RD, WD, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVD) // Opcode Function
	},

	{
		"CMPD", "Compare D_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCMPD,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RD, RD, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CMPD) // Opcode Function
	},

	{
		"MNEGD", "Move negated D_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMNEGD,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RD, WD, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MNEGD) // Opcode Function
	},

	{
		"TSTD", "Test D_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nTSTD,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ RD, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, TSTD) // Opcode Function
	},

	{
		"EMODD", "Extended modulus D_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nEMODD,			// Opcode (Extended + Normal)
		5,							// Number of Operands
		{ RD, RB, RD, WL, WD, 0  }, // Operand Scale/Mode

		DEF_NAME(vax, EMODD) // Opcode Function
	},

	{
		"POLYD", "Evaluate polynomial D_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nPOLYD,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RD, RW, AB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, POLYD) // Opcode Function
	},

	{
		"CVTDF", "Convert D_floating to F_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTDF,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RD, WF, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTDF) // Opcode Function
	},

	{
		"ASHL", "Arithmetic shift longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nASHL,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RB, RL, WL, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ASHL) // Opcode Function
	},

	{
		"ASHQ", "Arithmetic shift quadword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nASHQ,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RB, RQ, WQ, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ASHQ) // Opcode Function
	},

	{
		"EMUL", "Extended multiply",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nEMUL,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RL, RL, RL, WQ, 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, EMUL) // Opcode Function
	},

	{
		"EDIV", "Extended divide",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nEDIV,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RL, RQ, WL, WL, 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, EDIV) // Opcode Function
	},

	{
		"CLRQ", "Clear quadword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCLRQ,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ WQ, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CLRQ) // Opcode Function
	},

	{
		"CLRD", "Clear D_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCLRQ,			// Opcode (Extended + Normal)
		1,							// Number of Operands: 1
		{ WD, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CLRQ) // Opcode Function
	},

	{
		"CLRG", "Clear G_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCLRQ,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ WG, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CLRQ) // Opcode Function
	},

	{
		"MOVQ", "Move quadword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMOVQ,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RQ, WQ, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVQ) // Opcode Function
	},

	{
		"MOVAQ", "Move address of quadword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMOVAQ,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ AQ, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVL) // Opcode Function
	},

	{
		"MOVAD", "Move address of D_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMOVAQ,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ AD, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVL) // Opcode Function
	},

	{
		"MOVAG", "Move address of G_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMOVAQ,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ AG, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVL) // Opcode Function
	},

	{
		"PUSHAQ", "Push address of quadword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nPUSHAQ,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ AQ, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, PUSHL) // Opcode Function
	},

	{
		"PUSHAD", "Push address of D_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nPUSHAQ,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ AD, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, PUSHL) // Opcode Function
	},

	{
		"PUSHAG", "Push address of G_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nPUSHAQ,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ AG, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, PUSHL) // Opcode Function
	},

	{
		"ADDB2", "Add byte 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nADDB2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, MB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ADDB) // Opcode Function
	},

	{
		"ADDB3", "Add byte 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nADDB3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RB, RB, WB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ADDB) // Opcode Function
	},

	{
		"SUBB2", "Subtract byte 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nSUBB2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, MB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, SUBB) // Opcode Function
	},

	{
		"SUBB3", "Subtract byte 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nSUBB3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RB, RB, WB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, SUBB) // Opcode Function
	},

	{
		"MULB2", "Multiply byte 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMULB2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, MB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MULB) // Opcode Function
	},

	{
		"MULB3", "Multiply byte 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMULB3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RB, RB, WB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MULB) // Opcode Function
	},

	{
		"DIVB2", "Divide byte 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nDIVB2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, MB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, DIVB) // Opcode Function
	},

	{
		"DIVB3", "Divide byte 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nDIVB3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RB, RB, WB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, DIVB) // Opcode Function
	},

	{
		"BISB2", "Bit set byte 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBISB2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, MB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BISB) // Opcode Function
	},

	{
		"BISB3", "Bit set byte 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBISB3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RB, RB, WB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BISB) // Opcode Function
	},

	{
		"BICB2", "Bit clear byte 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBICB2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, MB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BICB) // Opcode Function
	},

	{
		"BICB3", "Bit clear byte 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBICB3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RB, RB, WB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BICB) // Opcode Function
	},

	{
		"XORB2", "Exclusive-OR byte 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nXORB2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, MB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, XORB) // Opcode Function
	},

	{
		"XORB3", "Exclusive-OR byte 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nXORB3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RB, RB, WB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, XORB) // Opcode Function
	},

	{
		"MNEGB", "Move negated byte",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMNEGB,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, WB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MNEGB) // Opcode Function
	},

	{
		"CASEB", "Case byte",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCASEB,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RB, RB, RB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CASEB) // Opcode Function
	},

	{
		"MOVB", "Move byte",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMOVB,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, WB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVB) // Opcode Function
	},

	{
		"CMPB", "Compare byte",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCMPB,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, RB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CMPB) // Opcode Function
	},

	{
		"MCOMB", "Move complemented byte",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMCOMB,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, WB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MCOMB) // Opcode Function
	},

	{
		"BITB", "Bit test byte",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBITB,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, RB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BITB) // Opcode Function
	},

	{
		"CLRB", "Clear byte",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCLRB,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ WB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CLRB) // Opcode Function
	},

	{
		"TSTB", "Test byte",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nTSTB,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ RB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, TSTB) // Opcode Function
	},

	{
		"INCB", "Increment byte",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nINCB,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ MB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, INCB) // Opcode Function
	},

	{
		"DECB", "Decrement byte",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nDECB,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ MB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, DECB) // Opcode Function
	},

	{
		"CVTBL", "Convert byte to longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTBL,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTBL) // Opcode Function
	},

	{
		"CVTBW", "Convert byte to word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTBW,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, WW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTBW) // Opcode Function
	},

	{
		"MOVZBL", "Move zero-extended byte to longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMOVZBL,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVZBL) // Opcode Function
	},

	{
		"MOVZBW", "Move zero-extended byte to word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMOVZBW,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, WW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVZBW) // Opcode Function
	},

	{
		"ROTL", "Rotate longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nROTL,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RB, RL, WL, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ROTL) // Opcode Function
	},

	{
		"ACBB", "Add compare and branch byte",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nACBB,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RB, RB, MB, BW, 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ACBB) // Opcode Function
	},

	{
		"MOVAB", "Move address of byte",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMOVAB,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ AB, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVL) // Opcode Function
	},

	{
		"PUSHAB", "Move address of byte",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nPUSHAB,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ AB, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, PUSHL) // Opcode Function
	},

	{
		"ADDW2", "Add word 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nADDW2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, MW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ADDW) // Opcode Function
	},

	{
		"ADDW3", "Add Word 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nADDW3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RW, RW, WW, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ADDW) // Opcode Function
	},

	{
		"SUBW2", "Subtract Word 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nSUBW2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, MW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, SUBW) // Opcode Function
	},

	{
		"SUBW3", "Subtract word 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nSUBW3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RW, RW, WW, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, SUBW) // Opcode Function
	},

	{
		"MULW2", "Multiply word 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMULW2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, MW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MULW) // Opcode Function
	},

	{
		"MULW3", "Multiply word 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMULW3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RW, RW, WW, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MULW) // Opcode Function
	},

	{
		"DIVW2", "Divide word 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nDIVW2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, MW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, DIVW) // Opcode Function
	},

	{
		"DIVW3", "Divide word 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nDIVW3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RW, RW, WW, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, DIVW) // Opcode Function
	},

	{
		"BISW2", "Bit set word 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBISW2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, MW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BISW) // Opcode Function
	},

	{
		"BISW3", "Bit set word 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBISW3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RW, RW, WW, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BISW) // Opcode Function
	},

	{
		"BICW2", "Bit clear word 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBICW2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, MW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BICW) // Opcode Function
	},

	{
		"BICW3", "Bit clear word 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBICW3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RW, RW, WW, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BICW) // Opcode Function
	},

	{
		"XORW2", "Exclusive-OR word 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nXORW2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, MW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, XORW) // Opcode Function
	},

	{
		"XORW3", "Exclusive-OR word 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nXORW3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RW, RW, WW, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, XORW) // Opcode Function
	},

	{
		"MNEGW", "Move negated word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMNEGW,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, WW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MNEGW) // Opcode Function
	},

	{
		"CASEW", "Case word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCASEW,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RW, RW, RW, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CASEW) // Opcode Function
	},

	{
		"MOVW", "Move word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMOVW,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, WW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVW) // Opcode Function
	},

	{
		"CMPW", "Compare word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCMPW,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, RW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CMPW) // Opcode Function
	},

	{
		"MCOMW", "Move complemented word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMCOMW,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, WW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MCOMW) // Opcode Function
	},

	{
		"BITW", "Bit test word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBITW,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, RW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BITW) // Opcode Function
	},

	{
		"CLRW", "Clear word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCLRW,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ WW, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CLRW) // Opcode Function
	},

	{
		"TSTW", "Test word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nTSTW,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ RW, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, TSTW) // Opcode Function
	},

	{
		"INCW", "Increment word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nINCW,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ MW, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, INCW) // Opcode Function
	},

	{
		"DECW", "Decrement word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nDECW,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ MW, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, DECW) // Opcode Function
	},

	{
		"BISPSW", "Bit set program status word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBISPSW,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ RW, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BISPSW) // Opcode Function
	},

	{
		"BICPSW", "Bit clear program status word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBICPSW,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ RW, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BICPSW) // Opcode Function
	},

	{
		"POPR", "Pop registers",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nPOPR,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ RW, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, POPR) // Opcode Function
	},

	{
		"PUSHR", "Push registers",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nPUSHR,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ RW, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, PUSHR) // Opcode Function
	},

	{
		"CHMK", "Change mode to kernel",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCHMK,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ RW, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CHMK) // Opcode Function
	},

	{
		"CHME", "Change mode to executive",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCHME,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ RW, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CHME) // Opcode Function
	},

	{
		"CHMS", "Change mode to supervisor",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCHMS,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ RW, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CHMS) // Opcode Function
	},

	{
		"CHMU", "Change mode to user",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCHMU,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ RW, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CHMU) // Opcode Function
	},

	{
		"ADDL2", "Add longword 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nADDL2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, ML, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ADDL) // Opcode Function
	},

	{
		"ADDL3", "Add longword 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nADDL3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RL, RL, WL, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ADDL) // Opcode Function
	},

	{
		"SUBL2", "Subtract longword 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nSUBL2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, ML, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, SUBL) // Opcode Function
	},

	{
		"SUBL3", "Subtract longword 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nSUBL3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RL, RL, WL, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, SUBL) // Opcode Function
	},

	{
		"MULL2", "Multiply longword 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMULL2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, ML, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MULL) // Opcode Function
	},

	{
		"MULL3", "Multiply longword 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMULL3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RL, RL, WL, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MULL) // Opcode Function
	},

	{
		"DIVL2", "Divide longword 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nDIVL2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, ML, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, DIVL) // Opcode Function
	},

	{
		"DIVL3", "Divide longword 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nDIVL3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RL, RL, WL, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, DIVL) // Opcode Function
	},

	{
		"BISL2", "Bit set longword 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBISL2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, ML, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BISL) // Opcode Function
	},

	{
		"BISL3", "Bit set longword 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBISL3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RL, RL, WL, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BISL) // Opcode Function
	},

	{
		"BICL2", "Bit clear longword 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBICL2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, ML, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BICL) // Opcode Function
	},

	{
		"BICL3", "Bit clear longword 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBICL3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RL, RL, WL, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BICL) // Opcode Function
	},

	{
		"XORL2", "Exclusive-OR longword 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nXORL2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, ML, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, XORL) // Opcode Function
	},

	{
		"XORL3", "Exclusive-OR longword 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nXORL3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RL, RL, WL, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, XORL) // Opcode Function
	},

	{
		"MNEGL", "Move negated longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMNEGL,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MNEGL) // Opcode Function
	},

	{
		"CASEL", "Case longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCASEL,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RL, RL, RL, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CASEL) // Opcode Function
	},

	{
		"MOVL", "Move longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMOVL,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVL) // Opcode Function
	},

	{
		"CMPL", "Compare longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCMPL,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, RL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CMPL) // Opcode Function
	},

	{
		"MCOML", "Move complemented longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMCOML,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MCOML) // Opcode Function
	},

	{
		"BITL", "Bit test longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBITL,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, RL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BITL) // Opcode Function
	},

	{
		"CLRL", "Clear longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCLRL,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ WL, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CLRL) // Opcode Function
	},

	{
		"CLRF", "Clear longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCLRL,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ WF, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CLRL) // Opcode Function
	},

	{
		"TSTL", "Test longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nTSTL,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ RL, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, TSTL) // Opcode Function
	},

	{
		"INCL", "Increment longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nINCL,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ ML, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, INCL) // Opcode Function
	},

	{
		"DECL", "Decrement longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nDECL,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ ML, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, DECL) // Opcode Function
	},

	{
		"ADWC", "Add with carry",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nADWC,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, ML, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ADWC) // Opcode Function
	},

	{
		"SBWC", "Subtract with carry",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nSBWC,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, ML, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, SBWC) // Opcode Function
	},

	{
		"MTPR", "Move to processor register",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMTPR,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, RL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MTPR) // Opcode Function
	},

	{
		"MFPR", "Move from processor register",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMFPR,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MFPR) // Opcode Function
	},

	{
		"MOVPSL", "Move program status longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMOVPSL,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ WL, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVPSL) // Opcode Function
	},

	{
		"PUSHL", "Push longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nPUSHL,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ RL, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, PUSHL) // Opcode Function
	},

	{
		"MOVAL", "Move address of longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMOVAL,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ AL, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVL) // Opcode Function
	},

	{
		"MOVAF", "Move address of F_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nMOVAL,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ AF, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVL) // Opcode Function
	},

	{
		"PUSHAL", "Push address of longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nPUSHAL,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ AL, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, PUSHL) // Opcode Function
	},

	{
		"PUSHAF", "Push address of F_floating",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nPUSHAL,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ AF, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, PUSHL) // Opcode Function
	},

	{
		"BBS", "Branch on bit set",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBBS,				// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RL, VB, BB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BBS) // Opcode Function
	},

	{
		"BBC", "Branch on bit clear",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBBC,				// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RL, VB, BB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BBC) // Opcode Function
	},

	{
		"BBSS", "Branch on bit set and set",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBBSS,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RL, VB, BB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BBSS) // Opcode Function
	},

	{
		"BBCS", "Branch on bit clear and set",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBBCS,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RL, VB, BB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BBCS) // Opcode Function
	},

	{
		"BBSC", "Branch on bit set and clear",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBBSC,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RL, VB, BB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BBSC) // Opcode Function
	},

	{
		"BBCC", "Branch on bit clear and clear",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBBCC,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RL, VB, BB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BBCC) // Opcode Function
	},

	{
		"BBSSI", "Branch on bit set and set interlocked",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBBSSI,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RL, VB, BB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BBSS) // Opcode Function
	},

	{
		"BBCCI", "Branch on bit clear and clear interlocked",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBBCCI,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RL, VB, BB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BBCC) // Opcode Function
	},

	{
		"BLBS", "Branch on low bit set",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBLBS,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, BB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BLBS) // Opcode Function
	},

	{
		"BLBC", "Branch on low bit clear",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nBLBC,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, BB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BLBC) // Opcode Function
	},

	{
		"FFS", "Find first set bit",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nFFS,				// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RL, RB, VB, WL, 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, FFS) // Opcode Function
	},

	{
		"FFC", "Find first clear bit",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nFFC,				// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RL, RB, VB, WL, 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, FFC) // Opcode Function
	},

	{
		"CMPV", "Compare field",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCMPV,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RL, RB, VB, RL, 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CMPV) // Opcode Function
	},

	{
		"CMPZV", "Compare zero-extended field",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCMPZV,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RL, RB, VB, RL, 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CMPZV) // Opcode Function
	},

	{
		"EXTV", "Extract field",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nEXTV,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RL, RB, VB, WL, 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, EXTV) // Opcode Function
	},

	{
		"EXTZV", "Extract zero-extended field",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nEXTZV,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RL, RB, VB, WL, 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, EXTZV) // Opcode Function
	},

	{
		"INSV", "Insert field",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nINSV,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RL, RL, RB, VB, 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, INSV) // Opcode Function
	},

	{
		"ACBL", "Add compare and branch longword",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nACBL,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RL, RL, ML, BW, 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ACBL) // Opcode Function
	},

	{
		"AOBLSS", "Add one and branch on less",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nAOBLSS,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RL, ML, BB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, AOBLSS) // Opcode Function
	},

	{
		"AOBLEQ", "Add one and branch on less or equal",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nAOBLEQ,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RL, ML, BB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, AOBLEQ) // Opcode Function
	},

	{
		"SOBGEQ", "Subtract one and branch on greater or equal",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nSOBGEQ,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ ML, BB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, SOBGEQ) // Opcode Function
	},

	{
		"SOBGTR", "Subtract one and branch on greater",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nSOBGTR,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ ML, BB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, SOBGTR) // Opcode Function
	},

	{
		"CVTLB", "Convert longword to byte",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTLB,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, WB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTLB) // Opcode Function
	},

	{
		"CVTLW", "Convert longword to word",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCVTLW,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, WW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTLW) // Opcode Function
	},

	{
		"ASHP", "Arithmetic shift and round packed",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nASHP,			// Opcode (Extended + Normal)
		6,							// Number of Operands
		{ RB, RW, AB, RB, RW, AB }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CVTLP", "Convert longword to packed",
		OPC_EMULATE,				// Instruction Flags
		0x00, OPC_nCVTLP,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RL, RW, AB, 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CALLG", "Call with general argument list",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCALLG,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ AB, AB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CALLG) // Opcode Function
	},

	{
		"CALLS", "Call with stack",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nCALLS,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, AB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CALLS) // Opcode Function
	},

	{
		"XFC", "Extended function call",
		OPC_REGULAR,				// Instruction Flags
		0x00, OPC_nXFC,				// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, XFC) // Opcode Function
	},

	{
		"WAIT", "Wait",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nWAIT,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Opcode Function
	},

	{
		"MFVP", "Move from vector processor",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nMFVP,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CVTDH", "Convert D_floating to H_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTDH,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RD, WH, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CVTGF", "Convert G_floating to F_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTGH,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RG, WF, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTGF) // Opcode Function
	},

	{
		"VLDL", "Load longword vector from memory to vector register",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVLDL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VGATHL", "Gather longword vector from memory to vector register",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVGATHL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VLDQ", "Load quadword vector from memory to vector register",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVLDQ,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VGATHQ", "Gather quadword vector from memory to vector register",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVGATHQ,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"ADDG2", "Add G_floating 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nADDG2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RG, MG, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ADDG) // Opcode Function
	},

	{
		"ADDG3", "Add G_floating 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nADDG3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RG, RG, WG, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ADDG) // Opcode Function
	},

	{
		"SUBG2", "Subtract G_floating 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nSUBG2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RG, MG, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, SUBG) // Opcode Function
	},

	{
		"SUBG3", "Subtract G_floating 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nSUBG3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RG, RG, WG, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, SUBG) // Opcode Function
	},

	{
		"MULG2", "Multiply G_floating 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nMULG2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RG, MG, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MULG) // Opcode Function
	},

	{
		"MULG3", "Multiply G_floating 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nMULG3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RG, RG, WG, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MULG) // Opcode Function
	},

	{
		"DIVG2", "Divide G_floating 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nDIVG2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RG, MG, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, DIVG) // Opcode Function
	},

	{
		"DIVG3", "Divide G_floating 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nDIVG3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RG, RG, WG, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, DIVG) // Opcode Function
	},

	{
		"CVTGB", "Convert G_floating to byte",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTGB,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RG, WB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTGB) // Opcode Function
	},

	{
		"CVTGW", "Convert G_floating to word",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTGW,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RG, WW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTGW) // Opcode Function
	},

	{
		"CVTGL", "Convert G_floating to longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTGL,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RG, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTGL) // Opcode Function
	},

	{
		"CVTRGL", "Convert rounded G_floating to longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTRGL,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RG, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTRGL) // Opcode Function
	},

	{
		"CVTBG", "Convert byte to G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTBG,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, WG, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTBG) // Opcode Function
	},

	{
		"CVTWG", "Convert word to G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTWG,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, WG, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTWG) // Opcode Function
	},

	{
		"CVTLG", "Convert longword to G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTLG,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, WG, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTLG) // Opcode Function
	},

	{
		"ACBG", "Add compare and branch G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nACBG,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RG, RG, WG, BW, 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, ACBG) // Opcode Function
	},

	{
		"MOVG", "Move G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nMOVG,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RG, WG, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MOVG) // Opcode Function
	},

	{
		"CMPG", "Compare G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCMPG,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RG, RG, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CMPG) // Opcode Function
	},

	{
		"MNEGG", "Move negated G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nMNEGG,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RG, WG, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, MNEGG) // Opcode Function
	},

	{
		"TSTG", "Test G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nTSTG,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ RG, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, TSTG) // Opcode Function
	},

	{
		"EMODG", "Extended modulus G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nEMODG,			// Opcode (Extended + Normal)
		5,							// Number of Operands
		{ RG, RW, RG, WL, WG, 0  },

		DEF_NAME(vax, EMODG) // Opcode Function
	},

	{
		"POLYG", "Evaluate polynomial G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nPOLYG,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RG, RW, AB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, POLYG) // Opcode Function
	},

	{
		"CVTGH", "Convert G_floating to H_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTGH,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RG, WH, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"ADDH2", "Add H_floating 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nADDH2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RH, MH, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"ADDH3", "Add H_floating 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nADDH3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RH, RH, WH, 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"SUBH2", "Subtract H_floating 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nSUBH2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RH, MH, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"SUBH3", "Subtract H_floating 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nSUBH3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RH, RH, WH, 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"MULH2", "Multiply H_floating 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nMULH2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RH, MH, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"MULH3", "Multiply H_floating 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nMULH3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RH, RH, WH, 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"DIVH2", "Divide H_floating 2 operand",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nDIVH2,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RH, MH, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"DIVH3", "Divide H_floating 3 operand",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nDIVH3,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RH, RH, WH, 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CVTHB", "Convert H_floating to byte",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTHB,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RH, WB, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CVTHW", "Convert H_floating to word",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTHW,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RH, WW, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CVTHL", "Convert H_floating to longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTHL,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RH, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CVTRHL", "Convert rounded H_floating to longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTRHL,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RH, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CVTBH", "Convert byte to H_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTBH,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RB, WH, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CVTWH", "Convert word to H_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTWH,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RW, WH, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CVTLH", "Convert longword to H_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTLH,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RL, WH, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"ACBH", "Add compare and branch H_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nACBH,			// Opcode (Extended + Normal)
		4,							// Number of Operands
		{ RH, RH, WH, BW, 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"MOVH", "Move H_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nMOVH,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RH, WH, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CMPH", "Compare H_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCMPH,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RH, RH, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"MNEGH", "Move negated H_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nMNEGH,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RH, WH, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"TSTH", "Test H_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nTSTH,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ RH, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"EMODH", "Extended modulus H_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nEMODH,			// Opcode (Extended + Normal)
		5,							// Number of Operands
		{ RH, RW, RH, WL, WH, 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"POLYH", "Evaluate polynomial H_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nPOLYH,			// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RH, RW, AB, 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CVTHG", "Convert H_floating to G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTHG,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RH, RG, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CLRO", "Clear octaword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCLRO,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ WO, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CLRH", "Clear H_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCLRO,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ WH, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"MOVO", "Move octaword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nMOVO,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RO, WO, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"MOVAO", "Move address of octaword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nMOVAO,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ AO, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr
	},

	{
		"MOVAH", "Move address of H_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nMOVAO,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ AH, WL, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"PUSHAO", "Push address of octaword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nPUSHAO,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ AO, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"PUSHAH", "Push address of H_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nPUSHAO,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ AH, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVADDL", "Vector vector add longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVADDL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSADDL", "Vector scalar add longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSADDL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVADDG", "Vector vector add G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVADDG,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSADDG", "Vector scalar add G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSADDG,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVADDF", "Vector vector add F_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVADDF,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSADDF", "Vector scalar add F_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSADDF,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVADDD", "Vector vector add D_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVADDD,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSADDD", "Vector scalar add D_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSADDD,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVSUBL", "Vector vector subtract longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVSUBL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSSUBL", "Vector scalar subtract longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSSUBL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVSUBG", "Vector vector subtract G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVSUBG,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSSUBG", "Vector scalar subtract G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSSUBG,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVSUBF", "Vector vector subtract F_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVSUBF,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSSUBF", "Vector scalar subtract F_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSSUBF,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVSUBD", "Vector vector subtract D_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVSUBD,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSSUBD", "Vector scalar subtract D_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSSUBD,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CVTFH", "Convert F_floating to H_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTFH,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RF, WH, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CVTFG", "Convert F_floating to G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTFG,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RF, WG, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, CVTFG) // Opcode Function
	},

	{
		"PROBEVMR", "Probe read access",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nPROBEVMR,		// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RB, RW, AB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, PROBER) // Opcode Function
	},

	{
		"PROBEVMW", "Probe write access",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nPROBEVMW,		// Opcode (Extended + Normal)
		3,							// Number of Operands
		{ RB, RW, AB, 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, PROBEW) // Opcode Function
	},

	{
		"VSTL", "Store longword vector from vector register to memory",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSTL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSCATL", "Scatter longword vector from vector register to memory",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSCATL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSTQ", "Store quadword vector from vector register to memory",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSTQ,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSCATQ", "Scatter quadword vector from vector register to memory",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSCATQ,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVMULL", "Vector vector multiply longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVMULL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSMULL", "Vector scalar multiply longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSMULL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVMULG", "Vector vector multiply G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVMULG,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSMULG", "Vector scalar multiply G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSMULG,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVMULF", "Vector vector multiply F_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVMULF,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSMULF", "Vector scalar multiply F_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSMULF,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVMULD", "Vector vector multiply D_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVMULD,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSMULD", "Vector scalar multiply D_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSMULD,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSYNC", "Synchronize vector memory access",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSYNC,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"MTVP", "Move to vector processor",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nMTVP,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVDIVG", "Vector vector divide G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVDIVG,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSDIVG", "Vector scalar divide G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSDIVG,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVDIVF", "Vector vector divide F_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVDIVF,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSDIVF", "Vector scalar divide F_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSDIVF,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVDIVD", "Vector vector divide D_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVDIVD,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSDIVD", "Vector scalar divide D_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSDIVD,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVCMPL", "Vector vector compare longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVCMPL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSCMPL", "Vector scalar compare longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSCMPL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVCMPG", "Vector vector compare G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVCMPG,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSCMPG", "Vector scalar compare G_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSCMPG,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVCMPF", "Vector vector compare F_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVCMPF,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSCMPF", "Vector scalar compare F_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSCMPF,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVCMPD", "Vector vector compare D_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVCMPD,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSCMPD", "Vector scalar compare D_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSCMPD,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVBISL", "Vector vector bit set longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVBISL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSBISL", "Vector scalar bit set longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSBISL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVBICL", "Vector vector bit clear longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVBICL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSBICL", "Vector scalar bit clear longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSBICL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVSRLL", "Vector vector shift right logical longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVSRLL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSSRLL", "Vector scalar shift right logical longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSSRLL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVSLLL", "Vector vector shift left logical longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVSLLL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSSLLL", "Vector scalar shift left logical longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSSLLL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVXORL", "Vector vector exclusive-OR longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVXORL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSXORL", "Vector scalar exclusive-OR longword",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSXORL,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVCVT", "Vector convert",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVCVT,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"IOTA", "Generate compressed iota vector",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nIOTA,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VVMERGE", "Vector vector merge",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVVMERGE,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"VSMERGE", "Vector scalar merge",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nVSMERGE,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ 0 , 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CVTHF", "Convert H_floating to F_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTHF,			// Opcode (Extended + Normal)
		2,							// Number of Operands
		{ RH, WF, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"CVTHD", "Convert H_floating to D_floating",
		OPC_REGULAR,				// Instruction Flags
		0xFD, OPC_nCVTHD,			// Opcode (Extended + Normal)
		0,							// Number of Operands
		{ RH, WD, 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		nullptr       // Execute Routine
	},

	{
		"BUGL", "Bug Check Long",
		OPC_REGULAR,				// Instruction Flags
		0xFF, OPC_nBUGL,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ IL, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BUGL) // Opcode Function
	},

	{
		"BUGW", "Bug Check Word",
		OPC_REGULAR,				// Instruction Flags
		0xFF, OPC_nBUGW,			// Opcode (Extended + Normal)
		1,							// Number of Operands
		{ IW, 0 , 0 , 0 , 0 , 0  }, // Operand Scale/Mode

		DEF_NAME(vax, BUGW) // Opcode Function
	},

	// null terminator here
	{ nullptr }
};
#endif
