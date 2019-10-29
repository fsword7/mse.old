/*
 * executes.h - PDP10 family processor - execute routine
 *
 *  Created on: Feb 12, 2017
 *      Author: Timothy Stark
 */

#include "emu/emucore.h"
#include "dev/cpu/pdp10/kx10.h"
#include "opcode.h"

// Operand flags for each instruction
//uint32_t oprFlags[01000] =
//{
//		// UUO Instructions 000-077
//		0, 0, 0, 0,  0, 0, 0, 0,
//		0, 0, 0, 0,  0, 0, 0, 0,
//		0, 0, 0, 0,  0, 0, 0, 0,
//		0, 0, 0, 0,  0, 0, 0, 0,
//		0, 0, 0, 0,  0, 0, 0, 0,
//		0, 0, 0, 0,  0, 0, 0, 0,
//		0, 0, 0, 0,  0, 0, 0, 0,
//		0, 0, 0, 0,  0, 0, 0, 0
//
//
//		// IOT instructions 700-777
//		0, 0, 0, 0,  0, 0, 0, 0,
//		0, 0, 0, 0,  0, 0, 0, 0,
//		0, 0, 0, 0,  0, 0, 0, 0,
//		0, 0, 0, 0,  0, 0, 0, 0,
//		0, 0, 0, 0,  0, 0, 0, 0,
//		0, 0, 0, 0,  0, 0, 0, 0,
//		0, 0, 0, 0,  0, 0, 0, 0,
//		0, 0, 0, 0,  0, 0, 0, 0
//};


void pdp10_cpu_base::execute()
{
	int 		irCode;
	int			acCode;
	bool		pcInhibit = false;

	while(1) {
		// Fetch instruction from current PC address

		// Decode instruction address

		// Fetch operands from Memory
		abReg = pcReg;
		if (readv(0, 0))
			goto page_fault;

		// Decoding instruction code
		irCode = (mbReg >> 27) & 0777;
		acCode = (mbReg >> 23) & 017;

		// Decoding direct/indirect address
//		do {
//
//		}

		// Execute instruction
		switch (irCode) {


		// UUO - illegal unimplemented opcode
		muuo:
		defualt:

		// MUUO - monitor unimplemented opcode
		case 0000:
		case 0040: case 0041: case 0042: case 0043:
		case 0044: case 0045: case 0046: case 0047:
		case 0050: case 0051: case 0052: case 0053:
		case 0054: case 0055: case 0056: case 0057:
		case 0060: case 0061: case 0062: case 0063:
		case 0064: case 0065: case 0066: case 0067:
		case 0070: case 0071: case 0072: case 0073:
		case 0074: case 0075: case 0076: case 0077:
#if CPU_KI10|CPU_KL10
		case 0100: case 0101: case 0102: case 0103:
		case 0104: case 0105: case 0106: case 0107:
#endif
			break;

		// LUUO - local unimplemented opcode (user mode)
		case 0001: case 0002: case 0003:
		case 0004: case 0005: case 0006: case 0007:
		case 0010: case 0011: case 0012: case 0013:
		case 0014: case 0015: case 0016: case 0017:
		case 0020: case 0021: case 0022: case 0023:
		case 0024: case 0025: case 0026: case 0027:
		case 0030: case 0031: case 0032: case 0033:
		case 0034: case 0035: case 0036: case 0037:
			break;


		}

		// Save operands to Memory

		// Page fault exception
	page_fault:

		// All done, increment PC register if inhibit is not set
		if (pcInhibit == false)
			pcReg = (pcReg + 1) & H10_MASK;
		pcInhibit = false;
	}
}
