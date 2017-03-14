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

void CPU_CLASS::execute()
{
	uint16_t opCode;

	while (1) {
		// Fetch instruction from current stream
		opCode = 0;
//		if (opCode >= OPC_nEXTEND) {
//			opCode = (opCode << 8) | 0;
//		}

		// Decode operands

		// Execute opcode
		switch (opCode) {

		}
	}
}
