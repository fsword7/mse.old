/*
 * executes.h - PDP10 family processor - execute routine
 *
 *  Created on: Feb 12, 2017
 *      Author: Timothy Stark
 */

#pragma once

#include "emu/emucore.h"
#include "dev/cpu/pdp10/kx10.h"
#include "dev/cpu/pdp10/opcodes.h"

#if 0
// Operand flags for each instruction
uint32_t oprFlags[01000] =
{
		// UUO Instructions 000-077
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0


		// IOT instructions 700-777
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0
};
#endif

void pdp10_cpu_base::execute()
{
	uint64_t	irCode;
	int 		opCode;
	int			acCode;

	while(1) {
		// Fetch instruction from current PC address

		// Decode instruction address

		// Fetch operands from Memory

		// Execute instruction
		switch (opCode) {

		default: // UUO/MUUO - unimplemented opcode
//		MUUO:

		}

		// Save operands to Memory

		// Page fault exception
	}
}
