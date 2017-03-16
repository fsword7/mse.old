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
#include "dev/cpu/vax/opcodes.h"

#define OPC_nHALT		0x00
#define OPC_nNOP		0x01
#define OPC_nREI		0x02
#define OPC_nBPT		0x03
#define OPC_nRET		0x04
#define OPC_nRSB		0x05
#define OPC_nLDPCTX		0x06
#define OPC_nSVPCTX		0x07

#define OPC_nCVTPS		0x08
#define OPC_nCVTSP		0x09
#define OPC_nINDEX		0x0A
#define OPC_nCRC		0x0B
#define OPC_nPROBER		0x0C
#define OPC_nPROBEW		0x0D
#define OPC_nINSQUE		0x0E
#define OPC_nREMQUE		0x0F

#define OPC_nBSBB		0x10
#define OPC_nBRB		0x11
#define OPC_nBNEQ		0x12
#define OPC_nBEQL		0x13
#define OPC_nBGTR		0x14
#define OPC_nBLEQ		0x15
#define OPC_nJSB		0x16
#define OPC_nJMP		0x17

#define OPC_nBGEQ		0x18
#define OPC_nBLSS		0x19
#define OPC_nBGTRU		0x1A
#define OPC_nBLEQU		0x1B
#define OPC_nBVC		0x1C
#define OPC_nBVS		0x1D
#define OPC_nBCC		0x1E
#define OPC_nBCS		0x1F

#define OPC_nADDP4		0x20
#define OPC_nADDP6		0x21
#define OPC_nSUBP4		0x22
#define OPC_nSUBP6		0x23
#define OPC_nCVTPT		0x24
#define OPC_nMULP6		0x25
#define OPC_nCVTTP		0x26
#define OPC_nDIVP6		0x27

#define OPC_nMOVC3		0x28
#define OPC_nCMPC3		0x29
#define OPC_nSCANC		0x2A
#define OPC_nSPANC		0x2B
#define OPC_nMOVC5		0x2C
#define OPC_nCMPC5		0x2D
#define OPC_nMOVTC		0x2E
#define OPC_nMOVTUC		0x2F

#define OPC_nBSBW		0x30
#define OPC_nBRW		0x31
#define OPC_nCVTWL		0x32
#define OPC_nCVTWB		0x33
#define OPC_nMOVP		0x34
#define OPC_nCMPP3		0x35
#define OPC_nCVTPL		0x36
#define OPC_nCMPP4		0x37

#define OPC_nEDITPC		0x38
#define OPC_nMATCHC		0x39
#define OPC_nLOCC		0x3A
#define OPC_nSKPC		0x3B
#define OPC_nMOVZWL		0x3C
#define OPC_nACBW		0x3D
#define OPC_nMOVAW		0x3E
#define OPC_nPUSHAW		0x3F

#define OPC_nADDF2		0x40
#define OPC_nADDF3		0x41
#define OPC_nSUBF2		0x42
#define OPC_nSUBF3		0x43
#define OPC_nMULF2		0x44
#define OPC_nMULF3		0x45
#define OPC_nDIVF2		0x46
#define OPC_nDIVF3		0x47

#define OPC_nCVTFB		0x48
#define OPC_nCVTFW		0x49
#define OPC_nCVTFL		0x4A
#define OPC_nCVTRFL		0x4B
#define OPC_nCVTBF		0x4C
#define OPC_nCVTWF		0x4D
#define OPC_nCVTLF		0x4E
#define OPC_nACBF		0x4F

#define OPC_nMOVF		0x50
#define OPC_nCMPF		0x51
#define OPC_nMNEGF		0x52
#define OPC_nTSTF		0x53
#define OPC_nEMODF		0x54
#define OPC_nPOLYF		0x55
#define OPC_nCVTFD		0x56
// Reserved				0x57

#define OPC_nADAWI		0x58
// Reserved				0x59
// Reserved				0x5A
// Reserved				0x5B
#define OPC_nINSQHI		0x5C
#define OPC_nINSQTI		0x5D
#define OPC_nREMQHI		0x5E
#define OPC_nREMQTI		0x5F

