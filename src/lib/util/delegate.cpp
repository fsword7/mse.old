/*
 * delegate.cpp
 *
 *  Created on: Jan 8, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include <cstdint>
#include "lib/util/delegate.h"

delegate_generic_func delegate_mfp::convert_to_generic(delegate_generic_class *&obj) const
{
	obj = reinterpret_cast<delegate_generic_class *>(reinterpret_cast<uint8_t *>(obj) + thisDelta);

	if (!(func & 1))
		return reinterpret_cast<delegate_generic_func>(func);

	uint8_t *vtable_base = *reinterpret_cast<uint8_t **>(obj);

	return *reinterpret_cast<delegate_generic_func *>(vtable_base + func - 1);
}
