/*
 * disasm.cpp
 *
 *  Created on: Mar 21, 2017
 *      Author: Timothy Stark
 *
 *  Disassembler utility
 *
 */

#include "emu/core.h"
#include "emu/devcpu.h"
#include "dev/cpu/vax/vax.h"
#include "dev/cpu/vax/opcodes.h"

static const char *regNames[] =
{
		"R0",  "R1",  "R2",  "R3",  "R4",  "R5",  "R6",  "R7",
		"R8",  "R9",  "R10", "R11", "AP",  "FP",  "SP",  "PC"
};


int vax_cpuDevice::disasmOperand(char **ptr, uint32_t &vAddr, const vaxOpcode *opc, int opn, bool idxFlag)
{
	int      oprFlags = opc->oprMode[opn];
	int      scale    = oprFlags & 0xFF;
	uint32_t oprType;
	uint32_t data;
	int      mode, reg;
	char     fmt[64];

	if ((opn > 0) && !idxFlag)
		*(*ptr)++ = ',';

	if (oprFlags & OPR_IMMED) {
		readci(vAddr, &data, scale);
		vAddr += scale;
		sprintf(fmt, "#%%0%dX", scale*2);
		*ptr += sprintf(*ptr, fmt, data);

		return 0;
	}

	if (oprFlags & OPR_BRANCH) {
		readci(vAddr, &data, scale);
		vAddr += scale;

		// Extend signed bit
		data = (scale == LN_BYTE) ? int8_t(data)  :
			   (scale == LN_WORD) ? int16_t(data) : data;

		*ptr += sprintf(*ptr, "%08X", vAddr + int32_t(data));

		return 0;
	}

	readci(vAddr++, &oprType, LN_BYTE);
	mode = (oprType >> 4) & 0x0F;
	reg  = oprType & 0x0F;

	switch (mode) {
	case 0x00: case 0x01: // Literal
	case 0x02: case 0x03:
		*ptr += sprintf(*ptr, "S^#%02X", oprType);
		break;

	case 0x04: // Indexed
		disasmOperand(ptr, vAddr, opc, opn, true);
		*ptr += sprintf(*ptr, "[%s]", regNames[reg]);
		break;

	case 0x05: // Register
		*ptr += sprintf(*ptr, "%s", regNames[reg]);
		break;

	case 0x06: // Register deferred
		*ptr += sprintf(*ptr, "(%s)", regNames[reg]);
		break;

	case 0x07: // Autodecrement
		*ptr += sprintf(*ptr, "-(%s)", regNames[reg]);
		break;

	case 0x08: // Autoincrement/Immediate
		if (reg < 0x0F)
			*ptr += sprintf(*ptr, "(%s)+", regNames[reg]);
		else {
			readci(vAddr, &data, scale);
			switch (scale) {
			case LN_BYTE:
				*ptr += sprintf(*ptr, "I^#%02X", data);
				break;
			case LN_WORD:
				*ptr += sprintf(*ptr, "I^#%04X", data);
				break;
			case LN_LONG:
			case LN_QUAD:
				*ptr += sprintf(*ptr, "I^#%08X", data);
				break;
			}
			vAddr += scale;
		}
		break;

	case 0x09: // Autoincrement deferred/Absolute
		if (reg < 0x0F)
			*ptr += sprintf(*ptr, "@(%s)+", regNames[reg]);
		else {
			readci(vAddr, &data, LN_LONG);
			*ptr += sprintf(*ptr, "@#%08X", data);
			vAddr += LN_LONG;
		}
		break;

	case 0x0A: // Byte displacement/relative
		readci(vAddr++, &data, LN_BYTE);
		*ptr += sprintf(*ptr, "B^%02X", data);
		if (reg < 0x0F)
			*ptr += sprintf(*ptr, "(%s)", regNames[reg]);
		break;

	case 0x0B: // Deferred byte displacement/relative
		readci(vAddr++, &data, LN_BYTE);
		*ptr += sprintf(*ptr, "@B^%02X", data);
		if (reg < 0x0F)
			*ptr += sprintf(*ptr, "(%s)", regNames[reg]);
		break;

	case 0x0C: // Word displacement/relative
		readci(vAddr, &data, LN_WORD);
		vAddr += LN_WORD;
		*ptr += sprintf(*ptr, "W^%04X", data);
		if (reg < 0x0F)
			*ptr += sprintf(*ptr, "(%s)", regNames[reg]);
		break;

	case 0x0D: // Deferred word displacement/relative
		readci(vAddr, &data, LN_WORD);
		vAddr += LN_WORD;
		*ptr += sprintf(*ptr, "@W^%04X", data);
		if (reg < 0x0F)
			*ptr += sprintf(*ptr, "(%s)", regNames[reg]);
		break;

	case 0x0E: // Longword displacement/relative
		readci(vAddr, &data, LN_LONG);
		vAddr += LN_LONG;
		*ptr += sprintf(*ptr, "L^%08X", data);
		if (reg < 0x0F)
			*ptr += sprintf(*ptr, "(%s)", regNames[reg]);
		break;

	case 0x0F: // Deferred longword displacement/relative
		readci(vAddr, &data, LN_LONG);
		vAddr += LN_LONG;
		*ptr += sprintf(*ptr, "@L^%08X", data);
		if (reg < 0x0F)
			*ptr += sprintf(*ptr, "(%s)", regNames[reg]);
		break;
	}

	return 0;
}

int vax_cpuDevice::disasm(uint32_t vAddr)
{
	char       line[256], *ptr = line;
	uint32_t   opCode, opExtend;
	uint32_t   pcAddr = vAddr;
	const vaxOpcode *opc;

	ptr += sprintf(ptr, "%08X ", pcAddr);

	// Fetch 1 or 2 bytes opcode
	readci(pcAddr++, &opCode, OPR_BYTE);
	if (opCode > OPC_nEXTEND) {
		opExtend = (opCode - OPC_nEXTEND) << 8;
		readci(pcAddr++, &opCode, OPR_BYTE);
		opCode |= opExtend;
	}
	opc = opCodes[opCode];

	if (opc != nullptr) {
		ptr += sprintf(ptr, "%-8s", opc->opName);
		for (int opn = 0; opn < opc->nOperands; opn++)
			disasmOperand(&ptr, pcAddr, opc, opn, false);
	} else {
		if (opCode > OPC_nEXTEND)
			ptr += sprintf(ptr, ".BYTE %02X,%02X", (opCode >> 8) + OPC_nEXTEND, opCode & 0xFF);
		else
			ptr += sprintf(ptr, ".BYTE %02X", opCode);
	}

#ifdef ENABLE_DEBUG
	dbg::log("%s", line);
#else
	printf("%s\n", line);
#endif /* ENABLE_DEBUG */

	return pcAddr - vAddr;
}