#define OPC_nADDD2		0x60
#define OPC_nADDD3		0x61
#define OPC_nSUBD2		0x62
#define OPC_nSUBD3		0x63
#define OPC_nMULD2		0x64
#define OPC_nMULD3		0x65
#define OPC_nDIVD2		0x66
#define OPC_nDIVD3		0x67

#define OPC_nCVTDB		0x68
#define OPC_nCVTDW		0x69
#define OPC_nCVTDL		0x6A
#define OPC_nCVTRDL		0x6B
#define OPC_nCVTBD		0x6C
#define OPC_nCVTWD		0x6D
#define OPC_nCVTLD		0x6E
#define OPC_nACBD		0x6F

#define OPC_nMOVD		0x70
#define OPC_nCMPD		0x71
#define OPC_nMNEGD		0x72
#define OPC_nTSTD		0x73
#define OPC_nEMODD		0x74
#define OPC_nPOLYD		0x75
#define OPC_nCVTDF		0x76
// Reserved				0x77

#define OPC_nASHL		0x78
#define OPC_nASHQ		0x79
#define OPC_nEMUL		0x7A
#define OPC_nEDIV		0x7B
#define OPC_nCLRQ		0x7C
#define OPC_nMOVQ		0x7D
#define OPC_nMOVAQ		0x7E
#define OPC_nPUSHAQ		0x7F

#define OPC_nADDB2		0x80
#define OPC_nADDB3		0x81
#define OPC_nSUBB2		0x82
#define OPC_nSUBB3		0x83
#define OPC_nMULB2		0x84
#define OPC_nMULB3		0x85
#define OPC_nDIVB2		0x86
#define OPC_nDIVB3		0x87

#define OPC_nBISB2		0x88
#define OPC_nBISB3		0x89
#define OPC_nBICB2		0x8A
#define OPC_nBICB3		0x8B
#define OPC_nXORB2		0x8C
#define OPC_nXORB3		0x8D
#define OPC_nMNEGB		0x8E
#define OPC_nCASEB		0x8F

#define OPC_nMOVB		0x90
#define OPC_nCMPB		0x91
#define OPC_nMCOMB		0x92
#define OPC_nBITB		0x93
#define OPC_nCLRB		0x94
#define OPC_nTSTB		0x95
#define OPC_nINCB		0x96
#define OPC_nDECB		0x97

#define OPC_nCVTBL		0x98
#define OPC_nCVTBW		0x99
#define OPC_nMOVZBL		0x9A
#define OPC_nMOVZBW		0x9B
#define OPC_nROTL		0x9C
#define OPC_nACBB		0x9D
#define OPC_nMOVAB		0x9E
#define OPC_nPUSHAB		0x9F

#define OPC_nADDW2		0xA0
#define OPC_nADDW3		0xA1
#define OPC_nSUBW2		0xA2
#define OPC_nSUBW3		0xA3
#define OPC_nMULW2		0xA4
#define OPC_nMULW3		0xA5
#define OPC_nDIVW2		0xA6
#define OPC_nDIVW3		0xA7

#define OPC_nBISW2		0xA8
#define OPC_nBISW3		0xA9
#define OPC_nBICW2		0xAA
#define OPC_nBICW3		0xAB
#define OPC_nXORW2		0xAC
#define OPC_nXORW3		0xAD
#define OPC_nMNEGW		0xAE
#define OPC_nCASEW		0xAF

#define OPC_nMOVW		0xB0
#define OPC_nCMPW		0xB1
#define OPC_nMCOMW		0xB2
#define OPC_nBITW		0xB3
#define OPC_nCLRW		0xB4
#define OPC_nTSTW		0xB5
#define OPC_nINCW		0xB6
#define OPC_nDECW		0xB7

#define OPC_nBISPSW		0xB8
#define OPC_nBICPSW		0xB9
#define OPC_nPOPR		0xBA
#define OPC_nPUSHR		0xBB
#define OPC_nCHMK		0xBC
#define OPC_nCHME		0xBD
#define OPC_nCHMS		0xBE
#define OPC_nCHMU		0xBF

#define OPC_nADDL2		0xC0
#define OPC_nADDL3		0xC1
#define OPC_nSUBL2		0xC2
#define OPC_nSUBL3		0xC3
#define OPC_nMULL2		0xC4
#define OPC_nMULL3		0xC5
#define OPC_nDIVL2		0xC6
#define OPC_nDIVL3		0xC7

