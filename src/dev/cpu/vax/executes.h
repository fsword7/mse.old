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

// Register validation check
#define Validate(rn, max) if ((rn) >= (max)) throw RSVD_ADDR_FAULT;

//#define Validate(rn, max) \
//	if ((rn) >= (max)) {          \
//		printf("%s: (BAD!!) reg=%d (max %d) idx=%d spec=%02X\n", devName.c_str(), \
//			rn, max, idx, spec);  \
//		throw RSVD_ADDR_FAULT;    \
//	}

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
	register uint32_t usrc, usrc1, usrc2;
	register uint32_t udst, utmp;
	register uint32_t mask;
	register int32_t  cnt;
	register int32_t  entry, pred, succ, hdr;
	register int32_t  a, b, c, ar;

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
			if (opc->flags & OPF_RSVD)
				throw RSVD_INST_FAULT;
//			if (psReg & PSL_FPD) {
//				if ((opc->flags & OPF_FPD) == 0)
//					throw RSVD_INST_FAULT;
//				goto EXECUTE;
//			}

			rqCount = 0;
			for (int idx = 0, opidx = 0; idx < opc->nOperands; idx++) {
				int opr = opc->oprMode[idx];
				int rn, ireg, spec;
				uint32_t mAddr, off;

				switch (opr) {
					case BB: // Byte Branch Mode
						opReg[opidx++] = SXTB(readvi(LN_BYTE));
						continue;

					case BW: // Word Branch Mode
						opReg[opidx++] = SXTW(readvi(LN_WORD));
						continue;

					default:
						spec  = readvi(LN_BYTE);
						rn    = spec & 0xF;
						opr  |= (spec & 0xF0);
				}

//				printf("%s: OP %d %02X => R%d (%04X)\n", devName.c_str(),
//					idx, spec, rn, opr);

				switch(opr) {
					// Short Literal Address Mode
					//   Only read access is allowed.
					case LIT0|RB: case LIT0|RW: case LIT0|RL:
					case LIT1|RB: case LIT1|RW: case LIT1|RL:
					case LIT2|RB: case LIT2|RW: case LIT2|RL:
					case LIT3|RB: case LIT3|RW: case LIT3|RL:
						opReg[opidx++] = spec;
						continue;
					case LIT0|RQ: case LIT1|RQ: case LIT2|RQ: case LIT3|RQ:
						opReg[opidx++] = spec;
						opReg[opidx++] = 0;
						continue;
					case LIT0|RO: case LIT1|RO: case LIT2|RO: case LIT3|RO:
						opReg[opidx++] = spec;
						opReg[opidx++] = 0;
						opReg[opidx++] = 0;
						opReg[opidx++] = 0;
						continue;

					case LIT0|RF: case LIT1|RF: case LIT2|RF: case LIT3|RF:
						opReg[opidx++] = (spec << 4) | 0x4000;
						continue;
					case LIT0|RD: case LIT1|RD: case LIT2|RD: case LIT3|RD:
						opReg[opidx++] = (spec << 4) | 0x4000;
						opReg[opidx++] = 0;
						continue;
					case LIT0|RG: case LIT1|RG: case LIT2|RG: case LIT3|RG:
						opReg[opidx++] = (spec << 1) | 0x4000;
						opReg[opidx++] = 0;
						continue;
					case LIT0|RH: case LIT1|RH: case LIT2|RH: case LIT3|RH:
						opReg[opidx++] =
							((spec & 7) << 29) | (0x4000 | ((spec >> 3) & 7));
						opReg[opidx++] = 0;
						opReg[opidx++] = 0;
						opReg[opidx++] = 0;
						continue;


					// Register Address Mode
					// Read Access for General Registers
					case REG|RB:
						Validate(rn, REG_nPC);
						opReg[opidx++] = CPU_REGUB(rn);
						continue;
					case REG|RW:
						Validate(rn, REG_nPC);
						opReg[opidx++] = CPU_REGUW(rn);
						continue;
					case REG|RL: case REG|RF:
						Validate(rn, REG_nPC);
						opReg[opidx++] = CPU_REGUL(rn);
						continue;
					case REG|RQ: case REG|RD: case REG|RG:
						Validate(rn, REG_nSP);
						opReg[opidx++] = CPU_REGUL(rn);
						opReg[opidx++] = CPU_REGUL(rn+1);
						continue;
					case REG|RO: case REG|RH:
						Validate(rn, REG_nAP);
						opReg[opidx++] = CPU_REGUL(rn);
						opReg[opidx++] = CPU_REGUL(rn+1);
						opReg[opidx++] = CPU_REGUL(rn+2);
						opReg[opidx++] = CPU_REGUL(rn+3);
						continue;

					// Modify Access for General Registers
					case REG|MB:
						Validate(rn, REG_nPC);
						opReg[opidx++] = CPU_REGUB(rn);
						opReg[opidx++] = ~rn;
						continue;
					case REG|MW:
						Validate(rn, REG_nPC);
						opReg[opidx++] = CPU_REGUW(rn);
						opReg[opidx++] = ~rn;
						continue;
					case REG|ML:
						Validate(rn, REG_nPC);
						opReg[opidx++] = CPU_REGUL(rn);
						opReg[opidx++] = ~rn;
						continue;
					case REG|MQ:
						Validate(rn, REG_nSP);
						opReg[opidx++] = CPU_REGUL(rn);
						opReg[opidx++] = CPU_REGUL(rn+1);
						opReg[opidx++] = ~rn;
						continue;
					case REG|MO:
						Validate(rn, REG_nAP);
						opReg[opidx++] = CPU_REGUL(rn);
						opReg[opidx++] = CPU_REGUL(rn+1);
						opReg[opidx++] = CPU_REGUL(rn+2);
						opReg[opidx++] = CPU_REGUL(rn+3);
						opReg[opidx++] = ~rn;
						continue;

					// Write Access for General Registers
					case REG|WB: case REG|WW: case REG|WL: case REG|WQ:
					case REG|WO: case REG|VB:
						Validate(rn, REG_nPC);
						opReg[opidx++] = ~rn;
						continue;


					// Register Deferred Address Mode (Rn)
					case REGD|RB: case REGD|RW: case REGD|RL: case REGD|RF:
						Validate(rn, REG_nPC);
						opReg[opidx++] = readv(CPU_REGUL(rn), OPM_SIZE(opr), RACC);
						continue;
					case REGD|RQ: case REGD|RD: case REGD|RG:
						Validate(rn, REG_nPC);
						opReg[opidx++] = readv(CPU_REGUL(rn),   LN_LONG, RACC);
						opReg[opidx++] = readv(CPU_REGUL(rn)+4, LN_LONG, RACC);
						continue;
					case REGD|RO: case REGD|RH:
						Validate(rn, REG_nPC);
						opReg[opidx++] = readv(CPU_REGUL(rn),    LN_LONG, RACC);
						opReg[opidx++] = readv(CPU_REGUL(rn)+4,  LN_LONG, RACC);
						opReg[opidx++] = readv(CPU_REGUL(rn)+8,  LN_LONG, RACC);
						opReg[opidx++] = readv(CPU_REGUL(rn)+12, LN_LONG, RACC);
						continue;

					case REGD|MB: case REGD|MW: case REGD|ML:
						Validate(rn, REG_nPC);
						opReg[opidx++] = readv(CPU_REGUL(rn), OPM_SIZE(opr), WACC);
						opReg[opidx++] = CPU_REGUL(rn);
						continue;
					case REGD|MQ:
						Validate(rn, REG_nPC);
						opReg[opidx++] = readv(CPU_REGUL(rn),   LN_LONG, WACC);
						opReg[opidx++] = readv(CPU_REGUL(rn)+4, LN_LONG, WACC);
						opReg[opidx++] = CPU_REGUL(rn);
						continue;
					case REGD|MO:
						Validate(rn, REG_nPC);
						opReg[opidx++] = readv(CPU_REGUL(rn),    LN_LONG, WACC);
						opReg[opidx++] = readv(CPU_REGUL(rn)+4,  LN_LONG, WACC);
						opReg[opidx++] = readv(CPU_REGUL(rn)+8,  LN_LONG, WACC);
						opReg[opidx++] = readv(CPU_REGUL(rn)+12, LN_LONG, WACC);
						opReg[opidx++] = CPU_REGUL(rn);
						continue;

					case REGD|WB: case REGD|WW: case REGD|WL: case REGD|WQ:
					case REGD|AB: case REGD|AW: case REGD|AL: case REGD|AQ:
					case REGD|WO: case REGD|AO: case REGD|VB:
						Validate(rn, REG_nPC);
						opReg[opidx++] = CPU_REGUL(rn);
						continue;


					// Autodecrement Address Mode -(Rn)
					case ADEC|RB: case ADEC|RW: case ADEC|RL: case ADEC|RF:
						Validate(rn, REG_nPC);
						gpReg[rn].l     -= OPM_SIZE(opr);
						rqReg[rqCount++] = (OPM_SIZE(opr) << 4) | rn;
						opReg[opidx++]   = readv(CPU_REGUL(rn), OPM_SIZE(opr), RACC);
						continue;
					case ADEC|RQ: case ADEC|RD: case ADEC|RG:
						Validate(rn, REG_nPC);
						gpReg[rn].l     -= LN_QUAD;
						rqReg[rqCount++] = (LN_QUAD << 4) | rn;
						opReg[opidx++]   = readv(CPU_REGUL(rn),   LN_LONG, RACC);
						opReg[opidx++]   = readv(CPU_REGUL(rn)+4, LN_LONG, RACC);
						continue;
					case ADEC|RO: case ADEC|RH:
						Validate(rn, REG_nPC);
						gpReg[rn].l     -= LN_OCTA;
						rqReg[rqCount++] = (LN_OCTA << 4) | rn;
						opReg[opidx++]   = readv(CPU_REGUL(rn),    LN_LONG, RACC);
						opReg[opidx++]   = readv(CPU_REGUL(rn)+4,  LN_LONG, RACC);
						opReg[opidx++]   = readv(CPU_REGUL(rn)+8,  LN_LONG, RACC);
						opReg[opidx++]   = readv(CPU_REGUL(rn)+12, LN_LONG, RACC);
						continue;

					case ADEC|MB: case ADEC|MW: case ADEC|ML:
						Validate(rn, REG_nPC);
						gpReg[rn].l     -= OPM_SIZE(opr);
						rqReg[rqCount++] = (OPM_SIZE(opr) << 4) | rn;
						opReg[opidx++]   = readv(CPU_REGUL(rn), OPM_SIZE(opr), WACC);
						opReg[opidx++]   = CPU_REGUL(rn);
						continue;
					case ADEC|MQ:
						Validate(rn, REG_nPC);
						gpReg[rn].l     -= LN_QUAD;
						rqReg[rqCount++] = (LN_QUAD << 4) | rn;
						opReg[opidx++]   = readv(CPU_REGUL(rn),   LN_LONG, WACC);
						opReg[opidx++]   = readv(CPU_REGUL(rn)+4, LN_LONG, WACC);
						opReg[opidx++]   = CPU_REGUL(rn);
						continue;
					case ADEC|MO:
						Validate(rn, REG_nPC);
						gpReg[rn].l     -= LN_OCTA;
						rqReg[rqCount++] = (LN_OCTA << 4) | rn;
						opReg[opidx++]   = readv(CPU_REGUL(rn),    LN_LONG, WACC);
						opReg[opidx++]   = readv(CPU_REGUL(rn)+4,  LN_LONG, WACC);
						opReg[opidx++]   = readv(CPU_REGUL(rn)+8,  LN_LONG, WACC);
						opReg[opidx++]   = readv(CPU_REGUL(rn)+12, LN_LONG, WACC);
						opReg[opidx++]   = CPU_REGUL(rn);
						continue;

					case ADEC|WB: case ADEC|WW: case ADEC|WL: case ADEC|WQ:
					case ADEC|AB: case ADEC|AW: case ADEC|AL: case ADEC|AQ:
					case ADEC|WO: case ADEC|AO: case ADEC|VB:
						Validate(rn, REG_nPC);
						gpReg[rn].l     -= OPM_SIZE(opr);
						rqReg[rqCount++] = (OPM_SIZE(opr) << 4) | rn;
						opReg[opidx++]   = CPU_REGUL(rn);
						continue;

					// Autoincrement/Immediate Address Mode
					case AINC|RB: case AINC|RW: case AINC|RL: case AINC|RF:
						if (rn < REG_nPC) {
							opReg[opidx++]   = readv(CPU_REGUL(rn), OPM_SIZE(opr), RACC);
							rqReg[rqCount++] = (-OPM_SIZE(opr) << 4) | rn;
							gpReg[rn].l     += OPM_SIZE(opr);
						} else
							opReg[opidx++] = readvi(OPM_SIZE(opr));
						continue;

					case AINC|RQ: case AINC|RD: case AINC|RG:
						if (rn < REG_nPC) {
							opReg[opidx++]   = readv(CPU_REGUL(rn),   LN_LONG, RACC);
							opReg[opidx++]   = readv(CPU_REGUL(rn)+4, LN_LONG, RACC);
							rqReg[rqCount++] = (-LN_QUAD << 4) | rn;
							gpReg[rn].l     += LN_QUAD;
						} else {
							opReg[opidx++] = readvi(LN_LONG);
							opReg[opidx++] = readvi(LN_LONG);
						}
						continue;

					case AINC|RO: case AINC|RH:
						if (rn < REG_nPC) {
							opReg[opidx++]   = readv(CPU_REGUL(rn),    LN_LONG, RACC);
							opReg[opidx++]   = readv(CPU_REGUL(rn)+4,  LN_LONG, RACC);
							opReg[opidx++]   = readv(CPU_REGUL(rn)+8,  LN_LONG, RACC);
							opReg[opidx++]   = readv(CPU_REGUL(rn)+12, LN_LONG, RACC);
							rqReg[rqCount++] = (-LN_OCTA << 4) | rn;
							gpReg[rn].l     += LN_OCTA;
						} else {
							opReg[opidx++] = readvi(LN_LONG);
							opReg[opidx++] = readvi(LN_LONG);
							opReg[opidx++] = readvi(LN_LONG);
							opReg[opidx++] = readvi(LN_LONG);
						}
						continue;

					case AINC|MB: case AINC|MW: case AINC|ML:
						Validate(rn, REG_nPC);
						opReg[opidx++]   = readv(CPU_REGUL(rn), OPM_SIZE(opr), WACC);
						opReg[opidx++]   = CPU_REGUL(rn);
						rqReg[rqCount++] = (-OPM_SIZE(opr) << 4) | rn;
						gpReg[rn].l     += OPM_SIZE(opr);
						continue;

					case AINC|MQ:
						Validate(rn, REG_nPC);
						opReg[opidx++]   = readv(CPU_REGUL(rn),   LN_LONG, WACC);
						opReg[opidx++]   = readv(CPU_REGUL(rn)+4, LN_LONG, WACC);
						opReg[opidx++]   = CPU_REGUL(rn);
						rqReg[rqCount++] = (-LN_QUAD << 4) | rn;
						gpReg[rn].l     += LN_QUAD;
						continue;

					case AINC|MO:
						Validate(rn, REG_nPC);
						opReg[opidx++]   = readv(CPU_REGUL(rn),    LN_LONG, WACC);
						opReg[opidx++]   = readv(CPU_REGUL(rn)+4,  LN_LONG, WACC);
						opReg[opidx++]   = readv(CPU_REGUL(rn)+8,  LN_LONG, WACC);
						opReg[opidx++]   = readv(CPU_REGUL(rn)+12, LN_LONG, WACC);
						opReg[opidx++]   = CPU_REGUL(rn);
						rqReg[rqCount++] = (-LN_OCTA << 4) | rn;
						gpReg[rn].l     += LN_OCTA;
						continue;

					case AINC|WB: case AINC|WW: case AINC|WL:
					case AINC|AB: case AINC|AW: case AINC|AL:
					case AINC|VB:
						opReg[opidx++] = CPU_REGUL(rn);
						if (rn < REG_nPC) {
							rqReg[rqCount++] = (-OPM_SIZE(opr) << 4) | rn;
							gpReg[rn].l     += OPM_SIZE(opr);
						} else
							readvi(OPM_SIZE(opr));
						continue;

					case AINC|WQ: case AINC|AQ:
						opReg[opidx++] = CPU_REGUL(rn);
						if (rn < REG_nPC) {
							rqReg[rqCount++] = (-OPM_SIZE(opr) << 4) | rn;
							gpReg[rn].l     += OPM_SIZE(opr);
						} else {
							readvi(LN_LONG);
							readvi(LN_LONG);
						}
						continue;

					case AINC|WO: case AINC|AO:
						opReg[opidx++] = CPU_REGUL(rn);
						if (rn < REG_nPC) {
							rqReg[rqCount++] = (-OPM_SIZE(opr) << 4) | rn;
							gpReg[rn].l     += OPM_SIZE(opr);
						} else {
							readvi(LN_LONG);
							readvi(LN_LONG);
							readvi(LN_LONG);
							readvi(LN_LONG);
						}
						continue;

					// Autoincrement Deferred Address Mode
					case AINCD|RB: case AINCD|RW: case AINCD|RL: case AINCD|RF:
						if (rn < REG_nPC) {
							mAddr            = readv(CPU_REGUL(rn), LN_LONG, RACC);
							rqReg[rqCount++] = (-LN_LONG << 4) | rn;
							gpReg[rn].l     += LN_LONG;
						} else
							mAddr = readvi(LN_LONG);
						opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), RACC);
						continue;

					case AINCD|RQ: case AINCD|RD: case AINCD|RG:
						if (rn < REG_nPC) {
							mAddr            = readv(CPU_REGUL(rn), LN_LONG, RACC);
							rqReg[rqCount++] = (-LN_LONG << 4) | rn;
							gpReg[rn].l     += LN_LONG;
						} else
							mAddr = readvi(LN_LONG);
						opReg[opidx++] = readv(mAddr,   LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+4, LN_LONG, RACC);
						continue;

					case AINCD|RO: case AINCD|RH:
						if (rn < REG_nPC) {
							mAddr            = readv(CPU_REGUL(rn), LN_LONG, RACC);
							rqReg[rqCount++] = (-LN_LONG << 4) | rn;
							gpReg[rn].l     += LN_LONG;
						} else
							mAddr = readvi(LN_LONG);
						opReg[opidx++] = readv(mAddr,    LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+4,  LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+8,  LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+12, LN_LONG, RACC);
						continue;

					case AINCD|MB: case AINCD|MW: case AINCD|ML:
						if (rn < REG_nPC) {
							mAddr            = readv(CPU_REGUL(rn), LN_LONG, RACC);
							rqReg[rqCount++] = (-LN_LONG << 4) | rn;
							gpReg[rn].l     += LN_LONG;
						} else
							mAddr = readvi(LN_LONG);
						opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), WACC);
						opReg[opidx++] = mAddr;
						continue;

					case AINCD|MQ:
						if (rn < REG_nPC) {
							mAddr            = readv(CPU_REGUL(rn), LN_LONG, RACC);
							rqReg[rqCount++] = (-LN_LONG << 4) | rn;
							gpReg[rn].l     += LN_LONG;
						} else
							mAddr = readvi(LN_LONG);
						opReg[opidx++] = readv(mAddr,   LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+4, LN_LONG, WACC);
						opReg[opidx++] = mAddr;
						continue;

					case AINCD|MO:
						if (rn < REG_nPC) {
							mAddr            = readv(CPU_REGUL(rn), LN_LONG, RACC);
							rqReg[rqCount++] = (-LN_LONG << 4) | rn;
							gpReg[rn].l     += LN_LONG;
						} else
							mAddr = readvi(LN_LONG);
						opReg[opidx++] = readv(mAddr,    LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+4,  LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+8,  LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+12, LN_LONG, WACC);
						opReg[opidx++] = mAddr;
						continue;

					case AINCD|WB: case AINCD|WW: case AINCD|WL: case AINCD|WQ:
					case AINCD|AB: case AINCD|AW: case AINCD|AL: case AINCD|AQ:
					case AINCD|WO: case AINCD|AO: case AINCD|VB:
						if (rn < REG_nPC) {
							opReg[opidx++]   = readv(CPU_REGUL(rn), LN_LONG, RACC);
							rqReg[rqCount++] = (-LN_LONG << 4) | rn;
							gpReg[rn].l     += LN_LONG;
						} else
							opReg[opidx++] = readvi(LN_LONG);
						continue;


					// Byte-Displacement Address Mode
					case BDP|RB: case BDP|RW: case BDP|RL: case BDP|RF:
						off   = SXTB(readvi(LN_BYTE));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), RACC);
						continue;
					case BDP|RQ: case BDP|RG: case BDP|RD:
						off   = SXTB(readvi(LN_BYTE));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = readv(mAddr,   LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+4, LN_LONG, RACC);
						continue;
					case BDP|RO: case BDP|RH:
						off   = SXTB(readvi(LN_BYTE));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = readv(mAddr,    LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+4,  LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+8,  LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+12, LN_LONG, RACC);
						continue;

					case BDP|MB: case BDP|MW: case BDP|ML:
						off   = SXTB(readvi(LN_BYTE));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), WACC);
						opReg[opidx++] = mAddr;
						continue;
					case BDP|MQ:
						off   = SXTB(readvi(LN_BYTE));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = readv(mAddr,   LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+4, LN_LONG, WACC);
						opReg[opidx++] = mAddr;
						continue;
					case BDP|MO:
						off   = SXTB(readvi(LN_BYTE));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = readv(mAddr,    LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+4,  LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+8,  LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+12, LN_LONG, WACC);
						opReg[opidx++] = mAddr;
						continue;

					case BDP|WB: case BDP|WW: case BDP|WL: case BDP|WQ:
					case BDP|AB: case BDP|AW: case BDP|AL: case BDP|AQ:
					case BDP|WO: case BDP|AO: case BDP|VB:
						off   = SXTB(readvi(LN_BYTE));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = mAddr;
		//				PrintLog3(LOG_TRACE, NULL,
		//					"%s: (BDP) R%d => %08X + %08X => %08X\n",
		//						IO_DEVNAME(cpu), rn, CPU_REGUL(rn), off, mAddr);
						continue;

					// Byte-Displacement Deferred Address Mode
					case BDPD|RB: case BDPD|RW: case BDPD|RL: case BDPD|RF:
						off   = SXTB(readvi(LN_BYTE));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), RACC);
						continue;
					case BDPD|RQ: case BDPD|RG: case BDPD|RD:
						off   = SXTB(readvi(LN_BYTE));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr,   LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+4, LN_LONG, RACC);
						continue;
					case BDPD|RO: case BDPD|RH:
						off   = SXTB(readvi(LN_BYTE));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr,    LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+4,  LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+8,  LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+12, LN_LONG, RACC);
						continue;

					case BDPD|MB: case BDPD|MW: case BDPD|ML:
						off   = SXTB(readvi(LN_BYTE));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), WACC);
						opReg[opidx++] = mAddr;
						continue;
					case BDPD|MQ:
						off   = SXTB(readvi(LN_BYTE));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr,   LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+4, LN_LONG, WACC);
						opReg[opidx++] = mAddr;
						continue;
					case BDPD|MO:
						off   = SXTB(readvi(LN_BYTE));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr,    LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+4,  LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+8,  LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+12, LN_LONG, WACC);
						opReg[opidx++] = mAddr;
						continue;

					case BDPD|WB: case BDPD|WW: case BDPD|WL: case BDPD|WQ:
					case BDPD|AB: case BDPD|AW: case BDPD|AL: case BDPD|AQ:
					case BDPD|WO: case BDPD|AO: case BDPD|VB:
						off   = SXTB(readvi(LN_BYTE));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = mAddr;
						continue;

					// Word-Displacement Address Mode
					case WDP|RB: case WDP|RW: case WDP|RL: case WDP|RF:
						off   = SXTW(readvi(LN_WORD));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), RACC);
						continue;
					case WDP|RQ: case WDP|RG: case WDP|RD:
						off   = SXTW(readvi(LN_WORD));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = readv(mAddr,   LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+4, LN_LONG, RACC);
						continue;
					case WDP|RO: case WDP|RH:
						off   = SXTW(readvi(LN_WORD));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = readv(mAddr,    LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+4,  LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+8,  LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+12, LN_LONG, RACC);
						continue;

					case WDP|MB: case WDP|MW: case WDP|ML:
						off   = SXTW(readvi(LN_WORD));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), WACC);
						opReg[opidx++] = mAddr;
						continue;
					case WDP|MQ:
						off   = SXTW(readvi(LN_WORD));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = readv(mAddr,   LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+4, LN_LONG, WACC);
						opReg[opidx++] = mAddr;
						continue;
					case WDP|MO:
						off   = SXTW(readvi(LN_WORD));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = readv(mAddr,    LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+4,  LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+8,  LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+12, LN_LONG, WACC);
						opReg[opidx++] = mAddr;
						continue;

					case WDP|WB: case WDP|WW: case WDP|WL: case WDP|WQ:
					case WDP|AB: case WDP|AW: case WDP|AL: case WDP|AQ:
					case WDP|WO: case WDP|AO: case WDP|VB:
						off   = SXTW(readvi(LN_WORD));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = mAddr;
						continue;

					// Word-Displacement Deferred Address Mode
					case WDPD|RB: case WDPD|RW: case WDPD|RL: case WDPD|RF:
						off   = SXTW(readvi(LN_WORD));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), RACC);
						continue;
					case WDPD|RQ: case WDPD|RG: case WDPD|RD:
						off   = SXTW(readvi(LN_WORD));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr,   LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+4, LN_LONG, RACC);
						continue;
					case WDPD|RO: case WDPD|RH:
						off   = SXTW(readvi(LN_WORD));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr,    LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+4,  LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+8,  LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+12, LN_LONG, RACC);
						continue;

					case WDPD|MB: case WDPD|MW: case WDPD|ML:
						off   = SXTW(readvi(LN_WORD));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), WACC);
						opReg[opidx++] = mAddr;
						continue;
					case WDPD|MQ:
						off   = SXTW(readvi(LN_WORD));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr,   LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+4, LN_LONG, WACC);
						opReg[opidx++] = mAddr;
						continue;
					case WDPD|MO:
						off   = SXTW(readvi(LN_WORD));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr,    LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+4,  LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+8,  LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+12, LN_LONG, WACC);
						opReg[opidx++] = mAddr;
						continue;

					case WDPD|WB: case WDPD|WW: case WDPD|WL: case WDPD|WQ:
					case WDPD|AB: case WDPD|AW: case WDPD|AL: case WDPD|AQ:
					case WDPD|WO: case WDPD|AO: case WDPD|VB:
						off   = SXTW(readvi(LN_WORD));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = mAddr;
						continue;

					// Longword-Displacement Address Mode
					case LDP|RB: case LDP|RW: case LDP|RL: case LDP|RF:
						off   = SXTL(readvi(LN_LONG));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), RACC);
						continue;
					case LDP|RQ: case LDP|RG: case LDP|RD:
						off   = SXTL(readvi(LN_LONG));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = readv(mAddr,   LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+4, LN_LONG, RACC);
						continue;
					case LDP|RO: case LDP|RH:
						off   = SXTL(readvi(LN_LONG));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = readv(mAddr,    LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+4,  LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+8,  LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+12, LN_LONG, RACC);
						continue;

					case LDP|MB: case LDP|MW: case LDP|ML:
						off   = SXTL(readvi(LN_LONG));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), WACC);
						opReg[opidx++] = mAddr;
						continue;
					case LDP|MQ:
						off   = SXTL(readvi(LN_LONG));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = readv(mAddr,   LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+4, LN_LONG, WACC);
						opReg[opidx++] = mAddr;
						continue;
					case LDP|MO:
						off   = SXTL(readvi(LN_LONG));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = readv(mAddr,    LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+4,  LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+8,  LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+12, LN_LONG, WACC);
						opReg[opidx++] = mAddr;
						continue;

					case LDP|WB: case LDP|WW: case LDP|WL: case LDP|WQ:
					case LDP|AB: case LDP|AW: case LDP|AL: case LDP|AQ:
					case LDP|WO: case LDP|AO: case LDP|VB:
						off   = SXTL(readvi(LN_LONG));
						mAddr = CPU_REGUL(rn) + off;
						opReg[opidx++] = mAddr;
						continue;

					// Longword-Displacement Deferred Address Mode
					case LDPD|RB: case LDPD|RW: case LDPD|RL: case LDPD|RF:
						off   = SXTL(readvi(LN_LONG));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), RACC);
						continue;
					case LDPD|RQ: case LDPD|RG: case LDPD|RD:
						off   = SXTL(readvi(LN_LONG));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr,   LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+4, LN_LONG, RACC);
						continue;
					case LDPD|RO: case LDPD|RH:
						off   = SXTL(readvi(LN_LONG));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr,    LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+4,  LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+8,  LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr+12, LN_LONG, RACC);
						continue;

					case LDPD|MB: case LDPD|MW: case LDPD|ML:
						off   = SXTL(readvi(LN_LONG));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), WACC);
						opReg[opidx++] = mAddr;
						continue;
					case LDPD|MQ:
						off   = SXTL(readvi(LN_LONG));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr,   LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+4, LN_LONG, WACC);
						opReg[opidx++] = mAddr;
						continue;
					case LDPD|MO:
						off   = SXTL(readvi(LN_LONG));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = readv(mAddr,    LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+4,  LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+8,  LN_LONG, WACC);
						opReg[opidx++] = readv(mAddr+12, LN_LONG, WACC);
						opReg[opidx++] = mAddr;
						continue;

					case LDPD|WB: case LDPD|WW: case LDPD|WL: case LDPD|WQ:
					case LDPD|AB: case LDPD|AW: case LDPD|AL: case LDPD|AQ:
					case LDPD|WO: case LDPD|AO: case LDPD|VB:
						off   = SXTL(readvi(LN_LONG));
						mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						opReg[opidx++] = mAddr;
						continue;

					// Indexed Register
					case IDX|RB: case IDX|RW: case IDX|RL: case IDX|RQ: case IDX|RO:
					case IDX|MB: case IDX|MW: case IDX|ML: case IDX|MQ: case IDX|MO:
					case IDX|WB: case IDX|WW: case IDX|WL: case IDX|WQ: case IDX|WO:
					case IDX|AB: case IDX|AW: case IDX|AL: case IDX|AQ: case IDX|AO:
					case IDX|RF: case IDX|RD: case IDX|RG: case IDX|RH: case IDX|VB:
						Validate(rn, REG_nPC);
						ireg = CPU_REGUL(rn) << OPM_SCALE(opr);
						spec = readvi(LN_BYTE);
						rn   = spec & 0xF;
						break;

					default:
						throw RSVD_ADDR_FAULT;
				}

		#ifdef DEBUG
		//		PrintLog3(LOG_TRACE, NULL,
		//			"%s: (OPR) Index %08X (Operand %02X)\n",
		//				IO_DEVNAME(cpu), idxReg, spec);
		#endif /* DEBUG */

				// Indexed Register
				switch(spec & 0xF0) {
					case REGD:
						Validate(rn, REG_nPC);
						ireg += CPU_REGUL(rn);
						break;

					case ADEC:
						Validate(rn, REG_nPC);
						rqReg[rqCount++] = (OPM_SIZE(opr) << 4) | rn;
						gpReg[rn].l     -= OPM_SIZE(opr);
						ireg            += CPU_REGUL(rn);
						break;

					case AINC:
						Validate(rn, REG_nPC);
						ireg            += CPU_REGUL(rn);
						rqReg[rqCount++] = (-OPM_SIZE(opr) << 4) | rn;
						gpReg[rn].l     += OPM_SIZE(opr);
						break;

					case AINCD:
						if (rn < REG_nPC) {
							ireg            += readv(CPU_REGUL(rn), LN_LONG, RACC);
							rqReg[rqCount++] = (-LN_LONG << 4) | rn;
							gpReg[rn].l     += LN_LONG;
						} else
							ireg += readvi(LN_LONG);
						break;

					case BDP:
						off     = SXTB(readvi(LN_BYTE));
						ireg += CPU_REGUL(rn) + off;
						break;

					case BDPD:
						off     = SXTB(readvi(LN_BYTE));
						ireg += readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						break;

					case WDP:
						off     = SXTW(readvi(LN_WORD));
						ireg += CPU_REGUL(rn) + off;
						break;

					case WDPD:
						off     = SXTW(readvi(LN_WORD));
						ireg += readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						break;

					case LDP:
						off     = SXTL(readvi(LN_LONG));
						ireg += CPU_REGUL(rn) + off;
						break;

					case LDPD:
						off     = SXTL(readvi(LN_LONG));
						ireg += readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
						break;

					default:
						throw RSVD_ADDR_FAULT;
				}

				switch(opr & (OPM_ACC|OPM_LEN)) {
					case RB: case RW: case RL:
						opReg[opidx++] = readv(ireg, OPM_SIZE(opr), RACC);
						break;
					case RQ:
						opReg[opidx++] = readv(ireg,   LN_LONG, RACC);
						opReg[opidx++] = readv(ireg+4, LN_LONG, RACC);
						break;
					case RO:
						opReg[opidx++] = readv(ireg,    LN_LONG, RACC);
						opReg[opidx++] = readv(ireg+4,  LN_LONG, RACC);
						opReg[opidx++] = readv(ireg+8,  LN_LONG, RACC);
						opReg[opidx++] = readv(ireg+12, LN_LONG, RACC);
						break;

					case MB: case MW: case ML:
						opReg[opidx++] = readv(ireg, OPM_SIZE(opr), WACC);
						opReg[opidx++] = ireg;
						break;
					case MQ:
						opReg[opidx++] = readv(ireg,   LN_LONG, WACC);
						opReg[opidx++] = readv(ireg+4, LN_LONG, WACC);
						opReg[opidx++] = ireg;
						break;
					case MO:
						opReg[opidx++] = readv(ireg,    LN_LONG, WACC);
						opReg[opidx++] = readv(ireg+4,  LN_LONG, WACC);
						opReg[opidx++] = readv(ireg+8,  LN_LONG, WACC);
						opReg[opidx++] = readv(ireg+12, LN_LONG, WACC);
						opReg[opidx++] = ireg;
						break;

					case WB: case WW: case WL: case WQ: case WO:
					case AB: case AW: case AL: case AQ: case AO:
						opReg[opidx++] = ireg;
						break;
				}
			}

			// Execute opcode
			switch (opCode) {

			case OPC_nHALT:
				// Must be in kernel mode
				if (PSL_GETCUR(psReg) != AM_KERNEL)
					throw PRIV_INST_FAULT;
				throw STOP_HALT;

			case OPC_nNOP:
				// Do nothing...
				break;

			case OPC_nREI:
				resume();
				break;

			case OPC_nBPT:
				// Reset PC address
				REG_PC = faultAddr;
				exception(IE_EXC, SCB_BPT, 0);
				break;

			case OPC_nXFC:
				// Reset PC address
				REG_PC = faultAddr;
				exception(IE_EXC, SCB_XFC, 0);
				break;

			// INDEX - Index instruction
			case OPC_nINDEX:
				src  = SXTL(opReg[0]);
				src1 = SXTL(opReg[3]);
				src2 = SXTL(opReg[4]);
				if ((src < SXTL(opReg[1])) || (src > SXTL(opReg[2]))) {
					irqFlags &= ~IRQ_TRAP;
					irqFlags |= IRQ_SETTRAP(TRAP_SUBRNG);
				}

				dst = (src + src2) * src1;
				storel(opReg[5], dst);
				UpdateCC_IIZZ_L(ccReg, dst);
				printf("%s: %08X (%08X to %08X) + %08X * %08X => %08X: %s\n", devName.c_str(),
					ZXTL(src), ZXTL(opReg[1]), ZXTL(opReg[2]), ZXTL(src2), ZXTL(src1),
					ZXTL(dst), stringCC(ccReg));
				break;

			// BIxPSW instructions
			case OPC_nBICPSW:
				mask = uint16_t(opReg[0]);
				if (mask & PSW_MBZ)
					throw RSVD_OPND_FAULT;
				psReg &= ~mask;
				ccReg &= ~mask;
				break;
			case OPC_nBISPSW:
				mask = uint16_t(opReg[0]);
				if (mask & PSW_MBZ)
					throw RSVD_OPND_FAULT;
				psReg |= (mask & ~PSW_CC);
				ccReg |= (mask & PSW_CC);
				break;

			// MOVPSL instruction
			case OPC_nMOVPSL:
				dst = psReg | ccReg;
				storel(opReg[0], dst);
				printf("%s: PSL %08X: %s\n", devName.c_str(),
						ZXTL(dst), stringCC(ccReg));
				break;

			// MTPR/MFPR instructions
			case OPC_nMTPR:
				// Must be kernel mode
				if (PSL_GETCUR(psReg) != AM_KERNEL)
					throw PRIV_INST_FAULT;
				src = opReg[0];
				writepr(opReg[1], src);
				UpdateCC_IIZP_L(ccReg, src);
				break;
			case OPC_nMFPR:
				// Must be kernel mode
				if (PSL_GETCUR(psReg) != AM_KERNEL)
					throw PRIV_INST_FAULT;
				src = readpr(opReg[0]);
				storel(opReg[1], src);
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
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBEQL:
				if (ccReg & CC_Z) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBGTR:
				if ((ccReg & (CC_N|CC_Z)) == 0) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBLEQ:
				if (ccReg & (CC_N|CC_Z)) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBGEQ:
				if ((ccReg & CC_N) == 0) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBLSS:
				if (ccReg & CC_N) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBGTRU:
				if ((ccReg & (CC_C|CC_Z)) == 0) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBLEQU:
				if (ccReg & (CC_C|CC_Z)) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBVC:
				if ((ccReg & CC_V) == 0) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBVS:
				if (ccReg & CC_V) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBCC:
				if ((ccReg & CC_C) == 0) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBCS:
				if (ccReg & CC_C) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;


			//  BRx instructions
			case OPC_nBRB:
				REG_PC += opReg[0];
				flushvi();
				break;
			case OPC_nBRW:
				REG_PC += opReg[0];
				flushvi();
				break;

			// JMP instruction
			case OPC_nJMP:
				REG_PC = opReg[0];
				flushvi();
				break;


			// BSBx instructions
			case OPC_nBSBB:
				writev(REG_SP - LN_LONG, REG_PC, LN_LONG, WACC);
				REG_SP -= LN_LONG;
				REG_PC += opReg[0];
				flushvi();
				break;
			case OPC_nBSBW:
				writev(REG_SP - LN_LONG, REG_PC, LN_LONG, WACC);
				REG_SP -= LN_LONG;
				REG_PC += opReg[0];
				flushvi();
				break;

			// JSB instruction
			case OPC_nJSB:
				writev(REG_SP - LN_LONG, REG_PC, LN_LONG, WACC);
				REG_SP -= LN_LONG;
				REG_PC = opReg[0];
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
				src   = SXTB(opReg[0]);
				src1  = SXTB(opReg[1]);
				src2  = SXTB(opReg[2]);
				dst   = src2 + src1;
				storeb(opReg[3], dst);
				UpdateCC_IIZP_B(ccReg, dst);
				SetV_ADD(ccReg, dst, src2, src1, SGN_BYTE);
				if ((src1 < 0) ? (dst >= src) : (dst <= src)) {
					REG_PC += opReg[4];
					flushvi();
				}
				printf("%s: %02X + %02X => %02X: %s\n", devName.c_str(),
					ZXTB(src2), ZXTB(src1), ZXTB(dst), stringCC(ccReg));
				printf("%s: %02X %s %02X: %s\n", devName.c_str(),
					ZXTB(dst), (src1 < 0) ? ">=" : "<=", ZXTB(src),
					((src1 < 0) ? (dst >= src) : (dst <= src)) ? "Jumped" : "Continue");
				break;
			case OPC_nACBW:
				src   = SXTW(opReg[0]);
				src1  = SXTW(opReg[1]);
				src2  = SXTW(opReg[2]);
				dst   = src2 + src1;
				storew(opReg[3], dst);
				UpdateCC_IIZP_W(ccReg, dst);
				SetV_ADD(ccReg, dst, src2, src1, SGN_WORD);
				if ((src1 < 0) ? (dst >= src) : (dst <= src)) {
					REG_PC += opReg[4];
					flushvi();
				}
				printf("%s: %04X + %04X => %04X: %s\n", devName.c_str(),
					ZXTW(src2), ZXTW(src1), ZXTW(dst), stringCC(ccReg));
				printf("%s: %04X %s %04X: %s\n", devName.c_str(),
					ZXTW(dst), (src1 < 0) ? ">=" : "<=", ZXTW(src),
					((src1 < 0) ? (dst >= src) : (dst <= src)) ? "Jumped" : "Continue");
				break;
			case OPC_nACBL:
				src   = SXTL(opReg[0]);
				src1  = SXTL(opReg[1]);
				src2  = SXTL(opReg[2]);
				dst   = src2 + src1;
				storel(opReg[3], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				SetV_ADD(ccReg, dst, src2, src1, SGN_LONG);
				if ((src1 < 0) ? (dst >= src) : (dst <= src)) {
					REG_PC += opReg[4];
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
				src1 = SXTB(opReg[0]);
				src2 = SXTB(opReg[1]);
				src  = SXTB(opReg[2]);
				dst  = src1 - src2;
				UpdateCC_CMP_B(ccReg, dst, src);
				if (ZXTB(dst) <= ZXTB(src)) {
					brDisp = readv(REG_PC + (ZXTB(dst) << 1), LN_WORD, RACC);
					REG_PC += SXTW(brDisp);
				} else
					REG_PC += ((ZXTB(src) << 1) + 2);
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
				src1 = SXTW(opReg[0]);
				src2 = SXTW(opReg[1]);
				src  = SXTW(opReg[2]);
				dst  = src1 - src2;
				UpdateCC_CMP_W(ccReg, dst, src);
				if (ZXTW(dst) <= ZXTW(src)) {
					brDisp = readv(REG_PC + (ZXTW(dst) << 1), LN_WORD, RACC);
					REG_PC += SXTW(brDisp);
				} else
					REG_PC += ((ZXTW(src) << 1) + 2);
				flushvi();
				printf("%s: (%04X - %04X) => %04X <= %04X\n", devName.c_str(),
					ZXTW(src1), ZXTW(src2), ZXTW(dst), ZXTW(src));
				break;
			case OPC_nCASEL:
				src1 = SXTL(opReg[0]);
				src2 = SXTL(opReg[1]);
				src  = SXTL(opReg[2]);
				dst  = src1 - src2;
				UpdateCC_CMP_L(ccReg, dst, src);
				if (ZXTL(dst) <= ZXTL(src)) {
					brDisp = readv(REG_PC + (ZXTL(dst) << 1), LN_WORD, RACC);
					REG_PC += SXTW(brDisp);
				} else
					REG_PC += ((ZXTL(src) << 1) + 2);
				flushvi();
				printf("%s: (%08X - %08X) => %08X <= %08X\n", devName.c_str(),
					ZXTL(src1), ZXTL(src2), ZXTL(dst), ZXTL(src));
				break;

			// AOBcc instructions
			case OPC_nAOBLEQ:
				src1 = SXTL(opReg[0]);
				src2 = SXTL(opReg[1]);
				dst  = src2 + 1;
				storel(opReg[2], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				SetV_ADD(ccReg, dst, src2, 1, SGN_LONG);
				if (dst <= src1) {
					REG_PC += opReg[3];;
					flushvi();
				}
				printf("%s: %08X + 1 => %08X <= %08X: %s\n", devName.c_str(),
						ZXTL(src2), ZXTL(dst), ZXTL(src1), stringCC(ccReg));
				if (dst <= src1)
					printf("%s: Jump into PC %08X\n", devName.c_str(), REG_PC);
				break;
			case OPC_nAOBLSS:
				src1 = SXTL(opReg[0]);
				src2 = SXTL(opReg[1]);
				dst  = src2 + 1;
				storel(opReg[2], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				SetV_ADD(ccReg, dst, src2, 1, SGN_LONG);
				if (dst < src1) {
					REG_PC += opReg[3];
					flushvi();
				}
				printf("%s: %08X + 1 => %08X < %08X: %s\n", devName.c_str(),
						ZXTL(src2), ZXTL(dst), ZXTL(src1), stringCC(ccReg));
				if (dst < src1)
					printf("%s: Jump into PC %08X\n", devName.c_str(), REG_PC);
				break;

			// SOBcc instructions
			case OPC_nSOBGEQ:
				src = SXTL(opReg[0]);
				dst = src - 1;
				storel(opReg[1], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				SetV_SUB(ccReg, dst, src, 1, SGN_LONG);
				if (dst >= 0) {
					REG_PC += opReg[2];
					flushvi();
				}
				printf("%s: %08X - 1 => %08X >= 0: %s\n", devName.c_str(),
						ZXTL(src), ZXTL(dst), stringCC(ccReg));
				if (dst >= 0)
					printf("%s: Jump into PC %08X\n", devName.c_str(), REG_PC);
				break;
			case OPC_nSOBGTR:
				src = SXTL(opReg[0]);
				dst = src - 1;
				storel(opReg[1], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				SetV_SUB(ccReg, dst, src, 1, SGN_LONG);
				if (dst > 0) {
					REG_PC += opReg[2];
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
					REG_PC += opReg[2];
					flushvi();
				}
				break;
			case OPC_nBBC:
				if (getBit() == 0) {
					REG_PC += opReg[2];
					flushvi();
				}
				break;
			case OPC_nBBSSI:
			case OPC_nBBSS:
				if (setBit(1) == 1) {
					REG_PC += opReg[2];
					flushvi();
				}
				break;
			case OPC_nBBSC:
				if (setBit(0) == 1) {
					REG_PC += opReg[2];
					flushvi();
				}
				break;
			case OPC_nBBCS:
				if (setBit(1) == 0) {
					REG_PC += opReg[2];
					flushvi();
				}
				break;
			case OPC_nBBCCI:
			case OPC_nBBCC:
				if (setBit(0) == 0) {
					REG_PC += opReg[2];
					flushvi();
				}
				break;

			// BLBx instructions
			case OPC_nBLBS:
				src = opReg[0];
				if ((src & 1) == 1) {
					REG_PC += opReg[1];
					flushvi();
				}
				printf("%s: %08X & 1 => %d\n", devName.c_str(), src, src & 1);
				break;
			case OPC_nBLBC:
				src = opReg[0];
				if ((src & 1) == 0) {
					REG_PC += opReg[1];
					flushvi();
				}
				printf("%s: %08X & 1 => %d\n", devName.c_str(), src, src & 1);
				break;

			// MOVx instructions
			// MOVAx instructions
			case OPC_nMOVB:
				dst = opReg[0];
				storeb(opReg[1], dst);
				UpdateCC_IIZP_B(ccReg, dst);
				printf("%s: Move %02X: %s\n", devName.c_str(),
						ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nMOVW:
				dst = opReg[0];
				storew(opReg[1], dst);
				UpdateCC_IIZP_W(ccReg, dst);
				printf("%s: Move %04X: %s\n", devName.c_str(),
						ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nMOVL:
			case OPC_nMOVAB:
			case OPC_nMOVAW:
			case OPC_nMOVAL:
			case OPC_nMOVAQ:
				dst = opReg[0];
				storel(opReg[1], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				printf("%s: Move %08X: %s\n", devName.c_str(),
						ZXTL(dst), stringCC(ccReg));
				break;
			case OPC_nMOVQ:
				dst1 = opReg[0];
				dst2 = opReg[1];
				storeq(opReg[2], dst1, dst2);
				UpdateCC_IIZP_Q(ccReg, dst1, dst2);
				printf("%s: Move %08X %08X: %s\n", devName.c_str(),
						ZXTL(dst1), ZXTL(dst2), stringCC(ccReg));
				break;

			// MCOMx - Move complemented instructions
			case OPC_nMCOMB:
				src = SXTB(opReg[0]);
				dst = ~src;
				storeb(opReg[1], dst);
				UpdateCC_IIZP_B(ccReg, dst);
				printf("%s: Move ~%02X => %02X: %s\n", devName.c_str(),
					ZXTB(src), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nMCOMW:
				src = SXTW(opReg[0]);
				dst = ~src;
				storew(opReg[1], dst);
				UpdateCC_IIZP_W(ccReg, dst);
				printf("%s: Move ~%04X => %04X: %s\n", devName.c_str(),
					ZXTW(src), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nMCOML:
				src = SXTL(opReg[0]);
				dst = ~src;
				storel(opReg[1], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				printf("%s: Move ~%08X => %08X: %s\n", devName.c_str(),
					ZXTL(src), ZXTL(dst), stringCC(ccReg));
				break;

			// MNEGx - Move negated instructions
			case OPC_nMNEGB:
				src = SXTB(opReg[0]);
				dst = -src;
				storeb(opReg[1], dst);
				SetCC_SUB_B(ccReg, dst, 0, src);
				printf("%s: Move -%02X => %02X: %s\n", devName.c_str(),
					ZXTB(src), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nMNEGW:
				src = SXTW(opReg[0]);
				dst = -src;
				storew(opReg[1], dst);
				SetCC_SUB_W(ccReg, dst, 0, src);
				printf("%s: Move -%04X => %04X: %s\n", devName.c_str(),
					ZXTW(src), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nMNEGL:
				src = SXTL(opReg[0]);
				dst = -src;
				storel(opReg[1], dst);
				SetCC_SUB_L(ccReg, dst, 0, src);
				printf("%s: Move -%08X => %08X: %s\n", devName.c_str(),
					ZXTL(src), ZXTL(dst), stringCC(ccReg));
				break;

			// MOVZx instructions
			case OPC_nMOVZBW:
				dst = ZXTB(opReg[0]);
				storew(opReg[1], dst);
				UpdateCC_IIZP_W(ccReg, dst);
				printf("%s: Move %02X => %04X: %s\n", devName.c_str(),
					ZXTB(dst), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nMOVZBL:
				dst = ZXTB(opReg[0]);
				storel(opReg[1], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				printf("%s: Move %02X => %08X: %s\n", devName.c_str(),
					ZXTB(dst), ZXTL(dst), stringCC(ccReg));
				break;
			case OPC_nMOVZWL:
				dst = ZXTW(opReg[0]);
				storel(opReg[1], dst);
				UpdateCC_IIZP_L(ccReg, dst);
				printf("%s: Move %04X => %08X: %s\n", devName.c_str(),
					ZXTW(dst), ZXTL(dst), stringCC(ccReg));
				break;

			// CVTx - Convert instructions
			case OPC_nCVTBW:
				src = SXTB(opReg[0]);
				dst = SXTW(src);
				storew(opReg[1], dst);
				UpdateCC_IIZZ_W(ccReg, dst);
				printf("%s: %02X => %04X: %s\n", devName.c_str(),
						ZXTB(src), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nCVTBL:
				src = SXTB(opReg[0]);
				dst = SXTL(src);
				storel(opReg[1], dst);
				UpdateCC_IIZZ_L(ccReg, dst);
				printf("%s: %02X => %08X: %s\n", devName.c_str(),
						ZXTB(src), ZXTL(dst), stringCC(ccReg));
				break;
			case OPC_nCVTWB:
				src = SXTW(opReg[0]);
				dst = SXTB(src);
				storeb(opReg[1], dst);
				UpdateCC_IIZZ_B(ccReg, dst);
				if (src < -128 || src > 127) {
					ccReg |= CC_V;
				}
				printf("%s: %04X => %02X: %s\n", devName.c_str(),
						ZXTW(src), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nCVTWL:
				src = SXTW(opReg[0]);
				dst = SXTL(src);
				storel(opReg[1], dst);
				UpdateCC_IIZZ_L(ccReg, dst);
				printf("%s: %04X => %08X: %s\n", devName.c_str(),
						ZXTW(src), ZXTL(dst), stringCC(ccReg));
				break;
			case OPC_nCVTLB:
				src = SXTL(opReg[0]);
				dst = SXTB(src);
				storeb(opReg[1], dst);
				UpdateCC_IIZZ_B(ccReg, dst);
				if (src < -128 || src > 127) {
					ccReg |= CC_V;
				}
				printf("%s: %08X => %02X: %s\n", devName.c_str(),
						ZXTL(src), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nCVTLW:
				src = SXTL(opReg[0]);
				dst = SXTW(src);
				storew(opReg[1], dst);
				UpdateCC_IIZZ_W(ccReg, dst);
				if (src < -128 || src > 127) {
					ccReg |= CC_V;
				}
				printf("%s: %08X => %04X: %s\n", devName.c_str(),
						ZXTL(src), ZXTW(dst), stringCC(ccReg));
				break;

			// CLRx instructions
			case OPC_nCLRB:
				storeb(opReg[0], 0);
				SetZ(ccReg);
				break;
			case OPC_nCLRW:
				storew(opReg[0], 0);
				SetZ(ccReg);
				break;
			case OPC_nCLRL:
				storel(opReg[0], 0);
				SetZ(ccReg);
				break;
			case OPC_nCLRQ:
				storeq(opReg[0], 0, 0);
				SetZ(ccReg);
				break;

			// CMPx instructions
			case OPC_nCMPB:
				src1 = SXTB(opReg[0]);
				src2 = SXTB(opReg[1]);
				UpdateCC_CMP_B(ccReg, src1, src2);
				printf("%s: Compare %02X with %02X: %s\n", devName.c_str(),
						ZXTB(src1), ZXTB(src2), stringCC(ccReg));
				break;
			case OPC_nCMPW:
				src1 = SXTW(opReg[0]);
				src2 = SXTW(opReg[1]);
				UpdateCC_CMP_W(ccReg, src1, src2);
				printf("%s: Compare %04X with %04X: %s\n", devName.c_str(),
						ZXTW(src1), ZXTW(src2), stringCC(ccReg));
				break;
			case OPC_nCMPL:
				src1 = SXTL(opReg[0]);
				src2 = SXTL(opReg[1]);
				UpdateCC_CMP_L(ccReg, src1, src2);
				printf("%s: Compare %08X with %08X: %s\n", devName.c_str(),
						ZXTL(src1), ZXTL(src2), stringCC(ccReg));
				break;

				// TSTx instructions
			case OPC_nTSTB:
				src = SXTB(opReg[0]);
				UpdateCC_IIZZ_B(ccReg, src);
				printf("%s: Test %02X: %s\n", devName.c_str(),
						ZXTB(src), stringCC(ccReg));
				break;
			case OPC_nTSTW:
				src = SXTW(opReg[0]);
				UpdateCC_IIZZ_W(ccReg, src);
				printf("%s: Test %04X: %s\n", devName.c_str(),
						ZXTW(src), stringCC(ccReg));
				break;
			case OPC_nTSTL:
				src = SXTL(opReg[0]);
				UpdateCC_IIZZ_L(ccReg, src);
				printf("%s: Test %08X: %s\n", devName.c_str(),
						ZXTL(src), stringCC(ccReg));
				break;

			// INCx instructions
			case OPC_nINCB:
				src = SXTB(opReg[0]);
				dst = src + 1;
				storeb(opReg[1], dst);
				SetCC_ADD_B(ccReg, dst, src, 1);
				printf("%s: %02X + 1 => %02X: %s\n",
					devName.c_str(), ZXTB(src), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nINCW:
				src = SXTW(opReg[0]);
				dst = src + 1;
				storew(opReg[1], dst);
				SetCC_ADD_W(ccReg, dst, src, 1);
				printf("%s: %04X + 1 => %04X: %s\n",
					devName.c_str(), ZXTW(src), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nINCL:
				src = SXTL(opReg[0]);
				dst = src + 1;
				storel(opReg[1], dst);
				SetCC_ADD_L(ccReg, dst, src, 1);
				printf("%s: %08X + 1 => %08X: %s\n",
					devName.c_str(), ZXTL(src), ZXTL(dst), stringCC(ccReg));
				break;

			// DECx instructions
			case OPC_nDECB:
				src = SXTB(opReg[0]);
				dst = src - 1;
				storeb(opReg[1], dst);
				SetCC_SUB_B(ccReg, dst, src, 1);
				printf("%s: %08X - 1 => %08X: %s\n",
					devName.c_str(), ZXTB(src), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nDECW:
				src = SXTW(opReg[0]);
				dst = src - 1;
				storew(opReg[1], dst);
				SetCC_SUB_W(ccReg, dst, src, 1);
				printf("%s: %04X - 1 => %04X: %s\n",
					devName.c_str(), ZXTW(src), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nDECL:
				src = SXTL(opReg[0]);
				dst = src - 1;
				storel(opReg[1], dst);
				SetCC_SUB_L(ccReg, dst, src, 1);
				printf("%s: %08X - 1 => %08X: %s\n",
					devName.c_str(), ZXTL(src), ZXTL(dst), stringCC(ccReg));
				break;

			// ADAWI instruction
			case OPC_nADAWI:
				src = SXTW(opReg[0]);
				if (OP_ISREG(opReg[1])) {
					tmp = SXTW(gpReg[~opReg[1]].l);
					dst = src + tmp;
					gpReg[~opReg[1]].l = ZXTW(dst);
				} else {
					if (opReg[1] & 1)
						throw RSVD_OPND_FAULT;
					tmp = SXTW(readv(opReg[1], LN_WORD, RACC));
					dst = src + tmp;
					writev(opReg[1], dst, LN_WORD, WACC);
				}
				SetCC_ADD_W(ccReg, dst, src, tmp);
				printf("%s: %04X + %04X => %04X: %s\n", devName.c_str(),
						ZXTW(src), ZXTW(tmp), ZXTW(dst), stringCC(ccReg));
				break;

			// ADDx instructions
			case OPC_nADDB2:
			case OPC_nADDB3:
				src1 = SXTB(opReg[0]);
				src2 = SXTB(opReg[1]);
				dst  = src2 + src1;
				storeb(opReg[2], dst);
				SetCC_ADD_B(ccReg, dst, src2, src1);
				printf("%s: %02X + %02X => %02X: %s\n", devName.c_str(),
					ZXTB(src1), ZXTB(src2), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nADDW2:
			case OPC_nADDW3:
				src1 = SXTW(opReg[0]);
				src2 = SXTW(opReg[1]);
				dst  = src2 + src1;
				storew(opReg[2], dst);
				SetCC_ADD_W(ccReg, dst, src2, src1);
				printf("%s: %04X + %04X => %04X: %s\n", devName.c_str(),
					ZXTW(src1), ZXTW(src2), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nADDL2:
			case OPC_nADDL3:
				src1 = SXTL(opReg[0]);
				src2 = SXTL(opReg[1]);
				dst  = src2 + src1;
				storel(opReg[2], dst);
				SetCC_ADD_L(ccReg, dst, src2, src1);
				printf("%s: %08X + %08X => %08X: %s\n", devName.c_str(),
					ZXTL(src1), ZXTL(src2), ZXTL(dst), stringCC(ccReg));
				break;

			// SUBx instructions
			case OPC_nSUBB2:
			case OPC_nSUBB3:
				src1 = SXTB(opReg[0]);
				src2 = SXTB(opReg[1]);
				dst  = src2 - src1;
				storeb(opReg[2], dst);
				SetCC_SUB_B(ccReg, dst, src2, src1);
				printf("%s: %02X - %02X => %02X: %s\n", devName.c_str(),
					ZXTB(src1), ZXTB(src2), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nSUBW2:
			case OPC_nSUBW3:
				src1 = SXTW(opReg[0]);
				src2 = SXTW(opReg[1]);
				dst  = src2 - src1;
				storew(opReg[2], dst);
				SetCC_SUB_W(ccReg, dst, src2, src1);
				printf("%s: %04X - %04X => %04X: %s\n", devName.c_str(),
					ZXTW(src1), ZXTW(src2), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nSUBL2:
			case OPC_nSUBL3:
				src1 = SXTL(opReg[0]);
				src2 = SXTL(opReg[1]);
				dst  = src2 - src1;
				storel(opReg[2], dst);
				SetCC_SUB_L(ccReg, dst, src2, src1);
				printf("%s: %08X - %08X => %08X: %s\n", devName.c_str(),
					ZXTL(src1), ZXTL(src2), ZXTL(dst), stringCC(ccReg));
				break;

			// MULx - Multiply instructions
			case OPC_nMULB2:
			case OPC_nMULB3:
				src1 = SXTB(opReg[0]);
				src2 = SXTB(opReg[1]);
				dst  = src2 * src1;
				storeb(opReg[2], dst);
				UpdateCC_IIZZ_B(ccReg, dst);
				if (dst < SCHAR_MIN || dst > SCHAR_MAX)
					ccReg |= CC_V;
				printf("%s: %02X * %02X => %02X: %s\n", devName.c_str(),
					ZXTB(src2), ZXTB(src1), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nMULW2:
			case OPC_nMULW3:
				src1 = SXTW(opReg[0]);
				src2 = SXTW(opReg[1]);
				dst  = src2 * src1;
				storew(opReg[2], dst);
				UpdateCC_IIZZ_W(ccReg, dst);
				if (dst < SHRT_MIN || dst > SHRT_MAX)
					ccReg |= CC_V;
				printf("%s: %02X * %02X => %02X: %s\n", devName.c_str(),
					ZXTW(src2), ZXTW(src1), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nMULL2:
			case OPC_nMULL3:
				srcq1 = SXTL(opReg[0]);
				srcq2 = SXTL(opReg[1]);
				dstq  = srcq2 * srcq1;
				storel(opReg[2], SXTL(dstq));
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
				src1 = SXTB(opReg[0]);
				src2 = SXTB(opReg[1]);
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
				storeb(opReg[2], dst);
				UpdateCC_IIZZ_B(ccReg, dst);
				if (ovflg)
					ccReg |= CC_V;
				printf("%s: %02X / %02X => %02X: %s\n", devName.c_str(),
					ZXTB(src2), ZXTB(src1), ZXTB(dst), stringCC(ccReg));
				break;
			case OPC_nDIVW2:
			case OPC_nDIVW3:
				src1 = SXTW(opReg[0]);
				src2 = SXTW(opReg[1]);
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
				storew(opReg[2], dst);
				UpdateCC_IIZZ_W(ccReg, dst);
				if (ovflg)
					ccReg |= CC_V;
				printf("%s: %04X / %04X => %04X: %s\n", devName.c_str(),
					ZXTW(src2), ZXTW(src1), ZXTW(dst), stringCC(ccReg));
				break;
			case OPC_nDIVL2:
			case OPC_nDIVL3:
				src1 = SXTL(opReg[0]);
				src2 = SXTL(opReg[1]);
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
				storel(opReg[2], dst);
				UpdateCC_IIZZ_L(ccReg, dst);
				if (ovflg)
					ccReg |= CC_V;
				printf("%s: %08X / %08X => %08X: %s\n", devName.c_str(),
					ZXTL(src2), ZXTL(src1), ZXTL(dst), stringCC(ccReg));
				break;

			// EMUL - Extended multiply instruction
			case OPC_nEMUL:
				srcq1 = SXTL(opReg[0]);
				srcq2 = SXTL(opReg[1]);
				srcq  = SXTL(opReg[2]);
				dstq  = (srcq2 * srcq1) + srcq;
				storeq(opReg[3], ZXTL(dstq), ZXTL(dstq >> 32));
				UpdateCC_IIZZ_64(ccReg, dstq);
				printf("%s: (%08X * %08X) + %08X => %08X %08X: %s\n", devName.c_str(),
					ZXTL(srcq2), ZXTL(srcq1), ZXTL(srcq), ZXTL(dstq >> 32), ZXTL(dstq),
					stringCC(ccReg));
				break;
			// EDIV - Extended divide instruction
			case OPC_nEDIV:
				srcq1 = SXTL(opReg[0]);
				srcq2 = (ZXTQ(opReg[2]) << 32) | ZXTQ(opReg[1]);
				ovflg = false;
				if ((srcq1 == 0) || (srcq1 == -1LL && srcq2 == LLONG_MIN)) {
					ovflg = true;
				} else {
//					printf("%s: %08X >= %08X\n", devName.c_str(),
//						abs(srcq2 >> 32), abs(srcq1));
					if (abs(srcq2 >> 32) >= abs(srcq1) && ZXTL(srcq1) != SGN_LONG)
						ovflg = true;
					else {
						dstq1 = srcq2 / srcq1;
						dstq2 = srcq2 % srcq1;
						if ((dstq1 < LONG_MIN) || (dstq1 > LONG_MAX))
							ovflg = true;
					}
				}

				if (ovflg == true) {
					dstq1 = srcq2;
					dstq2 = 0;
				}

				storel(opReg[3], dstq1);
				storel(opReg[4], dstq2);
				UpdateCC_IIZZ_L(ccReg, dstq1);
				ccReg |= ovflg ? CC_V : 0;
				printf("%s: %08X %08X / %08X => %08X R %08X: %s\n", devName.c_str(),
					ZXTL(srcq2 >> 32), ZXTL(srcq2), ZXTL(srcq1), ZXTL(dstq1), ZXTL(dstq2),
					stringCC(ccReg));
				break;

			// ADWC/SBWC instructions
			case OPC_nADWC:
				src1  = SXTL(opReg[0]);
				src2  = SXTL(opReg[1]);
				carry = ccReg & CC_C;
				dst   = src2 + src1 + carry;
				storel(opReg[2], dst);
				SetCC_ADD_L(ccReg, dst, src2, src1);
				if ((dst == src2) && src1 != 0)
					ccReg |= CC_C;
				printf("%s: %08X + %08X + %d => %08X: %s\n", devName.c_str(),
					ZXTL(src1), ZXTL(src2), carry, ZXTL(dst), stringCC(ccReg));
				break;
			case OPC_nSBWC:
				src1  = SXTL(opReg[0]);
				src2  = SXTL(opReg[1]);
				carry = ccReg & CC_C;
				dst   = src2 - src1 - carry;
				storel(opReg[2], dst);
				SetCC_SUB_L(ccReg, dst, src2, src1);
				if ((src1 == src2) && dst != 0)
					ccReg |= CC_C;
				printf("%s: %08X - %08X - %d => %08X: %s\n", devName.c_str(),
					ZXTL(src1), ZXTL(src2), carry, ZXTL(dst), stringCC(ccReg));
				break;

			// BICx instructions
			case OPC_nBICB2:
			case OPC_nBICB3:
				mask = ZXTB(opReg[0]);
				usrc = ZXTB(opReg[1]);
				udst = usrc & ~mask;
				storeb(opReg[2], udst);
				UpdateCC_IIZP_B(ccReg, udst);
				printf("%s: %02X & ~%02X => %02X\n", devName.c_str(),
						ZXTB(usrc), ZXTB(mask), ZXTB(udst), stringCC(ccReg));
				break;
			case OPC_nBICW2:
			case OPC_nBICW3:
				mask = ZXTW(opReg[0]);
				usrc = ZXTW(opReg[1]);
				udst = usrc & ~mask;
				storew(opReg[2], udst);
				UpdateCC_IIZP_W(ccReg, udst);
				printf("%s: %04X & ~%04X => %04X: %s\n", devName.c_str(),
						ZXTW(usrc), ZXTW(mask), ZXTW(udst), stringCC(ccReg));
				break;
			case OPC_nBICL2:
			case OPC_nBICL3:
				mask = ZXTL(opReg[0]);
				usrc = ZXTL(opReg[1]);
				udst = usrc & ~mask;
				storel(opReg[2], udst);
				UpdateCC_IIZP_L(ccReg, udst);
				printf("%s: %08X & ~%08X => %08X: %s\n", devName.c_str(),
						ZXTL(usrc), ZXTL(mask), ZXTL(udst), stringCC(ccReg));
				break;

			// BISx instructions
			case OPC_nBISB2:
			case OPC_nBISB3:
				mask = ZXTB(opReg[0]);
				usrc = ZXTB(opReg[1]);
				udst = usrc | mask;
				storeb(opReg[2], udst);
				UpdateCC_IIZP_B(ccReg, udst);
				printf("%s: %02X | %02X => %02X: %s\n", devName.c_str(),
						ZXTB(usrc), ZXTB(mask), ZXTB(udst), stringCC(ccReg));
				break;
			case OPC_nBISW2:
			case OPC_nBISW3:
				mask = ZXTW(opReg[0]);
				usrc = ZXTW(opReg[1]);
				udst = usrc | mask;
				storew(opReg[2], udst);
				UpdateCC_IIZP_W(ccReg, udst);
				printf("%s: %04X | %04X => %04X: %s\n", devName.c_str(),
						ZXTW(usrc), ZXTW(mask), ZXTW(udst), stringCC(ccReg));
				break;
			case OPC_nBISL2:
			case OPC_nBISL3:
				mask = ZXTL(opReg[0]);
				usrc = ZXTL(opReg[1]);
				udst = usrc | mask;
				storel(opReg[2], udst);
				UpdateCC_IIZP_L(ccReg, udst);
				printf("%s: %08X | %08X => %08X: %s\n", devName.c_str(),
						ZXTL(usrc), ZXTL(mask), ZXTL(udst), stringCC(ccReg));
				break;

			// BITx instructions
			case OPC_nBITB:
				mask = ZXTB(opReg[0]);
				usrc = ZXTB(opReg[1]);
				udst = usrc & mask;
				UpdateCC_IIZP_B(ccReg, udst);
				printf("%s: %02X & %02X => %02X: %s\n", devName.c_str(),
						ZXTB(usrc), ZXTB(mask), ZXTB(udst), stringCC(ccReg));
				break;
			case OPC_nBITW:
				mask = ZXTW(opReg[0]);
				usrc = ZXTW(opReg[1]);
				udst = usrc & mask;
				UpdateCC_IIZP_W(ccReg, udst);
				printf("%s: %04X & %04X => %04X: %s\n", devName.c_str(),
						ZXTW(usrc), ZXTW(mask), ZXTW(udst), stringCC(ccReg));
				break;
			case OPC_nBITL:
				mask = ZXTL(opReg[0]);
				usrc = ZXTL(opReg[1]);
				udst = usrc & mask;
				UpdateCC_IIZP_L(ccReg, udst);
				printf("%s: %08X & %08X => %08X: %s\n", devName.c_str(),
						ZXTL(usrc), ZXTL(mask), ZXTL(udst), stringCC(ccReg));
				break;

			// XORx instructions
			case OPC_nXORB2:
			case OPC_nXORB3:
				mask = ZXTB(opReg[0]);
				usrc = ZXTB(opReg[1]);
				udst = usrc ^ mask;
				storeb(opReg[2], udst);
				UpdateCC_IIZP_B(ccReg, udst);
				printf("%s: %02X ^ %02X => %02X: %s\n", devName.c_str(),
						ZXTB(usrc), ZXTB(mask), ZXTB(udst), stringCC(ccReg));
				break;
			case OPC_nXORW2:
			case OPC_nXORW3:
				mask = ZXTW(opReg[0]);
				usrc = ZXTW(opReg[1]);
				udst = usrc ^ mask;
				storew(opReg[2], udst);
				UpdateCC_IIZP_W(ccReg, udst);
				printf("%s: %04X ^ %04X => %04X: %s\n", devName.c_str(),
						ZXTW(usrc), ZXTW(mask), ZXTW(udst), stringCC(ccReg));
				break;
			case OPC_nXORL2:
			case OPC_nXORL3:
				mask = ZXTL(opReg[0]);
				usrc = ZXTL(opReg[1]);
				udst = usrc ^ mask;
				storel(opReg[2], udst);
				UpdateCC_IIZP_L(ccReg, udst);
				printf("%s: %08X ^ %08X => %08X: %s\n", devName.c_str(),
						ZXTL(usrc), ZXTL(mask), ZXTL(udst), stringCC(ccReg));
				break;

			// ASHx/ROTL - Shift instructions
			case OPC_nASHL:
				cnt   = SXTB(opReg[0]);
				src   = SXTL(opReg[1]);
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
				storel(opReg[2], dst);
				UpdateCC_IIZZ_L(ccReg, dst);
				if (ovflg)
					ccReg |= CC_V;
				printf("%s: %08X %s %d => %08X: %s\n", devName.c_str(),
					ZXTL(src), ((cnt < 0) ? ">>" : "<<"), abs(cnt),
					ZXTL(dst), stringCC(ccReg));
				break;

			case OPC_nASHQ:
				cnt    = SXTB(opReg[0]);
				srcq  = (ZXTQ(opReg[2]) << 32) | ZXTL(opReg[1]);
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
				storeq(opReg[3], SXTL(dstq), SXTL(dstq >> 32));
				UpdateCC_IIZZ_64(ccReg, dstq);
				if (ovflg)
					ccReg |= CC_V;
				printf("%s: %08X %08X %s %d => %08X %08X: %s\n", devName.c_str(),
					ZXTL(srcq >> 32), ZXTL(srcq), ((cnt < 0) ? ">>" : "<<"), abs(cnt),
					ZXTL(dstq >> 32), ZXTL(dstq), stringCC(ccReg));
				break;

			case OPC_nROTL:
				cnt  = ZXTB(opReg[0]) % 32;
				usrc = ZXTL(opReg[1]);
				udst = (cnt != 0) ? ((usrc << cnt) | (usrc >> (32 - cnt))) : usrc;
				storel(opReg[2], udst);
				UpdateCC_IIZP_L(ccReg, udst);
				printf("%s: %08X %s %d => %08X: %s\n", devName.c_str(),
					ZXTL(usrc), ((cnt < 0) ? ">>" : "<<"), abs(cnt),
					ZXTL(udst), stringCC(ccReg));
				break;

			// CMPV - Compare field instructions
			case OPC_nCMPV:
				dst = getField(true);
				src = SXTL(opReg[3]);
				UpdateCC_CMP_L(ccReg, dst, src);
				printf("%s: Compare %08X with %08X: %s\n", devName.c_str(),
					ZXTL(dst), ZXTL(src), stringCC(ccReg));
				break;
			case OPC_nCMPZV:
				dst = getField(false);
				src = SXTL(opReg[3]);
				UpdateCC_CMP_L(ccReg, dst, src);
				printf("%s: Compare %08X with %08X: %s\n", devName.c_str(),
					ZXTL(dst), ZXTL(src), stringCC(ccReg));
				break;

			// EXTV - Extract field instructions
			case OPC_nEXTV:
				src = getField(true);
				storel(opReg[3], src);
				UpdateCC_IIZP_L(ccReg, src);
				printf("%s: Extract %08X: %s\n", devName.c_str(),
					ZXTL(src), stringCC(ccReg));
				break;
			case OPC_nEXTZV:
				src = getField(false);
				storel(opReg[3], src);
				UpdateCC_IIZP_L(ccReg, src);
				printf("%s: Extract %08X: %s\n", devName.c_str(),
					ZXTL(src), stringCC(ccReg));
				break;

			// FFS/FFC - First find instruction
			case OPC_nFFS:
				usrc1 = ZXTL(opReg[0]);
				usrc2 = ZXTB(opReg[1]);
				if (usrc2 > 0) {
					int idx;
					usrc = getField(false);
					for (idx = 0; idx < usrc2; idx++)
						if ((usrc >> idx) & 1)
							break;
					udst = usrc1 + idx;
				} else {
					usrc = 0;
					udst = usrc1;
				}
				storel(opReg[3], udst);
				ccReg = usrc != 0 ? 0 : CC_Z;
				printf("%s: Find %08X<%d:%d> => %d: %s\n", devName.c_str(),
					ZXTL(usrc), ZXTL(usrc1), ZXTB(usrc2), udst,
					stringCC(ccReg));
				break;
			case OPC_nFFC:
				usrc1 = ZXTL(opReg[0]);
				usrc2 = ZXTB(opReg[1]);
				if (usrc2 > 0) {
					int idx;
					usrc = getField(false) ^ mskList[usrc2];
					printf("%s: %08X\n", devName.c_str(), usrc);
					for (idx = 0; idx < usrc2; idx++)
						if ((usrc >> idx) & 1)
							break;
					udst = usrc1 + idx;
				} else {
					usrc = 0;
					udst = usrc1;
				}
				storel(opReg[3], udst);
				ccReg = usrc != 0 ? 0 : CC_Z;
				printf("%s: Find %08X<%d:%d> => %d: %s\n", devName.c_str(),
					ZXTL(usrc), ZXTL(usrc1), ZXTB(usrc2), udst,
					stringCC(ccReg));
				break;

			// INSV - Insert field instruction
			case OPC_nINSV:
				putField();
				break;

			// ******************
			// Stack instructions
			// ******************

			// PUSHx/PUSHAx - Push instruction
			case OPC_nPUSHL:
			case OPC_nPUSHAB:
			case OPC_nPUSHAW:
			case OPC_nPUSHAL:
			case OPC_nPUSHAQ:
				src = opReg[0];
				writev(REG_SP - LN_LONG, src, LN_LONG, WACC);
				REG_SP -= LN_LONG;
				UpdateCC_IIZP_L(ccReg, src);
				printf("%s: Push %08X to SP (%08X): %s\n", devName.c_str(),
						src, REG_SP + LN_LONG, stringCC(ccReg));
				break;

			// PUSHR/POPR - Push/pop register instructions
			case OPC_nPUSHR:
				mask = SXTL(opReg[0]) & STK_MASK;
				// Check access for page faults

				// Push registers into stack
				for (int idx = REG_nSP; idx >= REG_nR0; idx--) {
					if ((mask >> idx) & 1) {
						writev(REG_SP - LN_LONG, gpReg[idx].l, LN_LONG, WACC);
						printf("%s: R%d %08X => (SP) %08X\n", devName.c_str(),
							idx, ZXTL(gpReg[idx].l), ZXTL(REG_SP));
						REG_SP -= LN_LONG;
					}
				}
				break;
			case OPC_nPOPR:
				mask = SXTL(opReg[0]) & STK_MASK;
				// Check access for page faults

				// Push registers into stack
				for (int idx = REG_nR0; idx <= REG_nSP; idx++) {
					if ((mask >> idx) & 1) {
						gpReg[idx].l = readv(REG_SP, LN_LONG, RACC);
						printf("%s: R%d %08X <= (SP) %08X\n", devName.c_str(),
							idx, ZXTL(gpReg[idx].l), ZXTL(REG_SP));
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

			// Floating instructions
			case OPC_nCVTFD:
			case OPC_nCVTLF:
			case OPC_nCVTLD:
			case OPC_nCVTLG:
				throw RSVD_INST_FAULT;

			// Queue instructions
			case OPC_nINSQUE:
				entry = SXTL(opReg[0]);
				pred  = SXTL(opReg[1]);
				// Check write access first for page faults
				succ = readv(pred, LN_LONG, WACC);
				readv(succ+LN_LONG, LN_LONG, WACC);
				readv(entry+LN_LONG, LN_LONG, WACC);
				// Insert entry into queue
				writev(entry, succ, LN_LONG, WACC);
				writev(entry+LN_LONG, pred, LN_LONG, WACC);
				writev(succ+LN_LONG, entry, LN_LONG, WACC);
				writev(pred, entry, LN_LONG, WACC);
				// Update condition codes
				UpdateCC_CMP_L(ccReg, succ, pred);
				printf("%s: Insert %08X to %08X with next %08X: %s\n",
					devName.c_str(), ZXTL(entry), ZXTL(pred), ZXTL(succ),
					stringCC(ccReg));
				break;
			case OPC_nINSQHI:
				entry = SXTL(opReg[0]);
				hdr   = SXTL(opReg[1]);
				if ((entry == hdr) || ((entry|hdr) & 07))
					throw RSVD_OPND_FAULT;
				// Check write access for page faults
				readv(entry, LN_LONG, WACC);
				a = readv(hdr, LN_LONG, WACC);
			OPC_INSQHI:
				if (a & 06)
					throw RSVD_OPND_FAULT;
				if (a & 01)
					ccReg = CC_C;
				else {
					writev(hdr, a | 1, LN_LONG, WACC);
					a += hdr;
					writev(hdr, a - hdr, LN_LONG, WACC);
					writev(a+LN_LONG, entry - a, LN_LONG, WACC);
					writev(entry, a - entry, LN_LONG, WACC);
					writev(entry+LN_LONG, hdr-entry, LN_LONG, WACC);
					writev(hdr, entry-hdr, LN_LONG, WACC);
					ccReg = (a == hdr) ? CC_Z : 0;
				}
				break;
			case OPC_nINSQTI:
				entry = SXTL(opReg[0]);
				hdr   = SXTL(opReg[1]);
				if ((entry == hdr) || ((entry|hdr) & 07))
					throw RSVD_OPND_FAULT;
				// Check write access for page faults
				readv(entry, LN_LONG, WACC);
				a = readv(hdr, LN_LONG, WACC);
				if (a == 0)
					goto OPC_INSQHI;
				if (a & 06)
					throw RSVD_OPND_FAULT;
				if (a & 01) {
					// Busy signal...
					ccReg = CC_C;
				} else {
					writev(hdr, a | 1, LN_LONG, WACC);
					c = readv(hdr+LN_LONG, LN_LONG, RACC) + hdr;
					if (c & 07) {
						writev(hdr, a, LN_LONG, WACC);
						throw RSVD_OPND_FAULT;
					}
					writev(hdr, a, LN_LONG, WACC);
					writev(c, entry-c, LN_LONG, WACC);
					writev(entry, hdr-entry, LN_LONG, WACC);
					writev(entry+LN_LONG, c-entry, LN_LONG, WACC);
					writev(hdr+LN_LONG, entry-hdr, LN_LONG, WACC);
					writev(hdr, a, LN_LONG, WACC);
					// Clear all condition codes
					ccReg = 0;
				}
				break;
			case OPC_nREMQUE:
				entry = SXTL(opReg[0]);
				succ = readv(entry, LN_LONG, RACC);
				pred = readv(entry+LN_LONG, LN_LONG, RACC);
				// Update condition codes first
				UpdateCC_CMP_L(ccReg, succ, pred);
				if (entry != pred) {
					// Check write access for page faults
					readv(succ+LN_LONG, LN_LONG, WACC);
					if (SXTL(opReg[1]) != OPR_MEM)
						readv(opReg[2], LN_LONG, WACC);
					// Remove entry from queue
					writev(pred, succ, LN_LONG, WACC);
					writev(succ+LN_LONG, pred, LN_LONG, WACC);
				} else
					ccReg |= CC_V;
				storel(opReg[1], entry);
				printf("%s: Remove %08X from %08X with next %08X: %s\n",
					devName.c_str(), ZXTL(entry), ZXTL(pred), ZXTL(succ),
					stringCC(ccReg));
				break;
			case OPC_nREMQHI:
				hdr = SXTL(opReg[0]);
				if (hdr & 07)
					throw RSVD_OPND_FAULT;
				if (!OP_ISREG(opReg[1])) {
					if (opReg[1] == hdr)
						throw RSVD_OPND_FAULT;
					readv(opReg[1], LN_LONG, WACC);
				}
				ar = readv(hdr, LN_LONG, WACC);
			OPC_REMQHI:
				if (ar & 06)
					throw RSVD_OPND_FAULT;
				if (ar & 01)
					ccReg = CC_V|CC_C;
				else {
					a = ar + hdr;
					if (ar != 0) {
						writev(hdr, ar|1, LN_LONG, WACC);
//						writev(hdr, ar, LN_LONG, WACC);
						b = readv(a, LN_LONG, RACC) + a;
						if (b & 07) {
							writev(hdr, ar, LN_LONG, WACC);
							throw RSVD_OPND_FAULT;
						}
//						writev(hdr, ar, LN_LONG, WACC);
						writev(b+LN_LONG, hdr-b, LN_LONG, WACC);
						writev(hdr, b-hdr, LN_LONG, WACC);
					}
					storel(opReg[1], a);
					// Update condition codes
					if (ar == 0)
						ccReg = CC_Z|CC_V;
					else if (b == hdr)
						ccReg = CC_Z;
					else
						ccReg = 0;
				}
				break;
			case OPC_nREMQTI:
				hdr = SXTL(opReg[0]);
				if (hdr & 07)
					throw RSVD_OPND_FAULT;
				if (!OP_ISREG(opReg[1])) {
					if (opReg[1] == hdr)
						throw RSVD_OPND_FAULT;
					readv(opReg[1], LN_LONG, WACC);
				}
				ar = readv(hdr, LN_LONG, WACC);
				if (ar & 06)
					throw RSVD_OPND_FAULT;
				if (ar & 01)
					ccReg = CC_V|CC_C;
				else {
					if (ar != 0) {
						writev(hdr, ar|1, LN_LONG, WACC);
						c = readv(hdr+LN_LONG, LN_LONG, RACC);
						if (ar == c) {
							writev(hdr, ar, LN_LONG, WACC);
							goto OPC_REMQHI;
						}
						if (c & 07) {
							writev(hdr, ar, LN_LONG, WACC);
							throw RSVD_OPND_FAULT;
						}
						c += hdr;
//						writev(hdr, ar, LN_LONG, WACC);
						b = readv(c+LN_LONG, LN_LONG, RACC) + c;
						if (b & 07) {
							writev(hdr, ar, LN_LONG, WACC);
							throw RSVD_OPND_FAULT;
						}
//						writev(hdr, ar, LN_LONG, WACC);
						writev(b, hdr-b, LN_LONG, WACC);
						writev(hdr+LN_LONG, b-hdr, LN_LONG, WACC);
						writev(hdr, ar, LN_LONG, WACC);
					} else
						c = hdr;
					storel(opReg[1], c);
					// Update condition codes
					ccReg = (ar == 0) ? (CC_Z|CC_V) : 0;
				}
				break;

			// *******************
			// String instructions
			// *******************

			// MOVC3 - Move characters (3 registers)
			// MOVC5 - Move characters (5 registers)
			case OPC_nMOVC3:
				movc(0);
				break;
			case OPC_nMOVC5:
				movc(1);
				break;

			// CMPC3 - Compare characters (3 registers)
			// CMPC5 - Compare characters (5 registers)
			case OPC_nCMPC3:
				cmpc(0);
				break;
			case OPC_nCMPC5:
				cmpc(1);
				break;

			// LOCC - Locate character
			// SKPC - skip character
			case OPC_nLOCC:
				locc(false);
				break;
			case OPC_nSKPC:
				locc(true);
				break;

			// SCANC - scan character
			// SPANC - span character
			case OPC_nSCANC:
				scanc(false);
				break;
			case OPC_nSPANC:
				scanc(true);
				break;

			// ****************
			// CIS instructions
			// ****************

			case OPC_nMOVP:
			case OPC_nADDP4:  case OPC_nADDP6:
			case OPC_nSUBP4:  case OPC_nSUBP6:
			case OPC_nMULP6:  case OPC_nDIVP6:
			case OPC_nCMPP3:  case OPC_nCMPP4:
			case OPC_nCVTPL:  case OPC_nCVTLP:
			case OPC_nCVTPS:  case OPC_nCVTSP:
			case OPC_nCVTPT:  case OPC_nCVTTP:
			case OPC_nASHP:   case OPC_nCRC:
			case OPC_nMOVTC:  case OPC_nMOVTUC:
			case OPC_nMATCHC: case OPC_nEDITPC:
				emulate(opCode);
				break;

			// context switch instructions
			// LDPCTX - Load process
			// SVPCTX - Save process
			case OPC_nLDPCTX:
				ldpctx();
				break;
			case OPC_nSVPCTX:
				svpctx();
				break;

			// Illegal/unimplemented instruction
			default:
//				if (opc->opCode != OPC_nUOPC)
//					throw STOP_UOPC;
				throw RSVD_INST_FAULT;
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
			case RSVD_INST_FAULT:
				if (fault(SCB_RESIN))
					return;
				break;
			case RSVD_ADDR_FAULT:
				if (fault(SCB_RESAD))
					return;
				break;
			case RSVD_OPND_FAULT:
				if (fault(SCB_RESOP))
					return;
				break;
			case PRIV_INST_FAULT:
				if (fault(SCB_RESIN|SCB_NOPRIV))
					return;
				break;
			}
		}
	}
}
