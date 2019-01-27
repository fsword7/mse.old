/*
 * devauto.h - auto device configuration
 *
 *  Created on: Jan 27, 2019
 *      Author: Tim Stark
 */

#pragma once

class devauto_base
{
public:
	devauto_base(device_t &base, tag_t *tag);
	virtual ~devauto_base();

private:
	devauto_base	*next;
	device_t		&base;
	tag_t			*tag;
	bool			resolved;
};

template <class ObjectClass, bool required>
class object_autodev_base : public devauto_base
{
protected:
	object_autodev_base(device_t &base, tag_t *name)
		: devauto_base(base, tag) { }

private:
	ObjectClass *target = nullptr;
};

template <class ObjectClass>
using optional_device = object_autodev_base<ObjectClass, false>;

template <class ObjectClass>
using required_device = object_autodev_base<ObjectClass, true>;