#define OPC_nBISL2		0xC8
#define OPC_nBISL3		0xC9
#define OPC_nBICL2		0xCA
#define OPC_nBICL3		0xCB
#define OPC_nXORL2		0xCC
#define OPC_nXORL3		0xCD
#define OPC_nMNEGL		0xCE
#define OPC_nCASEL		0xCF

#define OPC_nMOVL		0xD0
#define OPC_nCMPL		0xD1
#define OPC_nMCOML		0xD2
#define OPC_nBITL		0xD3
#define OPC_nCLRL		0xD4
#define OPC_nTSTL		0xD5
#define OPC_nINCL		0xD6
#define OPC_nDECL		0xD7

#define OPC_nADWC		0xD8
#define OPC_nSBWC		0xD9
#define OPC_nMTPR		0xDA
#define OPC_nMFPR		0xDB
#define OPC_nMOVPSL		0xDC
#define OPC_nPUSHL		0xDD
#define OPC_nMOVAL		0xDE
#define OPC_nPUSHAL		0xDF

#define OPC_nBBS		0xE0
#define OPC_nBBC		0xE1
#define OPC_nBBSS		0xE2
#define OPC_nBBCS		0xE3
#define OPC_nBBSC		0xE4
#define OPC_nBBCC		0xE5
#define OPC_nBBSSI		0xE6
#define OPC_nBBCCI		0xE7

#define OPC_nBLBS		0xE8
#define OPC_nBLBC		0xE9
#define OPC_nFFS		0xEA
#define OPC_nFFC		0xEB
#define OPC_nCMPV		0xEC
#define OPC_nCMPZV		0xED
#define OPC_nEXTV		0xEE
#define OPC_nEXTZV		0xEF

#define OPC_nINSV		0xF0
#define OPC_nACBL		0xF1
#define OPC_nAOBLSS		0xF2
#define OPC_nAOBLEQ		0xF3
#define OPC_nSOBGEQ		0xF4
#define OPC_nSOBGTR		0xF5
#define OPC_nCVTLB		0xF6
#define OPC_nCVTLW		0xF7

#define OPC_nASHP		0xF8
#define OPC_nCVTLP		0xF9
#define OPC_nCALLG		0xFA
#define OPC_nCALLS		0xFB
#define OPC_nXFC		0xFC
#define OPC_nEXTEND		0xFD
// Two-Byte Opcode		0xFE
// Two-Byte Opcode		0xFF


// FD - Two-Byte Opcode Table
// Reserved				0x00
// Reserved				0x01
#define OPC_nWAIT		0x02
// Reserved				0x03
// Reserved				0x04
// Reserved				0x05
// Reserved				0x06
// Reserved				0x07

// Reserved				0x08
// Reserved				0x09
// Reserved				0x0A
// Reserved				0x0B
// Reserved				0x0C
// Reserved				0x0D
// Reserved				0x0E
// Reserved				0x0F

// Reserved				0x10
// Reserved				0x11
// Reserved				0x12
// Reserved				0x13
// Reserved				0x14
// Reserved				0x15
// Reserved				0x16
// Reserved				0x17

// Reserved				0x18
// Reserved				0x19
// Reserved				0x1A
// Reserved				0x1B
// Reserved				0x1C
// Reserved				0x1D
// Reserved				0x1E
// Reserved				0x1F

// Reserved				0x20
// Reserved				0x21
// Reserved				0x22
// Reserved				0x23
// Reserved				0x24
// Reserved				0x25
// Reserved				0x26
// Reserved				0x27

// Reserved				0x28
// Reserved				0x29
// Reserved				0x2A
// Reserved				0x2B
// Reserved				0x2C
// Reserved				0x2D
// Reserved				0x2E
// Reserved				0x2F

// Reserved				0x30
#define OPC_nMFVP		0x31
#define OPC_nCVTDH		0x32
#define OPC_nCVTGF		0x33
#define OPC_nVLDL		0x34
#define OPC_nVGATHL		0x35
#define OPC_nVLDQ		0x36
#define OPC_nVGATHQ		0x37

// Reserved				0x38
// Reserved				0x39
// Reserved				0x3A
// Reserved				0x3B
// Reserved				0x3C
// Reserved				0x3D
// Reserved				0x3E
// Reserved				0x3F

