/*
 * executes.h
 *
 *  Created on: Mar 14, 2017
 *      Author: fswor
 */

#pragma once

#ifndef CPU_CLASS
#define CPU_CLASS vax_cpuDevice
#endif

static const char *stopNames[] =
{
	"HALT Instruction"
};

void CPU_CLASS::execute()
{
	uint32_t opCode;
	int32_t  brDisp;
	const vaxOpcode *opc;
	register uint32_t src, srcl, srch;
	register uint32_t dst, dstl, dsth;

	// Reset instruction steam
	flushvi();

	while (1) {
		try {
			// Fetch instruction from current stream
			faultAddr = REG_PC;
			opCode = uint8_t(readvi(LN_BYTE));
			if (opCode > OPC_nEXTEND) {
				opCode  = (opCode - OPC_nEXTEND) << 8;
				opCode |= uint8_t(readvi(LN_BYTE));
			}

			// Decode operands
			opc = opCodes[opCode];
			for (int idx = 0, opn = 0; idx < opc->nOperands; idx++) {
				int     opMode = opc->oprMode[idx];
				int     scale  = opMode & OPR_SCALE;
				int32_t iAddr, t1;

				if (opMode & OPR_IMMED)
					break;
				if (opMode & OPR_BRANCH) {
					brDisp = readvi(scale);
					break;
				}

				int opType = readvi(LN_BYTE);
				int mode   = opType & OPR_MMASK;
				int reg    = opType & OPR_RMASK;

				switch (mode) {
				// Short literal mode
				case LIT0: case LIT1:
				case LIT2: case LIT3:
//					if (opMode & (OPR_VADDR|OPR_ADDR|OPR_MODIFIED|OPR_WRITE))
//						throw EXC_RSVD_ADDR_FAULT;
					if (opMode & OPR_FLOAT)
						opRegs[opn++] = 0x4000 | (opType << (opMode & (OPR_FFLOAT|OPR_DFLOAT)) ? 4 : 1);
					else
						opRegs[opn++] = opType;
					if (scale > LN_LONG)
						opRegs[opn++] = 0;
					continue;

				// Register mode
				case REG:
//					if (reg >= (REG_nPC - (scale > LN_LONG)))
//						throw EXC_RSVD_ADDR_FAULT;
//					if (opMode & OPR_ADDR)
//						throw EXC_RSVD_ADDR_FAULT;
					if (opMode & (OPR_VADDR|OPR_WRITE)) {
						opRegs[opn++] = reg;
						opRegs[opn++] = gRegs[reg].l;
					} else {
						opRegs[opn++] = gRegs[reg].l;
						if (scale > LN_LONG)
							opRegs[opn++] = gRegs[reg+1].l;
						if (opMode & OPR_MODIFIED)
							opRegs[opn++] = reg;
					}
					continue;

				case ADEC: // Autodecrement mode
//					if (reg == REG_nPC)
//						throw EXC_RSVD_ADDR_FAULT;
					gRegs[reg].l -= scale;
					iAddr = gRegs[reg].l;
					break;

				case REGD: // Register deferred mode
//					if (reg == REG_nPC)
//						throw EXC_RSVD_ADDR_FAULT;
					iAddr = gRegs[reg].l;
					break;

				case AINC: // Autoincrement/immediate mode
					if (reg == REG_nPC) {
						// Immediate mode
						if (opMode & (OPR_VADDR|OPR_ADDR|OPR_WRITE)) {
							if (opMode & (OPR_VADDR|OPR_WRITE))
								opRegs[opn++] = OPR_MEM;
							opRegs[opn++] = reg;
							for (int sidx = 0; sidx < scale; sidx += LN_LONG)
								readvi(std::max(scale, LN_LONG));
						} else {
							for (int sidx = 0; sidx < scale; sidx += LN_LONG)
								opRegs[opn++] = readvi(std::max(scale, LN_LONG));
						}
						continue;
					}
					// Autoincrement mode
					iAddr = gRegs[reg].l;
					gRegs[reg].l += scale;
					break;

				case AINCD: // Autoincrement deferred/absolute mode
					if (reg == REG_nPC) {
						iAddr = readvi(LN_LONG);
					} else {
						iAddr = readv(gRegs[reg].l, LN_LONG, RACC);
						gRegs[reg].l += LN_LONG;
					}
					break;

				case BDP: // Byte displacement mode
					iAddr = gRegs[reg].l + int8_t(readvi(LN_BYTE));
					break;

				case BDPD: // Byte displacement deferred mode
					t1    = gRegs[reg].l + int8_t(readvi(LN_BYTE));
					iAddr = readv(t1, LN_LONG, RACC);
					break;

				case WDP: // Word displacement mode
					iAddr = gRegs[reg].l + int16_t(readvi(LN_WORD));
					break;

				case WDPD: // Word displacement deferred mode
					t1    = gRegs[reg].l + int16_t(readvi(LN_WORD));
					iAddr = readv(t1, LN_LONG, RACC);
					break;

				case LDP: // Longword displacement mode
					iAddr = gRegs[reg].l + int32_t(readvi(LN_LONG));
					break;

				case LDPD: // Longword displacement deferred mode
					t1    = gRegs[reg].l + int32_t(readvi(LN_LONG));
					iAddr = readv(t1, LN_LONG, RACC);
					break;

				case IDX: // Indexed mode
//					if (reg == REG_nPC)
//						throw EXC_RSVD_ADDR_FAULT;
					iAddr = gRegs[reg].l * scale;

					int opType = readvi(LN_BYTE);
					int mode   = opType & OPR_MMASK;
					int reg    = opType & OPR_RMASK;

					switch (mode) {
					case ADEC: // Autodecrement mode
//						if (reg == REG_nPC)
//							throw EXC_RSVD_ADDR_FAULT;
						gRegs[reg].l -= scale;
						iAddr += gRegs[reg].l;
						break;

					case REGD: // Register deferred mode
//						if (reg == REG_nPC)
//							throw EXC_RSVD_ADDR_FAULT;
						iAddr += gRegs[reg].l;
						break;

					case AINC: // Autoincrement mode
//						if (reg == REG_nPC)
//							throw EXC_RSVD_ADDR_FAULT;
						iAddr += gRegs[reg].l;
						gRegs[reg].l += scale;
						break;

					case AINCD: // Autoincrement deferred/absolute mode
						if (reg == REG_nPC) {
							iAddr += readvi(LN_LONG);
						} else {
							iAddr += readv(gRegs[reg].l, LN_LONG, RACC);
							gRegs[reg].l += LN_LONG;
						}
						break;

					case BDP: // Byte displacement mode
						iAddr += gRegs[reg].l + int8_t(readvi(LN_BYTE));
						break;

					case BDPD: // Byte displacement deferred mode
						t1     = gRegs[reg].l + int8_t(readvi(LN_BYTE));
						iAddr += readv(t1, LN_LONG, RACC);
						break;

					case WDP: // Word displacement mode
						iAddr += gRegs[reg].l + int16_t(readvi(LN_WORD));
						break;

					case WDPD: // Word displacement deferred mode
						t1     = gRegs[reg].l + int16_t(readvi(LN_WORD));
						iAddr += readv(t1, LN_LONG, RACC);
						break;

					case LDP: // Longword displacement mode
						iAddr += gRegs[reg].l + int32_t(readvi(LN_LONG));
						break;

					case LDPD: // Longword displacement deferred mode
						t1     = gRegs[reg].l + int32_t(readvi(LN_LONG));
						iAddr += readv(t1, LN_LONG, RACC);
						break;

//					default:
//						throw EXC_RSVD_ADDR_FAULT;
					}
					break;
				}

				// Write operands
				if (opMode & (OPR_VADDR|OPR_ADDR|OPR_WRITE)) {
					if (opMode & (OPR_VADDR|OPR_WRITE))
						opRegs[opn++] = OPR_MEM;
					opRegs[opn++] = iAddr;
				} else {
					opRegs[opn++] = readv(iAddr, scale, RACC);
					if (scale > LN_LONG)
						opRegs[opn++] = readv(iAddr, LN_LONG, RACC);
					if (opMode & OPR_MODIFIED) {
						opRegs[opn++] = OPR_MEM;
						opRegs[opn++] = iAddr;
					}
				}
			}

#ifdef ENABLE_DEBUG
			if (dbgFlags & DBG_TRACE)
				disasm(faultAddr);
#endif /* ENABLE_DEBUG */

			// Execute opcode
			switch (opCode) {

			case OPC_nHALT:
				throw STOP_HALT;

			case OPC_nNOP:
				// Do nothing...
				break;

			// MOVx instructions
			// MOVAx instructions
			case OPC_nMOVB:
				dst = opRegs[0];
				StoreB(opRegs[1], opRegs[2], dst);
				break;
			case OPC_nMOVW:
				dst = opRegs[0];
				StoreW(opRegs[1], opRegs[2], dst);
				break;
			case OPC_nMOVL:
			case OPC_nMOVAB:
			case OPC_nMOVAW:
			case OPC_nMOVAL:
			case OPC_nMOVAQ:
				dst = opRegs[0];
				StoreL(opRegs[1], opRegs[2], dst);
				break;
			case OPC_nMOVQ:
				dstl = opRegs[0];
				dsth = opRegs[1];
				StoreQ(opRegs[2], opRegs[3], dstl, dsth);
				break;

			// CLRx instructions
			case OPC_nCLRB:
				StoreB(opRegs[0], opRegs[1], 0);
				break;
			case OPC_nCLRW:
				StoreW(opRegs[0], opRegs[1], 0);
				break;
			case OPC_nCLRL:
				StoreL(opRegs[0], opRegs[1], 0);
				break;
			case OPC_nCLRQ:
				StoreQ(opRegs[0], opRegs[1], 0, 0);
				break;

			// MTPR/MFPR instructions
			case OPC_nMTPR:
				src = opRegs[0];
				writepr(opRegs[1], src);
				break;
			case OPC_nMFPR:
				src = readpr(opRegs[0]);
				StoreL(opRegs[1], opRegs[2], src);
				break;

			// Illegal/unimplemented instruction
			default:
				if (opc->opCode != OPC_nUOPC)
					throw STOP_UOPC;
//				throw EXC_ILLEGAL_INSTRUCTION;
				break;
			}
		}

		catch (int32_t exCode) {
			switch (exCode) {
			case STOP_HALT:
				printf("%s: %s at PC %08X\n", devName.c_str(), stopNames[~exCode], faultAddr);
				return;

			case STOP_UOPC:
				printf("%s: Opcode %s - Unimplemented opcode at PC %08X\n",
						devName.c_str(), opc->opName, faultAddr);
				return;
			}
		}
	}
}
