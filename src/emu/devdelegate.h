/*
 * devdelegate.h - delegate calls for devices
 *
 *  Created on: Jan 8, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "lib/util/delegate.h"


template<typename Signature>
class named_delegate : delegate<Signature>
{
private:
	using base = delegate<Signature>;

public:
	named_delegate() : base(), delegateName(nullptr) {}

	named_delegate(const base &src)
		: base(src), delegateName(src.name()) {}

	const char *name() const { return delegateName; }

private:
	const char *delegateName;
};

template<typename Signature>
class device_delegate : named_delegate<Signature>
{
private:
	using devbase = device_delegate<Signature>;
	using base    = named_delegate<Signature>;

public:
	device_delegate()
		: base(), deviceName(nullptr) {}
	device_delegate(const base &src)
		: base(src), deviceName(src.name()) {}
	device_delegate(const base &src, delegate_bind &obj)
		: base(src, obj), deviceName(src.name()) {}


	const char *name() const { return deviceName; }

private:
	const char *deviceName;
};
