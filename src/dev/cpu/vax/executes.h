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
	bool     ovflg;
	const vaxOpcode *opc;
	register int32_t  src1, src2, carry;
	register int32_t  dst1, dst2;
	register int32_t  src, dst, tmp;
	register int64_t  srcq, srcq1, srcq2;
	register int64_t  dstq, dstq1, dstq2;
	register uint32_t usrc, udst, utmp;
	register uint32_t mask;
	register int32_t  cnt;

	// Reset instruction steam
	flushvi();
	flushci();

	while (1) {
		try {
			faultAddr = REG_PC;

			// Check trap and interrupt requests first
			if (irqFlags != 0) {
				interrupt();
				UpdateIRQ();
				continue;
			}

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
							opRegs[opn++] = gRegs[reg].l;
							for (int sidx = 0; sidx < scale; sidx += LN_LONG)
								readvi(std::min(scale, LN_LONG));
						} else {
							for (int sidx = 0; sidx < scale; sidx += LN_LONG)
								opRegs[opn++] = readvi(std::min(scale, LN_LONG));
						}
						continue;
					}
					// Autoincrement mode
					iAddr = gRegs[reg].l;
					gRegs[reg].l += scale;
//					printf("reg: R%d => %08X\n", reg, iAddr);
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
					if (scale <= LN_LONG) {
						opRegs[opn++] = readv(iAddr, scale, RACC);
					} else {
						opRegs[opn++] = readv(iAddr, LN_LONG, RACC);
						opRegs[opn++] = readv(iAddr+LN_LONG, LN_LONG, RACC);
					}
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

			case OPC_nREI:
				resume();
				break;

			case OPC_nBPT:
				faultAddr = REG_PC;
				exception(SCB_BPT, 0, IE_EXC);
				break;

			case OPC_nXFC:
				faultAddr = REG_PC;
				exception(SCB_XFC, 0, IE_EXC);
				break;

			// INDEX - Index instruction
			case OPC_nINDEX:
				src  = SXTL(opRegs[0]);
				src1 = SXTL(opRegs[3]);
				src2 = SXTL(opRegs[4]);
//				if (src < opRegs[1] || src > opRegs[2]));

				dst = (src + src2) * src1;
				StoreL(opRegs[5], opRegs[6], dst);
				UpdateCC_IIZZ_L(ccReg, dst);
				printf("%s: %08X (%08X to %08X) + %08X * %08X => %08X: %s\n", devName.c_str(),
					ZXTL(src), ZXTL(opRegs[1]), ZXTL(opRegs[2]), ZXTL(src2), ZXTL(src1),
					ZXTL(dst), stringCC(ccReg));
				break;

			// BIxPSW instructions
			case OPC_nBICPSW:
				mask = uint16_t(opRegs[0]);
//				if (mask & PSW_MBZ)
//					throw EXC_RSVD_OPND_FAULT;
				psReg &= ~mask;
				ccReg &= ~mask;
				break;
			case OPC_nBISPSW:
				mask = uint16_t(opRegs[0]);
