/*
 * pdp10_exec.h
 *
 *  Created on: Feb 12, 2017
 *      Author: Timothy Stark
 */

#pragma once

#include "devices/cpu/pdp10/pdp10_opc.h"
#include "devices/cpu/pdp10/pdp10.hpp"

void Processor_PDP10::execute()
{
	int opCode;

	while(1) {
		switch (opCode) {

		default: // UUO/MUUO - unimplemented opcode
//		MUUO:

		}
	}
}
