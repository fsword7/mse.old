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
	register uint32_t src, src1, src2;
	register int32_t  ssrc1, ssrc2, sdst, stmp;
	register uint32_t dst, dst1, dst2;
	register uint32_t mask;
	register int32_t  cnt;

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

			// PUSHL instruction
			case OPC_nPUSHL:
				ssrc1 = opRegs[0];
				writev(REG_SP - LN_LONG, ssrc1, LN_LONG, WACC);
				REG_SP -= LN_LONG;
				UpdateCC_IIZP_L(ccReg, ssrc1);
				printf("%s: Push %08X to SP (%08X): %s\n", devName.c_str(),
						ssrc1, REG_SP + LN_LONG, stringCC(ccReg));
				break;

			// AOB instructions
			case OPC_nAOBLEQ:
				ssrc1 = opRegs[0];
				ssrc2 = opRegs[1];
				sdst  = ssrc2 + 1;
				StoreL(opRegs[2], opRegs[3], sdst);
				UpdateCC_IIZP_L(ccReg, sdst);
				UpdateV_ADD_L(ccReg, sdst, 1, ssrc2);
				if (sdst <= src1) {
					REG_PC = REG_PC + SXTB(brDisp);
					flushvi();
				}
				printf("%s: Inc %08X => %08X <= %08X: %s\n", devName.c_str(),
						ssrc2, sdst, ssrc1, stringCC(ccReg));
				if (sdst <= src1)
					printf("%s: Jump into PC %08X\n", devName.c_str(), REG_PC);
				break;
			case OPC_nAOBLSS:
				ssrc1 = opRegs[0];
				ssrc2 = opRegs[1];
				sdst  = ssrc2 + 1;
				StoreL(opRegs[2], opRegs[3], sdst);
				UpdateCC_IIZP_L(ccReg, sdst);
				UpdateV_ADD_L(ccReg, sdst, 1, ssrc2);
				if (sdst < src1) {
					REG_PC = REG_PC + SXTB(brDisp);
					flushvi();
				}
				printf("%s: Inc %08X => %08X < %08X: %s\n", devName.c_str(),
						ssrc2, sdst, ssrc1, stringCC(ccReg));
				if (sdst <= src1)
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
				printf("%s: Move %02X: %s\n", devName.c_str(), uint8_t(dst), stringCC(ccReg));
				break;
			case OPC_nMOVW:
				dst = opRegs[0];
				StoreW(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZP_W(ccReg, dst);
				printf("%s: Move %04X: %s\n", devName.c_str(), uint16_t(dst), stringCC(ccReg));
				break;
			case OPC_nMOVL:
			case OPC_nMOVAB:
			case OPC_nMOVAW:
			case OPC_nMOVAL:
			case OPC_nMOVAQ:
				dst = opRegs[0];
				StoreL(opRegs[1], opRegs[2], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				printf("%s: Move %08X: %s\n", devName.c_str(), uint32_t(dst), stringCC(ccReg));
				break;
			case OPC_nMOVQ:
				dst1 = opRegs[0];
				dst2 = opRegs[1];
				StoreQ(opRegs[2], opRegs[3], dst1, dst2);
				UpdateCC_IIZP_Q(ccReg, dst1, dst2);
				printf("%s: Move %08X %08X: %s\n", devName.c_str(),
						uint32_t(dst1), uint32_t(dst2), stringCC(ccReg));
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
				ssrc1 = SXTB(opRegs[0]);
				ssrc2 = SXTB(opRegs[1]);
				UpdateCC_CMP_B(ccReg, ssrc1, ssrc2);
				printf("%s: Compare %02X with %02X: %s\n", devName.c_str(),
						ZXTB(ssrc1), ZXTB(ssrc2), stringCC(ccReg));
				break;
			case OPC_nCMPW:
				ssrc1 = SXTW(opRegs[0]);
				ssrc2 = SXTW(opRegs[1]);
				UpdateCC_CMP_W(ccReg, ssrc1, ssrc2);
				printf("%s: Compare %04X with %04X: %s\n", devName.c_str(),
						ZXTW(ssrc1), ZXTW(ssrc2), stringCC(ccReg));
				break;
			case OPC_nCMPL:
				ssrc1 = SXTL(opRegs[0]);
				ssrc2 = SXTL(opRegs[1]);
				UpdateCC_CMP_L(ccReg, ssrc1, ssrc2);
				printf("%s: Compare %08X with %08X: %s\n", devName.c_str(),
						ZXTL(ssrc1), ZXTL(ssrc2), stringCC(ccReg));
				break;

				// TSTx instructions
			case OPC_nTSTB:
				ssrc1 = SXTB(opRegs[0]);
				UpdateCC_IIZZ_B(ccReg, ssrc1);
				printf("%s: Test %02X: %s\n", devName.c_str(), ZXTB(ssrc1), stringCC(ccReg));
				break;
			case OPC_nTSTW:
				ssrc1 = SXTW(opRegs[0]);
				UpdateCC_IIZZ_W(ccReg, ssrc1);
				printf("%s: Test %04X: %s\n", devName.c_str(), ZXTW(ssrc1), stringCC(ccReg));
				break;
			case OPC_nTSTL:
				ssrc1 = SXTL(opRegs[0]);
				UpdateCC_IIZZ_L(ccReg, ssrc1);
				printf("%s: Test %08X: %s\n", devName.c_str(), ZXTL(ssrc1), stringCC(ccReg));
				break;

			// INCx instructions
			case OPC_nINCB:
				ssrc1 = SXTB(opRegs[0]);
				sdst  = ssrc1 + 1;
				StoreB(opRegs[1], opRegs[2], sdst);
				UpdateCC_ADD_B(ccReg, sdst, 1, ssrc1);
				printf("%s: %02X + 1 => %02X: %s\n",
					devName.c_str(), ZXTB(ssrc1), ZXTB(sdst), stringCC(ccReg));
				break;
			case OPC_nINCW:
				ssrc1 = SXTW(opRegs[0]);
				sdst  = ssrc1 + 1;
				StoreW(opRegs[1], opRegs[2], sdst);
				UpdateCC_ADD_W(ccReg, sdst, 1, ssrc1);
				printf("%s: %04X + 1 => %04X: %s\n",
					devName.c_str(), ZXTW(ssrc1), ZXTW(sdst), stringCC(ccReg));
				break;
			case OPC_nINCL:
				ssrc1 = SXTL(opRegs[0]);
				sdst  = ssrc1 + 1;
				StoreL(opRegs[1], opRegs[2], sdst);
				UpdateCC_ADD_L(ccReg, sdst, 1, ssrc1);
				printf("%s: %08X + 1 => %08X: %s\n",
					devName.c_str(), ZXTL(ssrc1), ZXTL(sdst), stringCC(ccReg));
				break;

			// DECx instructions
			case OPC_nDECB:
				ssrc1 = SXTB(opRegs[0]);
				sdst = ssrc1 - 1;
				StoreB(opRegs[1], opRegs[2], sdst);
				UpdateCC_SUB_B(ccReg, sdst, 1, ssrc1);
				printf("%s: %08X - 1 => %08X: %s\n",
					devName.c_str(), ZXTB(ssrc1), ZXTB(sdst), stringCC(ccReg));
				break;
			case OPC_nDECW:
				ssrc1 = SXTW(opRegs[0]);
				sdst = ssrc1 - 1;
				StoreW(opRegs[1], opRegs[2], sdst);
				UpdateCC_SUB_W(ccReg, sdst, 1, ssrc1);
				printf("%s: %04X - 1 => %04X: %s\n",
					devName.c_str(), ZXTW(ssrc1), ZXTW(sdst), stringCC(ccReg));
				break;
			case OPC_nDECL:
				ssrc1 = SXTL(opRegs[0]);
				sdst = ssrc1 - 1;
				StoreL(opRegs[1], opRegs[2], sdst);
				UpdateCC_SUB_L(ccReg, sdst, 1, ssrc1);
				printf("%s: %08X - 1 => %08X: %s\n",
					devName.c_str(), ZXTL(ssrc1), ZXTL(sdst), stringCC(ccReg));
				break;

			// ADAWI instruction
			case OPC_nADAWI:
				ssrc1 = SXTW(opRegs[0]);
				if (opRegs[1] != OPR_MEM) {
					stmp = SXTW(gRegs[opRegs[1]].l);
					sdst = ssrc1 + stmp;
					gRegs[opRegs[1]].l = ZXTW(sdst);
				} else {
//					if (opRegs[1] & 1)
//						throw EXC_RSVD_OPND_FAULT;
					stmp = SXTW(readv(opRegs[2], LN_WORD, RACC));
					sdst = ssrc1 + stmp;
					writev(opRegs[2], sdst, LN_WORD, WACC);
				}
				UpdateCC_ADD_W(ccReg, sdst, ssrc1, stmp);
				printf("%s: %04X + %04X => %04X: %s\n", devName.c_str(),
						ZXTW(ssrc1), ZXTW(stmp), ZXTW(sdst), stringCC(ccReg));
				break;

			// BICx instructions
			case OPC_nBICB2:
			case OPC_nBICB3:
				mask = opRegs[0];
				src  = opRegs[1];
				dst  = src & ~mask;
				StoreB(opRegs[2], opRegs[3], dst);
				UpdateCC_IIZP_B(ccReg, dst);
				printf("%s: %02X & ~%02X => %02X\n", devName.c_str(),
						uint8_t(src), uint8_t(mask), uint8_t(dst));
				break;
			case OPC_nBICW2:
			case OPC_nBICW3:
				mask = opRegs[0];
				src  = opRegs[1];
				dst  = src & ~mask;
				StoreW(opRegs[2], opRegs[3], dst);
				UpdateCC_IIZP_W(ccReg, dst);
				printf("%s: %04X & ~%04X => %04X\n", devName.c_str(),
						uint16_t(src), uint16_t(mask), uint16_t(dst));
				break;
			case OPC_nBICL2:
			case OPC_nBICL3:
				mask = opRegs[0];
				src  = opRegs[1];
				dst  = src & ~mask;
				StoreL(opRegs[2], opRegs[3], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				printf("%s: %08X & ~%08X => %08X\n", devName.c_str(),
						uint32_t(src), uint32_t(mask), uint32_t(dst));
				break;

			// BISx instructions
			case OPC_nBISB2:
			case OPC_nBISB3:
				mask = opRegs[0];
				src  = opRegs[1];
				dst  = src | mask;
				StoreB(opRegs[2], opRegs[3], dst);
				UpdateCC_IIZP_B(ccReg, dst);
				printf("%s: %02X | %02X => %02X\n", devName.c_str(),
						uint8_t(src), uint8_t(mask), uint8_t(dst));
				break;
			case OPC_nBISW2:
			case OPC_nBISW3:
				mask = opRegs[0];
				src  = opRegs[1];
				dst  = src | mask;
				StoreW(opRegs[2], opRegs[3], dst);
				UpdateCC_IIZP_W(ccReg, dst);
				printf("%s: %04X | %04X => %04X\n", devName.c_str(),
						uint16_t(src), uint16_t(mask), uint16_t(dst));
				break;
			case OPC_nBISL2:
			case OPC_nBISL3:
				mask = opRegs[0];
				src  = opRegs[1];
				dst  = src | mask;
				StoreL(opRegs[2], opRegs[3], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				printf("%s: %08X | %08X => %08X\n", devName.c_str(),
						uint32_t(src), uint32_t(mask), uint32_t(dst));
				break;

			// BITx instructions
			case OPC_nBITB:
				mask = opRegs[0];
				src  = opRegs[1];
				dst  = src & mask;
				UpdateCC_IIZP_B(ccReg, dst);
				printf("%s: %02X & %02X => %02X: %s\n", devName.c_str(),
						ZXTB(src), ZXTB(mask), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nBITW:
				mask = opRegs[0];
				src  = opRegs[1];
				dst  = src & mask;
				UpdateCC_IIZP_W(ccReg, dst);
				printf("%s: %04X & %04X => %04X: %s\n", devName.c_str(),
						ZXTW(src), ZXTW(mask), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nBITL:
				mask = opRegs[0];
				src  = opRegs[1];
				dst  = src & mask;
				UpdateCC_IIZP_L(ccReg, dst);
				printf("%s: %08X & %08X => %08X: %s\n", devName.c_str(),
						ZXTL(src), ZXTL(mask), ZXTL(dst), stringCC(ccReg));
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

			case OPC_nROTL:
				cnt = ZXTB(opRegs[0]) % 32;
				src = opRegs[1];
				dst = (cnt != 0) ? ((src << cnt) | (src >> (32 - cnt))) : src;
				StoreL(opRegs[2], opRegs[3], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				printf("%s: %08X %s %d => %08X: %s\n", devName.c_str(),
					ZXTL(src), ((cnt < 0) ? ">>" : "<<"), abs(cnt),
					ZXTL(dst), stringCC(ccReg));
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
