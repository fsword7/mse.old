/*
 * pdp10_exec.h
 *
 *  Created on: Feb 12, 2017
 *      Author: Timothy Stark
 */

#pragma once

#include "devices/cpu/pdp10/pdp10_opc.h"
#include "devices/cpu/pdp10/pdp10.hpp"

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

void Processor_PDP10::execute()
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
