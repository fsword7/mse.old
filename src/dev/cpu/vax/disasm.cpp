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
#include "emu/debug.h"
#include "emu/devcpu.h"
#include "dev/cpu/vax/vax.h"
#include "dev/cpu/vax/opcodes.h"

static const char *regNames[] =
{
		"R0",  "R1",  "R2",  "R3",  "R4",  "R5",  "R6",  "R7",
		"R8",  "R9",  "R10", "R11", "AP",  "FP",  "SP",  "PC"
};


int vax_cpuDevice::disasmOperand(uint32_t &vAddr, const vaxOpcode *opc, char **ptr)
{
	int      opMode, opScale;
	uint32_t opType;
	uint32_t data;
	uint32_t sts;
	int      rn, irn;


	for (int opn = 0; opn < opc->nOperands; opn++) {
		if (opn > 0)
			*(*ptr)++ = ',';

		opMode  = opc->oprMode[opn];
		opScale = OPM_SIZE(opMode);

		switch (opMode) {
		case BB: case BW:
			data = readc(vAddr, opScale, &sts);
			vAddr += opScale;
			// Extend signed bit
			data = (opScale == LN_BYTE) ? SXTB(data) : SXTW(data);
			*ptr += sprintf(*ptr, "%08X", vAddr + data);
			continue;

		case IW:
			data = readc(vAddr, opScale, &sts);
			vAddr += opScale;
			*ptr += sprintf(*ptr, "%04X", data);
			continue;

		case IL:
			data = readc(vAddr, opScale, &sts);
			vAddr += opScale;
			*ptr += sprintf(*ptr, "%08X", data);
			continue;

		default:
			opType = readc(vAddr++, LN_BYTE, &sts);
			if ((opType & OPR_MMASK) == IDX) {
				irn = opType & OPR_RMASK;
				opType = readc(vAddr++, LN_BYTE, &sts);
			} else
				irn = -1;
			rn = opType & OPR_RMASK;
			break;
		}

		switch (opType & OPR_MMASK) {
		case LIT0: case LIT1: // Literal
		case LIT2: case LIT3:
			*ptr += sprintf(*ptr, "S^#%02X", opType);
			break;

		case REG: // Register
			*ptr += sprintf(*ptr, "%s", regNames[rn]);
			break;

		case REGD: // Register deferred
			*ptr += sprintf(*ptr, "(%s)", regNames[rn]);
			break;

		case ADEC: // Autodecrement
			*ptr += sprintf(*ptr, "-(%s)", regNames[rn]);
			break;

		case AINC: // Autoincrement/Immediate
			if (rn < REG_nPC)
				*ptr += sprintf(*ptr, "(%s)+", regNames[rn]);
			else {
				data = readc(vAddr, opScale, &sts);
				switch (opScale) {
				case LN_BYTE:
					*ptr += sprintf(*ptr, "#%02X", data);
					break;
				case LN_WORD:
					*ptr += sprintf(*ptr, "#%04X", data);
					break;
				case LN_LONG:
					*ptr += sprintf(*ptr, "#%08X", data);
					break;
				case LN_QUAD:
					*ptr  += sprintf(*ptr, "#%08X", data);
					data  = readc(vAddr+LN_LONG, LN_LONG, &sts);
					*ptr  += sprintf(*ptr, "%08X", data);
					break;
				case LN_OCTA:
					*ptr  += sprintf(*ptr, "#%08X", data);
					data  = readc(vAddr+LN_LONG, LN_LONG, &sts);
					*ptr  += sprintf(*ptr, "%08X", data);
					data  = readc(vAddr+(LN_LONG*2), LN_LONG, &sts);
					*ptr  += sprintf(*ptr, "%08X", data);
					data  = readc(vAddr+(LN_LONG*3), LN_LONG, &sts);
					*ptr  += sprintf(*ptr, "%08X", data);
					break;
				}
				vAddr += opScale;
			}
			break;

		case AINCD: // Autoincrement deferred/Absolute
			if (rn < REG_nPC)
				*ptr += sprintf(*ptr, "@(%s)+", regNames[rn]);
			else {
				data = readc(vAddr, LN_LONG, &sts);
				*ptr += sprintf(*ptr, "@#%08X", data);
				vAddr += LN_LONG;
			}
			break;

		case BDPD:
			*(*ptr)++ = '@';
		case BDP: // Byte displacement/relative
			data = readc(vAddr++, LN_BYTE, &sts);
			*ptr += sprintf(*ptr, "B^%02X", data);
			if (rn < REG_nPC)
				*ptr += sprintf(*ptr, "(%s)", regNames[rn]);
			break;

		case WDPD:
			*(*ptr)++ = '@';
		case WDP: // Word displacement/relative
			data = readc(vAddr, LN_WORD, &sts);
			vAddr += LN_WORD;
			*ptr += sprintf(*ptr, "W^%04X", data);
			if (rn < REG_nPC)
				*ptr += sprintf(*ptr, "(%s)", regNames[rn]);
			break;

		case LDPD:
			*(*ptr)++ = '@';
		case LDP: // Longword displacement/relative
			data = readc(vAddr, LN_LONG, &sts);
			vAddr += LN_LONG;
			*ptr += sprintf(*ptr, "L^%08X", data);
			if (rn < REG_nPC)
				*ptr += sprintf(*ptr, "(%s)", regNames[rn]);
			break;
		}
		if (irn >= 0)
			*ptr += sprintf(*ptr, "[%s]", regNames[irn]);
	}

	return 0;
}

int vax_cpuDevice::disasm(uint32_t vAddr)
{
	char       line[256], *ptr = line;
	uint32_t   opCode, opExtend;
	uint32_t   pcAddr = vAddr;
	const vaxOpcode *opc;
	uint32_t   sts;

	ptr += sprintf(ptr, "%08X ", pcAddr);

	// Fetch 1 or 2 bytes opcode
	opCode = readc(pcAddr++, LN_BYTE, &sts);
	if (opCode > OPC_nEXTEND) {
		opExtend = (opCode - OPC_nEXTEND) << 8;
		opCode = readc(pcAddr++, LN_BYTE, &sts);
		opCode |= opExtend;
	}
	opc = opCodes[opCode];

	if (opc->opCode != OPC_nUOPC) {
		ptr += sprintf(ptr, "%-8s", opc->opName);
		disasmOperand(pcAddr, opc, &ptr);
	} else {
		if (opCode > OPC_nEXTEND)
			ptr += sprintf(ptr, ".BYTE %02X,%02X", (opCode >> 8) + OPC_nEXTEND, opCode & 0xFF);
		else
			ptr += sprintf(ptr, ".BYTE %02X", opCode);
	}
	*ptr++ = '\0';

#ifdef ENABLE_DEBUG
	dbg::log("%s", line);
#else
	printf("%s\n", line);
#endif /* ENABLE_DEBUG */

	return pcAddr - vAddr;
}
