
#include "emu/emucore.h"
#include "dev/cpu/alpha/ev4.h"

ev4_cpu::ev4_cpu(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock)
: axp_cpu_base(config, tag, owner, clock)
{
}

ev4_cpu::~ev4_cpu()
{
}

void ev4_cpu::execute()
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
