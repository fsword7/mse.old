/*
 * pdp10_exec.h
 *
 *  Created on: Feb 12, 2017
 *      Author: Timothy Stark
 */

#pragma once

#include "dev/cpu/pdp10/pdp10.h"
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

#ifndef CPU_CLASS
#define CPU_CLASS pdp10_cpuDevice
#endif

void CPU_CLASS::execute()
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