//				if (mask & PSW_MBZ)
//					throw EXC_RSVD_OPND_FAULT;
				psReg |= (mask & ~PSW_CC);
				ccReg |= (mask & PSW_CC);
				break;

			// MOVPSL instruction
			case OPC_nMOVPSL:
				dst = psReg | ccReg;
				StoreL(opRegs[0], opRegs[1], dst);
				printf("%s: PSL %08X: %s\n", devName.c_str(),
						ZXTL(dst), stringCC(ccReg));
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


			// BSBx instructions
			case OPC_nBSBB:
				writev(REG_SP - LN_LONG, REG_PC, LN_LONG, WACC);
				REG_SP -= LN_LONG;
				REG_PC = REG_PC + int8_t(brDisp);
				flushvi();
				break;
			case OPC_nBSBW:
				writev(REG_SP - LN_LONG, REG_PC, LN_LONG, WACC);
				REG_SP -= LN_LONG;
				REG_PC = REG_PC + int16_t(brDisp);
				flushvi();
				break;

			// JSB instruction
			case OPC_nJSB:
				writev(REG_SP - LN_LONG, REG_PC, LN_LONG, WACC);
				REG_SP -= LN_LONG;
				REG_PC = opRegs[0];
				flushvi();
				break;

			// RSB instruction
			case OPC_nRSB:
				REG_PC  = readv(REG_SP, LN_LONG, RACC);
				REG_SP += LN_LONG;
				flushvi();
				break;

			// ACBx instructions
			case OPC_nACBB:
				src   = SXTB(opRegs[0]);
				src1  = SXTB(opRegs[1]);
				src2  = SXTB(opRegs[2]);
				dst   = src2 + src1;
				StoreB(opRegs[3], opRegs[4], dst);
				UpdateCC_IIZP_B(ccReg, dst);
				UpdateV_ADD_B(ccReg, dst, src1, src2);
				if ((src1 < 0) ? (dst >= src) : (dst <= src)) {
					REG_PC = REG_PC + SXTW(brDisp);
					flushvi();
				}
				printf("%s: %02X + %02X => %02X: %s\n", devName.c_str(),
					ZXTB(src2), ZXTB(src1), ZXTB(dst), stringCC(ccReg));
				printf("%s: %02X %s %02X: %s\n", devName.c_str(),
					ZXTB(dst), (src1 < 0) ? ">=" : "<=", ZXTB(src),
					((src1 < 0) ? (dst >= src) : (dst <= src)) ? "Jumped" : "Continue");
				break;
			case OPC_nACBW:
				src   = SXTW(opRegs[0]);
				src1  = SXTW(opRegs[1]);
				src2  = SXTW(opRegs[2]);
				dst   = src2 + src1;
				StoreW(opRegs[3], opRegs[4], dst);
				UpdateCC_IIZP_W(ccReg, dst);
				UpdateV_ADD_W(ccReg, dst, src1, src2);
				if ((src1 < 0) ? (dst >= src) : (dst <= src)) {
					REG_PC = REG_PC + SXTW(brDisp);
					flushvi();
				}
				printf("%s: %04X + %04X => %04X: %s\n", devName.c_str(),
					ZXTW(src2), ZXTW(src1), ZXTW(dst), stringCC(ccReg));
				printf("%s: %04X %s %04X: %s\n", devName.c_str(),
					ZXTW(dst), (src1 < 0) ? ">=" : "<=", ZXTW(src),
					((src1 < 0) ? (dst >= src) : (dst <= src)) ? "Jumped" : "Continue");
				break;
			case OPC_nACBL:
				src   = SXTL(opRegs[0]);
				src1  = SXTL(opRegs[1]);
				src2  = SXTL(opRegs[2]);
				dst   = src2 + src1;
				StoreL(opRegs[3], opRegs[4], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				UpdateV_ADD_L(ccReg, dst, src1, src2);
				if ((src1 < 0) ? (dst >= src) : (dst <= src)) {
					REG_PC = REG_PC + SXTW(brDisp);
					flushvi();
				}
				printf("%s: %08X + %08X => %08X: %s\n", devName.c_str(),
					ZXTL(src2), ZXTL(src1), ZXTL(dst), stringCC(ccReg));
				printf("%s: %08X %s %08X: %s\n", devName.c_str(),
					ZXTL(dst), (src1 < 0) ? ">=" : "<=", ZXTL(src),
					((src1 < 0) ? (dst >= src) : (dst <= src)) ? "Jumped" : "Continue");
				break;

			// CASEx - Case instructions
			case OPC_nCASEB:
				src1 = SXTB(opRegs[0]);
				src2 = SXTB(opRegs[1]);
				src  = SXTB(opRegs[2]);
				dst  = src1 - src2;
				UpdateCC_CMP_B(ccReg, dst, src);
				if (ZXTB(dst) <= ZXTB(src)) {
					brDisp = readv(REG_PC + (ZXTB(dst) << 1), LN_WORD, RACC);
					REG_PC = REG_PC + SXTW(brDisp);
				} else
					REG_PC = REG_PC + ((ZXTB(src) << 1) + 2);
				flushvi();
				printf("%s: (%02X - %02X) => %02X <= %02X\n", devName.c_str(),
					ZXTB(src1), ZXTB(src2), ZXTB(dst), ZXTB(src));
//				if (ZXTB(dst) <= ZXTB(src))
//					printf("%s: %08X + %02X => %08X\n", devName.c_str(),
//						REG_PC, dst << 1, REG_PC + (dst << 1));
//				else
//					printf("%s: %08X + %02X + 2 => %08X\n", devName.c_str(),
//						REG_PC, src << 1, REG_PC + (src << 1) + 2);
				break;
			case OPC_nCASEW:
				src1 = SXTW(opRegs[0]);
				src2 = SXTW(opRegs[1]);
				src  = SXTW(opRegs[2]);
				dst  = src1 - src2;
				UpdateCC_CMP_W(ccReg, dst, src);
				if (ZXTW(dst) <= ZXTW(src)) {
					brDisp = readv(REG_PC + (ZXTW(dst) << 1), LN_WORD, RACC);
					REG_PC = REG_PC + SXTW(brDisp);
				} else
					REG_PC = REG_PC + ((ZXTW(src) << 1) + 2);
				flushvi();
				printf("%s: (%04X - %04X) => %04X <= %04X\n", devName.c_str(),
					ZXTW(src1), ZXTW(src2), ZXTW(dst), ZXTW(src));
				break;
			case OPC_nCASEL:
				src1 = SXTL(opRegs[0]);
				src2 = SXTL(opRegs[1]);
				src  = SXTL(opRegs[2]);
				dst  = src1 - src2;
				UpdateCC_CMP_L(ccReg, dst, src);
				if (ZXTL(dst) <= ZXTL(src)) {
					brDisp = readv(REG_PC + (ZXTL(dst) << 1), LN_WORD, RACC);
					REG_PC = REG_PC + SXTW(brDisp);
				} else
					REG_PC = REG_PC + ((ZXTL(src) << 1) + 2);
				flushvi();
				printf("%s: (%08X - %08X) => %08X <= %08X\n", devName.c_str(),
					ZXTL(src1), ZXTL(src2), ZXTL(dst), ZXTL(src));
				break;

			// AOBcc instructions
			case OPC_nAOBLEQ:
				src1 = SXTL(opRegs[0]);
				src2 = SXTL(opRegs[1]);
				dst  = src2 + 1;
				StoreL(opRegs[2], opRegs[3], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				UpdateV_ADD_L(ccReg, dst, 1, src2);
				if (dst <= src1) {
					REG_PC = REG_PC + SXTB(brDisp);
					flushvi();
				}
				printf("%s: %08X + 1 => %08X <= %08X: %s\n", devName.c_str(),
						ZXTL(src2), ZXTL(dst), ZXTL(src1), stringCC(ccReg));
				if (dst <= src1)
					printf("%s: Jump into PC %08X\n", devName.c_str(), REG_PC);
				break;
			case OPC_nAOBLSS:
				src1 = SXTL(opRegs[0]);
				src2 = SXTL(opRegs[1]);
				dst  = src2 + 1;
				StoreL(opRegs[2], opRegs[3], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				UpdateV_ADD_L(ccReg, dst, 1, src2);
				if (dst < src1) {
					REG_PC = REG_PC + SXTB(brDisp);
					flushvi();
				}
				printf("%s: %08X + 1 => %08X < %08X: %s\n", devName.c_str(),
						ZXTL(src2), ZXTL(dst), ZXTL(src1), stringCC(ccReg));
				if (dst < src1)
					printf("%s: Jump into PC %08X\n", devName.c_str(), REG_PC);
				break;

			// SOBcc instructions
			case OPC_nSOBGEQ:
				src = SXTL(opRegs[0]);
				dst = src - 1;
				StoreL(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				UpdateV_SUB_L(ccReg, dst, 1, src);
				if (dst >= 0) {
					REG_PC = REG_PC + SXTB(brDisp);
					flushvi();
				}
				printf("%s: %08X - 1 => %08X >= 0: %s\n", devName.c_str(),
						ZXTL(src), ZXTL(dst), stringCC(ccReg));
				if (dst >= 0)
					printf("%s: Jump into PC %08X\n", devName.c_str(), REG_PC);
				break;
			case OPC_nSOBGTR:
				src = SXTL(opRegs[0]);
				dst = src - 1;
				StoreL(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				UpdateV_SUB_L(ccReg, dst, 1, src);
				if (dst > 0) {
					REG_PC = REG_PC + SXTB(brDisp);
					flushvi();
				}
				printf("%s: %08X - 1 => %08X > 0: %s\n", devName.c_str(),
						ZXTL(src), ZXTL(dst), stringCC(ccReg));
				if (dst > 0)
					printf("%s: Jump into PC %08X\n", devName.c_str(), REG_PC);
				break;

			// BBx instructions
			case OPC_nBBS:
				if (getBit() == 1) {
					REG_PC = REG_PC + SXTB(brDisp);
					flushvi();
				}
				break;
			case OPC_nBBC:
				if (getBit() == 0) {
					REG_PC = REG_PC + SXTB(brDisp);
					flushvi();
				}
				break;
			case OPC_nBBSSI:
			case OPC_nBBSS:
				if (setBit(1) == 1) {
					REG_PC = REG_PC + SXTB(brDisp);
					flushvi();
				}
				break;
			case OPC_nBBSC:
				if (setBit(0) == 1) {
					REG_PC = REG_PC + SXTB(brDisp);
					flushvi();
				}
				break;
			case OPC_nBBCS:
				if (setBit(1) == 0) {
					REG_PC = REG_PC + SXTB(brDisp);
					flushvi();
				}
				break;
			case OPC_nBBCCI:
			case OPC_nBBCC:
				if (setBit(0) == 0) {
					REG_PC = REG_PC + SXTB(brDisp);
					flushvi();
				}
				break;

			// BLBx instructions
			case OPC_nBLBS:
				src = opRegs[0];
				if ((src & 1) == 1) {
					REG_PC = REG_PC + SXTB(brDisp);
					flushvi();
				}
				printf("%s: %08X & 1 => %d\n", devName.c_str(), src, src & 1);
				break;
			case OPC_nBLBC:
				src = opRegs[0];
				if ((src & 1) == 0) {
					REG_PC = REG_PC + SXTB(brDisp);
					flushvi();
				}
				printf("%s: %08X & 1 => %d\n", devName.c_str(), src, src & 1);
				break;

			// MOVx instructions
			// MOVAx instructions
			case OPC_nMOVB:
				dst = opRegs[0];
				StoreB(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZP_B(ccReg, dst);
				printf("%s: Move %02X: %s\n", devName.c_str(),
						ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nMOVW:
				dst = opRegs[0];
				StoreW(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZP_W(ccReg, dst);
				printf("%s: Move %04X: %s\n", devName.c_str(),
						ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nMOVL:
			case OPC_nMOVAB:
			case OPC_nMOVAW:
			case OPC_nMOVAL:
			case OPC_nMOVAQ:
				dst = opRegs[0];
				StoreL(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				printf("%s: Move %08X: %s\n", devName.c_str(),
						ZXTL(dst), stringCC(ccReg));
				break;
			case OPC_nMOVQ:
				dst1 = opRegs[0];
				dst2 = opRegs[1];
				StoreQ(opRegs[2], opRegs[3], dst1, dst2);
				UpdateCC_IIZP_Q(ccReg, dst1, dst2);
				printf("%s: Move %08X %08X: %s\n", devName.c_str(),
						ZXTL(dst1), ZXTL(dst2), stringCC(ccReg));
				break;

			// MCOMx - Move complemented instructions
			case OPC_nMCOMB:
				src = SXTB(opRegs[0]);
				dst = ~src;
				StoreB(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZP_B(ccReg, dst);
				printf("%s: Move ~%02X => %02X: %s\n", devName.c_str(),
					ZXTB(src), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nMCOMW:
				src = SXTW(opRegs[0]);
				dst = ~src;
				StoreW(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZP_W(ccReg, dst);
				printf("%s: Move ~%04X => %04X: %s\n", devName.c_str(),
					ZXTW(src), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nMCOML:
				src = SXTL(opRegs[0]);
				dst = ~src;
				StoreL(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				printf("%s: Move ~%08X => %08X: %s\n", devName.c_str(),
					ZXTL(src), ZXTL(dst), stringCC(ccReg));
				break;

			// MNEGx - Move negated instructions
			case OPC_nMNEGB:
				src = SXTB(opRegs[0]);
				dst = -src;
				StoreB(opRegs[1], opRegs[2], dst);
				UpdateCC_SUB_B(ccReg, dst, src, 0);
				printf("%s: Move -%02X => %02X: %s\n", devName.c_str(),
					ZXTB(src), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nMNEGW:
				src = SXTW(opRegs[0]);
				dst = -src;
				StoreW(opRegs[1], opRegs[2], dst);
				UpdateCC_SUB_W(ccReg, dst, src, 0);
				printf("%s: Move -%04X => %04X: %s\n", devName.c_str(),
					ZXTW(src), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nMNEGL:
				src = SXTL(opRegs[0]);
				dst = -src;
				StoreL(opRegs[1], opRegs[2], dst);
				UpdateCC_SUB_L(ccReg, dst, src, 0);
				printf("%s: Move -%08X => %08X: %s\n", devName.c_str(),
					ZXTL(src), ZXTL(dst), stringCC(ccReg));
				break;

			// MOVZx instructions
			case OPC_nMOVZBW:
				dst = ZXTB(opRegs[0]);
				StoreW(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZP_W(ccReg, dst);
				printf("%s: Move %02X => %04X: %s\n", devName.c_str(),
					ZXTB(dst), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nMOVZBL:
				dst = ZXTB(opRegs[0]);
				StoreL(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				printf("%s: Move %02X => %08X: %s\n", devName.c_str(),
					ZXTB(dst), ZXTL(dst), stringCC(ccReg));
				break;
			case OPC_nMOVZWL:
				dst = ZXTW(opRegs[0]);
				StoreL(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				printf("%s: Move %04X => %08X: %s\n", devName.c_str(),
					ZXTW(dst), ZXTL(dst), stringCC(ccReg));
				break;

			// CVTx - Convert instructions
			case OPC_nCVTBW:
				src = SXTB(opRegs[0]);
				dst = SXTW(src);
				StoreW(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZZ_W(ccReg, dst);
				printf("%s: %02X => %04X: %s\n", devName.c_str(),
						ZXTB(src), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nCVTBL:
				src = SXTB(opRegs[0]);
				dst = SXTL(src);
				StoreL(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZZ_L(ccReg, dst);
				printf("%s: %02X => %08X: %s\n", devName.c_str(),
						ZXTB(src), ZXTL(dst), stringCC(ccReg));
				break;
			case OPC_nCVTWB:
				src = SXTW(opRegs[0]);
				dst = SXTB(src);
				StoreB(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZZ_B(ccReg, dst);
				if (src < -128 || src > 127) {
					ccReg |= CC_V;
				}
				printf("%s: %04X => %02X: %s\n", devName.c_str(),
						ZXTW(src), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nCVTWL:
				src = SXTW(opRegs[0]);
				dst = SXTL(src);
				StoreL(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZZ_L(ccReg, dst);
				printf("%s: %04X => %08X: %s\n", devName.c_str(),
						ZXTW(src), ZXTL(dst), stringCC(ccReg));
				break;
			case OPC_nCVTLB:
				src = SXTL(opRegs[0]);
				dst = SXTB(src);
				StoreB(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZZ_B(ccReg, dst);
				if (src < -128 || src > 127) {
					ccReg |= CC_V;
				}
				printf("%s: %08X => %02X: %s\n", devName.c_str(),
						ZXTL(src), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nCVTLW:
				src = SXTL(opRegs[0]);
				dst = SXTW(src);
				StoreW(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZZ_W(ccReg, dst);
				if (src < -128 || src > 127) {
					ccReg |= CC_V;
				}
				printf("%s: %08X => %04X: %s\n", devName.c_str(),
						ZXTL(src), ZXTW(dst), stringCC(ccReg));
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

			// CMPx instructions
			case OPC_nCMPB:
				src1 = SXTB(opRegs[0]);
				src2 = SXTB(opRegs[1]);
				UpdateCC_CMP_B(ccReg, src1, src2);
				printf("%s: Compare %02X with %02X: %s\n", devName.c_str(),
						ZXTB(src1), ZXTB(src2), stringCC(ccReg));
				break;
			case OPC_nCMPW:
				src1 = SXTW(opRegs[0]);
				src2 = SXTW(opRegs[1]);
				UpdateCC_CMP_W(ccReg, src1, src2);
				printf("%s: Compare %04X with %04X: %s\n", devName.c_str(),
						ZXTW(src1), ZXTW(src2), stringCC(ccReg));
				break;
			case OPC_nCMPL:
				src1 = SXTL(opRegs[0]);
				src2 = SXTL(opRegs[1]);
				UpdateCC_CMP_L(ccReg, src1, src2);
				printf("%s: Compare %08X with %08X: %s\n", devName.c_str(),
						ZXTL(src1), ZXTL(src2), stringCC(ccReg));
				break;

				// TSTx instructions
			case OPC_nTSTB:
				src = SXTB(opRegs[0]);
				UpdateCC_IIZZ_B(ccReg, src);
				printf("%s: Test %02X: %s\n", devName.c_str(),
						ZXTB(src), stringCC(ccReg));
				break;
			case OPC_nTSTW:
				src = SXTW(opRegs[0]);
				UpdateCC_IIZZ_W(ccReg, src);
				printf("%s: Test %04X: %s\n", devName.c_str(),
						ZXTW(src), stringCC(ccReg));
				break;
			case OPC_nTSTL:
				src = SXTL(opRegs[0]);
				UpdateCC_IIZZ_L(ccReg, src);
				printf("%s: Test %08X: %s\n", devName.c_str(),
						ZXTL(src), stringCC(ccReg));
				break;

			// INCx instructions
			case OPC_nINCB:
				src = SXTB(opRegs[0]);
				dst = src + 1;
				StoreB(opRegs[1], opRegs[2], dst);
				UpdateCC_ADD_B(ccReg, dst, 1, src);
				printf("%s: %02X + 1 => %02X: %s\n",
					devName.c_str(), ZXTB(src), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nINCW:
				src = SXTW(opRegs[0]);
				dst = src + 1;
				StoreW(opRegs[1], opRegs[2], dst);
				UpdateCC_ADD_W(ccReg, dst, 1, src);
				printf("%s: %04X + 1 => %04X: %s\n",
					devName.c_str(), ZXTW(src), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nINCL:
				src = SXTL(opRegs[0]);
				dst = src + 1;
				StoreL(opRegs[1], opRegs[2], dst);
				UpdateCC_ADD_L(ccReg, dst, 1, src);
				printf("%s: %08X + 1 => %08X: %s\n",
					devName.c_str(), ZXTL(src), ZXTL(dst), stringCC(ccReg));
				break;

			// DECx instructions
			case OPC_nDECB:
				src = SXTB(opRegs[0]);
				dst = src - 1;
				StoreB(opRegs[1], opRegs[2], dst);
				UpdateCC_SUB_B(ccReg, dst, 1, src);
				printf("%s: %08X - 1 => %08X: %s\n",
					devName.c_str(), ZXTB(src), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nDECW:
				src = SXTW(opRegs[0]);
				dst = src - 1;
				StoreW(opRegs[1], opRegs[2], dst);
				UpdateCC_SUB_W(ccReg, dst, 1, src);
				printf("%s: %04X - 1 => %04X: %s\n",
					devName.c_str(), ZXTW(src), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nDECL:
				src = SXTL(opRegs[0]);
				dst = src - 1;
				StoreL(opRegs[1], opRegs[2], dst);
				UpdateCC_SUB_L(ccReg, dst, 1, src);
				printf("%s: %08X - 1 => %08X: %s\n",
					devName.c_str(), ZXTL(src), ZXTL(dst), stringCC(ccReg));
				break;

			// ADAWI instruction
			case OPC_nADAWI:
				src = SXTW(opRegs[0]);
				if (opRegs[1] != OPR_MEM) {
					tmp = SXTW(gRegs[opRegs[1]].l);
					dst = src + tmp;
					gRegs[opRegs[1]].l = ZXTW(dst);
				} else {
//					if (opRegs[1] & 1)
//						throw EXC_RSVD_OPND_FAULT;
					tmp = SXTW(readv(opRegs[2], LN_WORD, RACC));
					dst = src + tmp;
					writev(opRegs[2], dst, LN_WORD, WACC);
				}
				UpdateCC_ADD_W(ccReg, dst, src, tmp);
				printf("%s: %04X + %04X => %04X: %s\n", devName.c_str(),
						ZXTW(src), ZXTW(tmp), ZXTW(dst), stringCC(ccReg));
				break;

			// ADDx instructions
			case OPC_nADDB2:
			case OPC_nADDB3:
				src1 = SXTB(opRegs[0]);
				src2 = SXTB(opRegs[1]);
				dst  = src2 + src1;
				StoreB(opRegs[2], opRegs[3], dst);
				UpdateCC_ADD_B(ccReg, dst, src1, src2);
				printf("%s: %02X + %02X => %02X: %s\n", devName.c_str(),
					ZXTB(src1), ZXTB(src2), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nADDW2:
			case OPC_nADDW3:
				src1 = SXTW(opRegs[0]);
				src2 = SXTW(opRegs[1]);
				dst  = src2 + src1;
				StoreW(opRegs[2], opRegs[3], dst);
				UpdateCC_ADD_W(ccReg, dst, src1, src2);
				printf("%s: %04X + %04X => %04X: %s\n", devName.c_str(),
					ZXTW(src1), ZXTW(src2), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nADDL2:
			case OPC_nADDL3:
				src1 = SXTL(opRegs[0]);
				src2 = SXTL(opRegs[1]);
				dst  = src2 + src1;
				StoreL(opRegs[2], opRegs[3], dst);
				UpdateCC_ADD_L(ccReg, dst, src1, src2);
				printf("%s: %08X + %08X => %08X: %s\n", devName.c_str(),
					ZXTL(src1), ZXTL(src2), ZXTL(dst), stringCC(ccReg));
				break;

			// SUBx instructions
			case OPC_nSUBB2:
			case OPC_nSUBB3:
				src1 = SXTB(opRegs[0]);
				src2 = SXTB(opRegs[1]);
				dst  = src2 - src1;
				StoreB(opRegs[2], opRegs[3], dst);
				UpdateCC_SUB_B(ccReg, dst, src1, src2);
				printf("%s: %02X - %02X => %02X: %s\n", devName.c_str(),
					ZXTB(src1), ZXTB(src2), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nSUBW2:
			case OPC_nSUBW3:
				src1 = SXTW(opRegs[0]);
				src2 = SXTW(opRegs[1]);
				dst  = src2 - src1;
				StoreW(opRegs[2], opRegs[3], dst);
				UpdateCC_SUB_W(ccReg, dst, src1, src2);
				printf("%s: %04X - %04X => %04X: %s\n", devName.c_str(),
					ZXTW(src1), ZXTW(src2), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nSUBL2:
			case OPC_nSUBL3:
				src1 = SXTL(opRegs[0]);
				src2 = SXTL(opRegs[1]);
				dst  = src2 - src1;
				StoreL(opRegs[2], opRegs[3], dst);
				UpdateCC_SUB_L(ccReg, dst, src1, src2);
				printf("%s: %08X - %08X => %08X: %s\n", devName.c_str(),
					ZXTL(src1), ZXTL(src2), ZXTL(dst), stringCC(ccReg));
				break;

			// MULx - Multiply instructions
			case OPC_nMULB2:
			case OPC_nMULB3:
				src1 = SXTB(opRegs[0]);
				src2 = SXTB(opRegs[1]);
				dst  = src2 * src1;
				StoreB(opRegs[2], opRegs[3], dst);
				UpdateCC_IIZZ_B(ccReg, dst);
				if (dst < SCHAR_MIN || dst > SCHAR_MAX)
					ccReg |= CC_V;
				printf("%s: %02X * %02X => %02X: %s\n", devName.c_str(),
					ZXTB(src2), ZXTB(src1), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nMULW2:
			case OPC_nMULW3:
				src1 = SXTW(opRegs[0]);
				src2 = SXTW(opRegs[1]);
				dst  = src2 * src1;
				StoreW(opRegs[2], opRegs[3], dst);
				UpdateCC_IIZZ_W(ccReg, dst);
				if (dst < SHRT_MIN || dst > SHRT_MAX)
					ccReg |= CC_V;
				printf("%s: %02X * %02X => %02X: %s\n", devName.c_str(),
					ZXTW(src2), ZXTW(src1), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nMULL2:
			case OPC_nMULL3:
				srcq1 = SXTL(opRegs[0]);
				srcq2 = SXTL(opRegs[1]);
				dstq  = srcq2 * srcq1;
				StoreL(opRegs[2], opRegs[3], SXTL(dstq));
				UpdateCC_IIZZ_L(ccReg, dstq);
//				if (SXTL(dstq >> 32) != (SXTL(dstq) & SGN_LONG) ? -1LL : 0LL)
//					ccReg |= CC_V;
				if (dstq < LONG_MIN || dstq > LONG_MAX)
					ccReg |= CC_V;
				printf("%s: %08X * %08X => %08X: %s\n", devName.c_str(),
					ZXTL(srcq2), ZXTL(srcq1), ZXTL(dstq), stringCC(ccReg));
				break;

			// DIVx - Divide instructions
			case OPC_nDIVB2:
			case OPC_nDIVB3:
				src1 = SXTB(opRegs[0]);
				src2 = SXTB(opRegs[1]);
				if (src1 == 0) {
					dst   = src2;
					ovflg = true;
				} else if (src1 == -1 && src2 == SCHAR_MIN) {
					dst   = src2;
					ovflg = true;
				} else {
					dst   = src2 / src1;
					ovflg = false;
				}
				StoreB(opRegs[2], opRegs[3], dst);
				UpdateCC_IIZZ_B(ccReg, dst);
				if (ovflg)
					ccReg |= CC_V;
				printf("%s: %02X / %02X => %02X: %s\n", devName.c_str(),
					ZXTB(src2), ZXTB(src1), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nDIVW2:
			case OPC_nDIVW3:
				src1 = SXTW(opRegs[0]);
				src2 = SXTW(opRegs[1]);
				if (src1 == 0) {
					dst   = src2;
					ovflg = true;
				} else if (src1 == -1 && src2 == SHRT_MIN) {
					dst   = src2;
					ovflg = true;
				} else {
					dst   = src2 / src1;
					ovflg = false;
				}
				StoreW(opRegs[2], opRegs[3], dst);
				UpdateCC_IIZZ_W(ccReg, dst);
				if (ovflg)
					ccReg |= CC_V;
				printf("%s: %04X / %04X => %04X: %s\n", devName.c_str(),
					ZXTW(src2), ZXTW(src1), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nDIVL2:
			case OPC_nDIVL3:
				src1 = SXTL(opRegs[0]);
				src2 = SXTL(opRegs[1]);
				if (src1 == 0) {
					dst   = src2;
					ovflg = true;
				} else if (src1 == -1 && src2 == LONG_MIN) {
					dst   = src2;
					ovflg = true;
				} else {
					dst   = src2 / src1;
					ovflg = false;
				}
				StoreL(opRegs[2], opRegs[3], dst);
				UpdateCC_IIZZ_L(ccReg, dst);
				if (ovflg)
					ccReg |= CC_V;
				printf("%s: %08X / %08X => %08X: %s\n", devName.c_str(),
					ZXTL(src2), ZXTL(src1), ZXTL(dst), stringCC(ccReg));
				break;

			// ADWC/SBWC instructions
			case OPC_nADWC:
				src1  = SXTL(opRegs[0]);
				src2  = SXTL(opRegs[1]);
				carry = ccReg & CC_C;
				dst   = src2 + src1 + carry;
				StoreL(opRegs[2], opRegs[3], dst);
				UpdateCC_ADD_L(ccReg, dst, src1, src2);
				if ((dst == src2) && src1 != 0)
					ccReg |= CC_C;
				printf("%s: %08X + %08X + %d => %08X: %s\n", devName.c_str(),
					ZXTL(src1), ZXTL(src2), carry, ZXTL(dst), stringCC(ccReg));
				break;
			case OPC_nSBWC:
				src1  = SXTL(opRegs[0]);
				src2  = SXTL(opRegs[1]);
				carry = ccReg & CC_C;
				dst   = src2 - src1 - carry;
				StoreL(opRegs[2], opRegs[3], dst);
				UpdateCC_SUB_L(ccReg, dst, src1, src2);
				if ((src1 == src2) && dst != 0)
					ccReg |= CC_C;
				printf("%s: %08X - %08X - %d => %08X: %s\n", devName.c_str(),
					ZXTL(src1), ZXTL(src2), carry, ZXTL(dst), stringCC(ccReg));
				break;

			// BICx instructions
			case OPC_nBICB2:
			case OPC_nBICB3:
				mask = ZXTB(opRegs[0]);
				usrc = ZXTB(opRegs[1]);
				udst = usrc & ~mask;
				StoreB(opRegs[2], opRegs[3], udst);
				UpdateCC_IIZP_B(ccReg, udst);
				printf("%s: %02X & ~%02X => %02X\n", devName.c_str(),
						ZXTB(usrc), ZXTB(mask), ZXTB(udst), stringCC(ccReg));
				break;
			case OPC_nBICW2:
			case OPC_nBICW3:
				mask = ZXTW(opRegs[0]);
				usrc = ZXTW(opRegs[1]);
				udst = usrc & ~mask;
				StoreW(opRegs[2], opRegs[3], udst);
				UpdateCC_IIZP_W(ccReg, udst);
				printf("%s: %04X & ~%04X => %04X: %s\n", devName.c_str(),
						ZXTW(usrc), ZXTW(mask), ZXTW(udst), stringCC(ccReg));
				break;
			case OPC_nBICL2:
			case OPC_nBICL3:
				mask = ZXTL(opRegs[0]);
				usrc = ZXTL(opRegs[1]);
				udst = usrc & ~mask;
				StoreL(opRegs[2], opRegs[3], udst);
				UpdateCC_IIZP_L(ccReg, udst);
				printf("%s: %08X & ~%08X => %08X: %s\n", devName.c_str(),
						ZXTL(usrc), ZXTL(mask), ZXTL(udst), stringCC(ccReg));
				break;

			// BISx instructions
			case OPC_nBISB2:
			case OPC_nBISB3:
				mask = ZXTB(opRegs[0]);
				usrc = ZXTB(opRegs[1]);
				udst = usrc | mask;
				StoreB(opRegs[2], opRegs[3], udst);
				UpdateCC_IIZP_B(ccReg, udst);
				printf("%s: %02X | %02X => %02X: %s\n", devName.c_str(),
						ZXTB(usrc), ZXTB(mask), ZXTB(udst), stringCC(ccReg));
				break;
			case OPC_nBISW2:
			case OPC_nBISW3:
				mask = ZXTW(opRegs[0]);
				usrc = ZXTW(opRegs[1]);
				udst = usrc | mask;
				StoreW(opRegs[2], opRegs[3], udst);
				UpdateCC_IIZP_W(ccReg, udst);
				printf("%s: %04X | %04X => %04X: %s\n", devName.c_str(),
						ZXTW(usrc), ZXTW(mask), ZXTW(udst), stringCC(ccReg));
				break;
			case OPC_nBISL2:
			case OPC_nBISL3:
				mask = ZXTL(opRegs[0]);
				usrc = ZXTL(opRegs[1]);
				udst = usrc | mask;
				StoreL(opRegs[2], opRegs[3], udst);
				UpdateCC_IIZP_L(ccReg, udst);
				printf("%s: %08X | %08X => %08X: %s\n", devName.c_str(),
						ZXTL(usrc), ZXTL(mask), ZXTL(udst), stringCC(ccReg));
				break;

			// BITx instructions
			case OPC_nBITB:
				mask = ZXTB(opRegs[0]);
				usrc = ZXTB(opRegs[1]);
				udst = usrc & mask;
				UpdateCC_IIZP_B(ccReg, udst);
				printf("%s: %02X & %02X => %02X: %s\n", devName.c_str(),
						ZXTB(usrc), ZXTB(mask), ZXTB(udst), stringCC(ccReg));
				break;
			case OPC_nBITW:
				mask = ZXTW(opRegs[0]);
				usrc = ZXTW(opRegs[1]);
				udst = usrc & mask;
				UpdateCC_IIZP_W(ccReg, udst);
				printf("%s: %04X & %04X => %04X: %s\n", devName.c_str(),
						ZXTW(usrc), ZXTW(mask), ZXTW(udst), stringCC(ccReg));
				break;
			case OPC_nBITL:
				mask = ZXTL(opRegs[0]);
				usrc = ZXTL(opRegs[1]);
				udst = usrc & mask;
				UpdateCC_IIZP_L(ccReg, udst);
				printf("%s: %08X & %08X => %08X: %s\n", devName.c_str(),
						ZXTL(usrc), ZXTL(mask), ZXTL(udst), stringCC(ccReg));
				break;

			// XORx instructions
			case OPC_nXORB2:
			case OPC_nXORB3:
				mask = ZXTB(opRegs[0]);
				usrc = ZXTB(opRegs[1]);
				udst = usrc ^ mask;
				StoreB(opRegs[2], opRegs[3], udst);
				UpdateCC_IIZP_B(ccReg, udst);
				printf("%s: %02X ^ %02X => %02X: %s\n", devName.c_str(),
						ZXTB(usrc), ZXTB(mask), ZXTB(udst), stringCC(ccReg));
				break;
			case OPC_nXORW2:
			case OPC_nXORW3:
				mask = ZXTW(opRegs[0]);
				usrc = ZXTW(opRegs[1]);
				udst = usrc ^ mask;
				StoreW(opRegs[2], opRegs[3], udst);
				UpdateCC_IIZP_W(ccReg, udst);
				printf("%s: %04X ^ %04X => %04X: %s\n", devName.c_str(),
						ZXTW(usrc), ZXTW(mask), ZXTW(udst), stringCC(ccReg));
				break;
			case OPC_nXORL2:
			case OPC_nXORL3:
				mask = ZXTL(opRegs[0]);
				usrc = ZXTL(opRegs[1]);
				udst = usrc ^ mask;
				StoreL(opRegs[2], opRegs[3], udst);
				UpdateCC_IIZP_L(ccReg, udst);
				printf("%s: %08X ^ %08X => %08X: %s\n", devName.c_str(),
						ZXTL(usrc), ZXTL(mask), ZXTL(udst), stringCC(ccReg));
				break;

			// ASHx/ROTL - Shift instructions
			case OPC_nASHL:
				cnt   = SXTB(opRegs[0]);
				src   = SXTL(opRegs[1]);
				ovflg = false;
				if (cnt == 0) {
					dst = src;
				} else if (cnt < 0) {
					dst = (cnt > -32) ? (src >> -cnt) : (src < 0) ? -1 : 0;
				} else {
					if (cnt < 32) {
						dst = ZXTL(src) << cnt;
						ovflg = (src != (dst >> cnt));
					} else {
						dst = 0;
						ovflg = (src != 0);
					}
				}
				StoreL(opRegs[2], opRegs[3], dst);
				UpdateCC_IIZZ_L(ccReg, dst);
				if (ovflg)
					ccReg |= CC_V;
				printf("%s: %08X %s %d => %08X: %s\n", devName.c_str(),
					ZXTL(src), ((cnt < 0) ? ">>" : "<<"), abs(cnt),
					ZXTL(dst), stringCC(ccReg));
				break;

			case OPC_nASHQ:
				cnt    = SXTB(opRegs[0]);
				srcq  = (ZXTQ(opRegs[2]) << 32) | ZXTL(opRegs[1]);
				ovflg = false;
				if (cnt == 0) {
					dstq = srcq;
				} else if (cnt < 0) {
					dstq = (cnt > -64) ? (srcq >> -cnt) : (srcq < 0) ? -1LL : 0LL;
				} else {
					if (cnt < 64) {
						dstq  = srcq << cnt;
						ovflg = (srcq != (dstq >> cnt));
					} else {
						dstq  = 0LL;
						ovflg = (srcq != 0);
					}
				}
				StoreQ(opRegs[3], opRegs[4], SXTL(dstq), SXTL(dstq >> 32));
				UpdateCC_IIZZ_64(ccReg, dstq);
				if (ovflg)
					ccReg |= CC_V;
				printf("%s: %08X %08X %s %d => %08X %08X: %s\n", devName.c_str(),
					ZXTL(srcq >> 32), ZXTL(srcq), ((cnt < 0) ? ">>" : "<<"), abs(cnt),
					ZXTL(dstq >> 32), ZXTL(dstq), stringCC(ccReg));
				break;

			case OPC_nROTL:
				cnt  = ZXTB(opRegs[0]) % 32;
				usrc = ZXTL(opRegs[1]);
				udst = (cnt != 0) ? ((usrc << cnt) | (usrc >> (32 - cnt))) : usrc;
				StoreL(opRegs[2], opRegs[3], udst);
				UpdateCC_IIZP_L(ccReg, udst);
				printf("%s: %08X %s %d => %08X: %s\n", devName.c_str(),
					ZXTL(usrc), ((cnt < 0) ? ">>" : "<<"), abs(cnt),
					ZXTL(udst), stringCC(ccReg));
				break;

			// CMPV - Compare field instructions
			case OPC_nCMPV:
				dst = getField(true);
				src = SXTL(opRegs[4]);
				UpdateCC_CMP_L(ccReg, dst, src);
				printf("%s: Compare %08X with %08X: %s\n", devName.c_str(),
					ZXTL(dst), ZXTL(src), stringCC(ccReg));
				break;
			case OPC_nCMPZV:
				dst = getField(false);
				src = SXTL(opRegs[4]);
				UpdateCC_CMP_L(ccReg, dst, src);
				printf("%s: Compare %08X with %08X: %s\n", devName.c_str(),
					ZXTL(dst), ZXTL(src), stringCC(ccReg));
				break;

			// EXTV - Extract field instructions
			case OPC_nEXTV:
				src = getField(true);
				StoreL(opRegs[4], opRegs[5], src);
				UpdateCC_IIZP_L(ccReg, src);
				printf("%s: Extract %08X: %s\n", devName.c_str(),
					ZXTL(src), stringCC(ccReg));
				break;
			case OPC_nEXTZV:
				src = getField(false);
				StoreL(opRegs[4], opRegs[5], src);
				UpdateCC_IIZP_L(ccReg, src);
				printf("%s: Extract %08X: %s\n", devName.c_str(),
					ZXTL(src), stringCC(ccReg));
				break;

//			case OPC_nFFS:
//			case OPC_nFFC:
//			case OPC_nINSV:

			// ******************
			// Stack instructions
			// ******************

			// PUSHx/PUSHAx - Push instruction
			case OPC_nPUSHL:
			case OPC_nPUSHAB:
			case OPC_nPUSHAW:
			case OPC_nPUSHAL:
			case OPC_nPUSHAQ:
				src = opRegs[0];
				writev(REG_SP - LN_LONG, src, LN_LONG, WACC);
				REG_SP -= LN_LONG;
				UpdateCC_IIZP_L(ccReg, src);
				printf("%s: Push %08X to SP (%08X): %s\n", devName.c_str(),
						src, REG_SP + LN_LONG, stringCC(ccReg));
				break;

			// PUSHR/POPR - Push/pop register instructions
			case OPC_nPUSHR:
				mask = SXTL(opRegs[0]) & STK_MASK;
				// Check access for page faults

				// Push registers into stack
				for (int idx = REG_nSP; idx >= REG_nR0; idx--) {
					if ((mask >> idx) & 1) {
						writev(REG_SP - LN_LONG, gRegs[idx].l, LN_LONG, WACC);
						printf("%s: R%d %08X => (SP) %08X\n", devName.c_str(),
							idx, ZXTL(gRegs[idx].l), ZXTL(REG_SP));
						REG_SP -= LN_LONG;
					}
				}
				break;
			case OPC_nPOPR:
				mask = SXTL(opRegs[0]) & STK_MASK;
				// Check access for page faults

				// Push registers into stack
				for (int idx = REG_nR0; idx <= REG_nSP; idx++) {
					if ((mask >> idx) & 1) {
						gRegs[idx].l = readv(REG_SP, LN_LONG, RACC);
						printf("%s: R%d %08X <= (SP) %08X\n", devName.c_str(),
							idx, ZXTL(gRegs[idx].l), ZXTL(REG_SP));
						if (idx < REG_nSP)
							REG_SP += LN_LONG;
					}
				}
				break;

			// CALLx - Call instructions
			case OPC_nCALLS:
				call(true);
				break;
			case OPC_nCALLG:
				call(false);
				break;

			// RET - Return instruction
			case OPC_nRET:
				ret();
				break;

			case OPC_nCVTFD:
			case OPC_nCVTLG:
				throw EXC_RSVD_INST_FAULT;

			// Illegal/unimplemented instruction
			default:
				if (opc->opCode != OPC_nUOPC)
					throw STOP_UOPC;
				throw EXC_RSVD_INST_FAULT;
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

			// Exception fault codes
			case EXC_RSVD_INST_FAULT:
				if (fault(SCB_RESIN))
					return;
				break;
			case EXC_RSVD_ADDR_FAULT:
				if (fault(SCB_RESAD))
					return;
				break;
			case EXC_RSVD_OPND_FAULT:
				if (fault(SCB_RESOP))
					return;
				break;
			case EXC_PRIV_INST_FAULT:
				if (fault(SCB_RESIN|SCB_NOPRIV))
					return;
				break;
			}
		}
	}
}
