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
	register uint32_t mask;

	// Reset instruction steam
	flushvi();
	flushci();

	while (1) {
		try {
			faultAddr = REG_PC;
//#ifdef ENABLE_DEBUG
//			if (dbgFlags & DBG_TRACE)
//				disasm(faultAddr);
//#endif /* ENABLE_DEBUG */
			disasm(faultAddr);

			// Fetch instruction from current stream
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

//				printf("opr: mode=%02X reg=%02X\n", mode, reg);

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
					printf("reg: R%d => %08X\n", reg, iAddr);
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
					t1    = int8_t(readvi(LN_BYTE));
					iAddr = gRegs[reg].l + t1;
					break;

				case BDPD: // Byte displacement deferred mode
					t1    = int8_t(readvi(LN_BYTE));
					t1    = gRegs[reg].l + t1;
					iAddr = readv(t1, LN_LONG, RACC);
					break;

				case WDP: // Word displacement mode
					t1    = int16_t(readvi(LN_WORD));
					iAddr = gRegs[reg].l + t1;
					break;

				case WDPD: // Word displacement deferred mode
					t1    = int16_t(readvi(LN_WORD));
					t1    = gRegs[reg].l + t1;
					iAddr = readv(t1, LN_LONG, RACC);
					break;

				case LDP: // Longword displacement mode
					t1    = int32_t(readvi(LN_LONG));
					iAddr = gRegs[reg].l + t1;
					break;

				case LDPD: // Longword displacement deferred mode
					t1    = int32_t(readvi(LN_LONG));
					t1    = gRegs[reg].l + t1;
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
						t1     = int8_t(readvi(LN_BYTE));
						iAddr += gRegs[reg].l + t1;
						break;

					case BDPD: // Byte displacement deferred mode
						t1     = int8_t(readvi(LN_BYTE));
						t1     = gRegs[reg].l + t1;
						iAddr += readv(t1, LN_LONG, RACC);
						break;

					case WDP: // Word displacement mode
						t1     = int16_t(readvi(LN_WORD));
						iAddr += gRegs[reg].l + t1;
						break;

					case WDPD: // Word displacement deferred mode
						t1     = int16_t(readvi(LN_WORD));
						t1     = gRegs[reg].l + t1;
						iAddr += readv(t1, LN_LONG, RACC);
						break;

					case LDP: // Longword displacement mode
						t1     = int32_t(readvi(LN_LONG));
						iAddr += gRegs[reg].l + t1;
						break;

					case LDPD: // Longword displacement deferred mode
						t1     = int32_t(readvi(LN_LONG));
						t1     = gRegs[reg].l + t1;
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


			// Execute opcode
			switch (opCode) {

			case OPC_nHALT:
				throw STOP_HALT;

			case OPC_nNOP:
				// Do nothing...
				break;

			// MTPR/MFPR instructions
			case OPC_nMTPR:
				src = opRegs[0];
				writepr(opRegs[1], src);
				UpdateCC_IIZP_L(ccReg, src);
				break;
			case OPC_nMFPR:
				src = readpr(opRegs[0]);
				StoreL(opRegs[1], opRegs[2], src);
				UpdateCC_IIZP_L(ccReg, src);
				break;

			// Bcc instructions - Branch On (condition)
			//
			// Opcodes: Condition
			//   12     Z EQL 0         BNEQ  Branch on Not Equal (signed)
			//   12     Z EQL 0         BNEQU Branch on Not Equal Unsigned
			//   13     Z EQL 1         BEQL  Branch on Equal (signed)
			//   13     Z EQL 1         BEQLU Branch on Equal Unsigned
			//   14     (N OR Z) EQL 0  BGTR  Branch on Greater Than (signed)
			//   15     (N OR Z) EQL 1  BLEQ  Branch on Less Than or Equal (signed)
			//   18     N EQL 0         BGEQ  Branch on Greater Than or Equal (signed)
			//   19     N EQL 1         BLSS  Branch on Less Than (signed)
			//   1A     (C OR Z) EQL 0  BGTRU Branch on Greater Than Unsigned
			//   1B     (C OR Z) EQL 1  BLEQU Branch on Less Than or Equal Unsigned
			//   1C     V EQL 0         BVC   Branch on Overflow Clear
			//   1D     V EQL 1         BVS   Branch on Overflow Set
			//   1E     C EQL 0         BGEQU Branch on Greater Than or Equal Unsigned
			//   1E     C EQL 0         BCC   Branch on Carry Clear
			//   1F     C EQL 1         BLSSU Branch on Less Than Unsigned
			//   1F     C EQL 1         BCS   Branch on Carry Set

			case OPC_nBNEQ:
				if ((ccReg & CC_Z) == 0) {
					REG_PC = REG_PC + int8_t(brDisp);
					flushvi();
				}
				break;

			case OPC_nBEQL:
				if (ccReg & CC_Z) {
					REG_PC = REG_PC + int8_t(brDisp);
					flushvi();
				}
				break;

			case OPC_nBGTR:
				if ((ccReg & (CC_N|CC_Z)) == 0) {
					REG_PC = REG_PC + int8_t(brDisp);
					flushvi();
				}
				break;

			case OPC_nBLEQ:
				if (ccReg & (CC_N|CC_Z)) {
					REG_PC = REG_PC + int8_t(brDisp);
					flushvi();
				}
				break;

			case OPC_nBGEQ:
				if ((ccReg & CC_N) == 0) {
					REG_PC = REG_PC + int8_t(brDisp);
					flushvi();
				}
				break;

			case OPC_nBLSS:
				if (ccReg & CC_N) {
					REG_PC = REG_PC + int8_t(brDisp);
					flushvi();
				}
				break;

			case OPC_nBGTRU:
				if ((ccReg & (CC_C|CC_Z)) == 0) {
					REG_PC = REG_PC + int8_t(brDisp);
					flushvi();
				}
				break;

			case OPC_nBLEQU:
				if (ccReg & (CC_C|CC_Z)) {
					REG_PC = REG_PC + int8_t(brDisp);
					flushvi();
				}
				break;

			case OPC_nBVC:
				if ((ccReg & CC_V) == 0) {
					REG_PC = REG_PC + int8_t(brDisp);
					flushvi();
				}
				break;

			case OPC_nBVS:
				if (ccReg & CC_V) {
					REG_PC = REG_PC + int8_t(brDisp);
					flushvi();
				}
				break;

			case OPC_nBCC:
				if ((ccReg & CC_C) == 0) {
					REG_PC = REG_PC + int8_t(brDisp);
					flushvi();
				}
				break;

			case OPC_nBCS:
				if (ccReg & CC_C) {
					REG_PC = REG_PC + int8_t(brDisp);
					flushvi();
				}
				break;

			//  BRx instructions
			case OPC_nBRB:
				REG_PC = REG_PC + int8_t(brDisp);
				flushvi();
				break;
			case OPC_nBRW:
				REG_PC = REG_PC + int16_t(brDisp);
				flushvi();
				break;

			// JMP instruction
			case OPC_nJMP:
				REG_PC = opRegs[0];
				flushvi();
				break;

			// MOVx instructions
			// MOVAx instructions
			case OPC_nMOVB:
				dst = opRegs[0];
				StoreB(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZP_B(ccReg, dst);
				printf("%s: Move %02X\n", devName.c_str(), uint8_t(dst));
				break;
			case OPC_nMOVW:
				dst = opRegs[0];
				StoreW(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZP_W(ccReg, dst);
				printf("%s: Move %04X\n", devName.c_str(), uint16_t(dst));
				break;
			case OPC_nMOVL:
			case OPC_nMOVAB:
			case OPC_nMOVAW:
			case OPC_nMOVAL:
			case OPC_nMOVAQ:
				dst = opRegs[0];
				StoreL(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				printf("%s: Move %08X\n", devName.c_str(), uint32_t(dst));
				break;
			case OPC_nMOVQ:
				dstl = opRegs[0];
				dsth = opRegs[1];
				StoreQ(opRegs[2], opRegs[3], dstl, dsth);
				UpdateCC_IIZP_Q(ccReg, dstl, dsth);
				printf("%s: Move %08X %08X\n", devName.c_str(), uint32_t(dstl), uint32_t(dsth));
				break;

			// CLRx instructions
			case OPC_nCLRB:
				StoreB(opRegs[0], opRegs[1], 0);
				UpdateCC_Z1ZP(ccReg);
				break;
			case OPC_nCLRW:
				StoreW(opRegs[0], opRegs[1], 0);
				UpdateCC_Z1ZP(ccReg);
				break;
			case OPC_nCLRL:
				StoreL(opRegs[0], opRegs[1], 0);
				UpdateCC_Z1ZP(ccReg);
				break;
			case OPC_nCLRQ:
				StoreQ(opRegs[0], opRegs[1], 0, 0);
				UpdateCC_Z1ZP(ccReg);
				break;

			// INCx instructions
			case OPC_nINCB:
				src = opRegs[0];
				dst = src + 1;
				StoreB(opRegs[1], opRegs[2], dst);
				printf("%s: Inc %02X => %02X\n", devName.c_str(), uint8_t(src), uint8_t(dst));
				break;
			case OPC_nINCW:
				src = opRegs[0];
				dst = src + 1;
				StoreW(opRegs[1], opRegs[2], dst);
				printf("%s: Inc %04X => %04X\n", devName.c_str(), uint16_t(src), uint16_t(dst));
				break;
			case OPC_nINCL:
				src = opRegs[0];
				dst = src + 1;
				StoreL(opRegs[1], opRegs[2], dst);
				printf("%s: Inc %08X => %08X\n", devName.c_str(), uint32_t(src), uint32_t(dst));
				break;

			// DECx instructions
			case OPC_nDECB:
				src = opRegs[0];
				dst = src - 1;
				StoreB(opRegs[1], opRegs[2], dst);
				printf("%s: Dec %02X => %02X\n", devName.c_str(), uint8_t(src), uint8_t(dst));
				break;
			case OPC_nDECW:
				src = opRegs[0];
				dst = src - 1;
				StoreW(opRegs[1], opRegs[2], dst);
				printf("%s: Dec %04X => %04X\n", devName.c_str(), uint16_t(src), uint16_t(dst));
				break;
			case OPC_nDECL:
				src = opRegs[0];
				dst = src - 1;
				StoreL(opRegs[1], opRegs[2], dst);
				printf("%s: Dec %08X => %08X\n", devName.c_str(), uint32_t(src), uint32_t(dst));
				break;

			// XORx instructions
			case OPC_nXORB2:
			case OPC_nXORB3:
				mask = opRegs[0];
				src  = opRegs[1];
				dst  = src ^ mask;
				StoreB(opRegs[2], opRegs[3], dst);
				UpdateCC_IIZP_B(ccReg, dst);
				printf("%s: %02X ^ %02X => %02X\n", devName.c_str(),
						uint8_t(src), uint8_t(mask), uint8_t(dst));
				break;
			case OPC_nXORW2:
			case OPC_nXORW3:
				mask = opRegs[0];
				src  = opRegs[1];
				dst  = src ^ mask;
				StoreW(opRegs[2], opRegs[3], dst);
				UpdateCC_IIZP_W(ccReg, dst);
				printf("%s: %04X ^ %04X => %04X\n", devName.c_str(),
						uint16_t(src), uint16_t(mask), uint16_t(dst));
				break;
			case OPC_nXORL2:
			case OPC_nXORL3:
				mask = opRegs[0];
				src  = opRegs[1];
				dst  = src ^ mask;
				StoreL(opRegs[2], opRegs[3], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				printf("%s: %08X ^ %08X => %08X\n", devName.c_str(),
						uint32_t(src), uint32_t(mask), uint32_t(dst));
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