#define OPC_nADDG2		0x40
#define OPC_nADDG3		0x41
#define OPC_nSUBG2		0x42
#define OPC_nSUBG3		0x43
#define OPC_nMULG2		0x44
#define OPC_nMULG3		0x45
#define OPC_nDIVG2		0x46
#define OPC_nDIVG3		0x47

#define OPC_nCVTGB		0x48
#define OPC_nCVTGW		0x49
#define OPC_nCVTGL		0x4A
#define OPC_nCVTRGL		0x4B
#define OPC_nCVTBG		0x4C
#define OPC_nCVTWG		0x4D
#define OPC_nCVTLG		0x4E
#define OPC_nACBG		0x4F

#define OPC_nMOVG		0x50
#define OPC_nCMPG		0x51
#define OPC_nMNEGG		0x52
#define OPC_nTSTG		0x53
#define OPC_nEMODG		0x54
#define OPC_nPOLYG		0x55
#define OPC_nCVTGH		0x56
// Reserved				0x57

// Reserved				0x58
// Reserved				0x59
// Reserved				0x5A
// Reserved				0x5B
// Reserved				0x5C
// Reserved				0x5D
// Reserved				0x5E
// Reserved				0x5F

#define OPC_nADDH2		0x60
#define OPC_nADDH3		0x61
#define OPC_nSUBH2		0x62
#define OPC_nSUBH3		0x63
#define OPC_nMULH2		0x64
#define OPC_nMULH3		0x65
#define OPC_nDIVH2		0x66
#define OPC_nDIVH3		0x67

#define OPC_nCVTHB		0x68
#define OPC_nCVTHW		0x69
#define OPC_nCVTHL		0x6A
#define OPC_nCVTRHL		0x6B
#define OPC_nCVTBH		0x6C
#define OPC_nCVTWH		0x6D
#define OPC_nCVTLH		0x6E
#define OPC_nACBH		0x6F

#define OPC_nMOVH		0x70
#define OPC_nCMPH		0x71
#define OPC_nMNEGH		0x72
#define OPC_nTSTH		0x73
#define OPC_nEMODH		0x74
#define OPC_nPOLYH		0x75
#define OPC_nCVTHG		0x76
// Reserved				0x77

// Reserved				0x78
// Reserved				0x79
// Reserved				0x7A
// Reserved				0x7B
#define OPC_nCLRO		0x7C
#define OPC_nMOVO		0x7D
#define OPC_nMOVAO		0x7E
#define OPC_nPUSHAO		0x7F

#define OPC_nVVADDL		0x80
#define OPC_nVSADDL		0x81
#define OPC_nVVADDG		0x82
#define OPC_nVSADDG		0x83
#define OPC_nVVADDF		0x84
#define OPC_nVSADDF		0x85
#define OPC_nVVADDD		0x86
#define OPC_nVSADDD		0x87

#define OPC_nVVSUBL		0x88
#define OPC_nVSSUBL		0x89
#define OPC_nVVSUBG		0x8A
#define OPC_nVSSUBG		0x8B
#define OPC_nVVSUBF		0x8C
#define OPC_nVSSUBF		0x8D
#define OPC_nVVSUBD		0x8E
#define OPC_nVSSUBD		0x8F

// Reserved				0x90
// Reserved				0x91
// Reserved				0x92
// Reserved				0x93
// Reserved				0x94
// Reserved				0x95
// Reserved				0x96
// Reserved				0x97

#define OPC_nCVTFH		0x98
#define OPC_nCVTFG		0x99
#define OPC_nPROBEVMR	0x9A
#define OPC_nPROBEVMW	0x9B
#define OPC_nVSTL		0x9C
#define OPC_nVSCATL		0x9D
#define OPC_nVSTQ		0x9E
#define OPC_nVSCATQ		0x9F

#define OPC_nVVMULL		0xA0
#define OPC_nVSMULL		0xA1
#define OPC_nVVMULG		0xA2
#define OPC_nVSMULG		0xA3
#define OPC_nVVMULF		0xA4
#define OPC_nVSMULF		0xA5
#define OPC_nVVMULD		0xA6
#define OPC_nVSMULD		0xA7

