/*
 * devsys2.h
 *
 *  Created on: Jan 9, 2019
 *      Author: Tim Stark
 */

#pragma once


using system_delegate = delegate<void ()>;

class system_device : public device_t
{
public:
	enum syscbType {
		sysStart,
		sysReset,
		sysCallbackCount
	};

	system_device(const system_config &config, const device_type_base &type, tag_t *tag, uint64_t clock);

	virtual ~system_device();

	virtual void init();

private:

	system_delegate cbFunc[sysCallbackCount];
};
