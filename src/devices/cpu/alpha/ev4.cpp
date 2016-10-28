
#include "emu/core.hpp"
#include "emu/devcpu.hpp"
#include "devices/cpu/alpha/axp.hpp"
#include "devices/cpu/alpha/ev4.hpp"

ev4_cpuDevice::ev4_cpuDevice()
{
}

ev4_cpuDevice::~ev4_cpuDevice()
{
}

void ev4_cpuDevice::execute()
{
	uint32_t inst, opc;
	uint32_t pal;



	while(true) {
		opc = OP_GETCD(inst);

		switch (opc) {
		case 0x00: // PALcode
			pal = OP_GETPAL(inst);
			break;
		default:   // Illegal instruction
			break;
		}
	}
}