#define OPC_nVSYNC		0xA8
#define OPC_nMTVP		0xA9
#define OPC_nVVDIVG		0xAA
#define OPC_nVSDIVG		0xAB
#define OPC_nVVDIVF		0xAC
#define OPC_nVSDIVF		0xAD
#define OPC_nVVDIVD		0xAE
#define OPC_nVSDIVD		0xAF

// Reserved				0xB0
// Reserved				0xB1
// Reserved				0xB2
// Reserved				0xB3
// Reserved				0xB4
// Reserved				0xB5
// Reserved				0xB6
// Reserved				0xB7

// Reserved				0xB8
// Reserved				0xB9
// Reserved				0xBA
// Reserved				0xBB
// Reserved				0xBC
// Reserved				0xBD
// Reserved				0xBE
// Reserved				0xBF

#define OPC_nVVCMPL		0xC0
#define OPC_nVSCMPL		0xC1
#define OPC_nVVCMPG		0xC2
#define OPC_nVSCMPG		0xC3
#define OPC_nVVCMPF		0xC4
#define OPC_nVSCMPF		0xC5
#define OPC_nVVCMPD		0xC6
#define OPC_nVSCMPD		0xC7

#define OPC_nVVBISL		0xC8
#define OPC_nVSBISL		0xC9
// Illegal Vector		0xCA
// Illegal Vector		0xCB
#define OPC_nVVBICL		0xCC
#define OPC_nVSBICL		0xCD
// Illegal Vector		0xCE
// Illegal Vector		0xCF

// Reserved				0xD0
// Reserved				0xD1
// Reserved				0xD2
// Reserved				0xD3
// Reserved				0xD4
// Reserved				0xD5
// Reserved				0xD6
// Reserved				0xD7

// Reserved				0xD8
// Reserved				0xD9
// Reserved				0xDA
// Reserved				0xDB
// Reserved				0xDC
// Reserved				0xDD
// Reserved				0xDE
// Reserved				0xDF

#define OPC_nVVSRLL		0xE0
#define OPC_nVSSRLL		0xE1
// Illegal Vector		0xE2
// Illegal Vector		0xE3
#define OPC_nVVSLLL		0xE4
#define OPC_nVSSLLL		0xE5
// Illegal Vector		0xE6
// Illegal Vector		0xE7

#define OPC_nVVXORL		0xE8
#define OPC_nVSXORL		0xE9
// Illegal Vector		0xEA
// Illegal Vector		0xEB
#define OPC_nVVCVT		0xEC
#define OPC_nIOTA		0xED
#define OPC_nVVMERGE	0xEE
#define OPC_nVSMERGE	0xEF

// Reserved				0xF0
// Reserved				0xF1
// Reserved				0xF2
// Reserved				0xF3
// Reserved				0xF4
// Reserved				0xF5
#define OPC_nCVTHF		0xF6
#define OPC_nCVTHD		0xF7

// Reserved				0xF8
// Reserved				0xF9
// Reserved				0xFA
// Reserved				0xFB
// Reserved				0xFC
// Reserved				0xFD
// Reserved				0xFE
// Reserved				0xFF

// FE - Two-Byte Opcode Table
// Reserved				0x00 - 0xFF

// FF - Two Byte Opcode Table
// Reserved				0x00 - 0xF7

// Reserved				0xF8
// Reserved				0xF9
// Reserved				0xFA
// Reserved				0xFB
#define OPC_nBUGL		0xFD
#define OPC_nBUGW		0xFE
// Reserved all time 	0xFF



#define DEF_NAME(cpu, opc) nullptr

const vaxOpcode vax_Instruction[] =
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

#if 0
#define OPC_REG OPC_REGULAR
#define OPC_EMU OPC_EMULATE

#define INST(name, flags, extend, opcode, nopr, opr) \
	{ name, nullptr, flags, extend, opcode, nopr, opr, nullptr }

const vaxOpcode vax_Opcodes2[] = {
	INST("BUGL", OPC_REG, 0xFF, OPC_nBUGL, 1, { IL, 0 , 0 , 0 , 0 , 0  }),
	INST("BUGW", OPC_REG, 0xFF, OPC_nBUGW, 1, { IL, 0 , 0 , 0 , 0 , 0  })

	// null terminator
	{ nullptr }
};
#endif
