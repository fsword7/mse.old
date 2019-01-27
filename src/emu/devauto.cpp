/*
 * devauto.cpp - auto device configuration
 *
 *  Created on: Jan 27, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/device.h"
#include "emu/devauto.h"

devauto_base::devauto_base(device_t &_base, tag_t *name)
: next(base.register_device(this)),
  base(_base),
  tag(name),
  resolved(false)
{

}

devauto_base::~devauto_base()
{
}
