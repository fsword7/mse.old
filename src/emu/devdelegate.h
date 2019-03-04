/*
 * devdelegate.h - delegate calls for devices
 *
 *  Created on: Jan 8, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "lib/util/delegate.h"

template<typename Signature>
class named_delegate : public delegate<Signature>
{
private:
	using base = delegate<Signature>;

protected:
	template<class FunctionClass> using member_ptr_func =
			typename base::template member_ptr_func<FunctionClass>;
	template<class FunctionClass> using const_member_ptr_func =
			typename base::template const_member_ptr_func<FunctionClass>;
	template<class FunctionClass> using static_ref_func =
			typename base::template static_ref_func<FunctionClass>;

public:
	named_delegate() : base(), delegateName(nullptr) {}

	named_delegate(const base &src)
		: base(src), delegateName(src.name()) {}
	template<class FunctionClass> named_delegate(member_ptr_func<FunctionClass> func, tag_t *name, FunctionClass *obj)
		: base(func, obj), delegateName(name) {}
	template<class FunctionClass> named_delegate(const_member_ptr_func<FunctionClass> func, tag_t *name, FunctionClass *obj)
		: base(func, obj), delegateName(name) {}
	template<class FunctionClass> named_delegate(static_ref_func<FunctionClass> func, tag_t *name, FunctionClass *obj)
		: base(func, obj), delegateName(name) {}

//	named_delegate &operator = (const base &src)
//	{
//		base::operator = (src);
//		delegateName = src.delegateName;
//		return *this;
//	}

	const char *name() const { return delegateName; }

private:
	const char *delegateName;
};

template<typename Signature>
class device_delegate : public named_delegate<Signature>
{
private:
	using devbase = device_delegate<Signature>;
	using base    = named_delegate<Signature>;

protected:
	template<class FunctionClass> using member_ptr_func =
			typename base::template member_ptr_func<FunctionClass>;
	template<class FunctionClass> using const_member_ptr_func =
			typename base::template const_member_ptr_func<FunctionClass>;
	template<class FunctionClass> using static_ref_func =
			typename base::template static_ref_func<FunctionClass>;

public:
	device_delegate()
		: base(), deviceName(nullptr) {}
	device_delegate(const base &src)
		: base(src), deviceName(src.name()) {}
	device_delegate(const base &src, delegate_bind &obj)
		: base(src, obj), deviceName(src.name()) {}

	template<class FunctionClass>
	device_delegate(member_ptr_func<FunctionClass> func, const char *name, const char *devName)
		: base(func, name), deviceName(devName) {}

	template<class FunctionClass>
	device_delegate(const_member_ptr_func<FunctionClass> func, const char *name, const char *devName)
		: base(func, name), deviceName(devName) {}

	template<class FunctionClass>
	device_delegate(static_ref_func<FunctionClass> func, const char *name, const char *devName)
		: base(func, name), deviceName(devName) {}

//	device_delegate(static_ref_func<device_t> func, const char *name)
//		: base(func, name), deviceName(devName) {}

	const char *name() const { return deviceName; }

private:
	const char *deviceName;
};

typedef device_delegate<int ()> read_delegate;
typedef device_delegate<void (int)> write_delegate;

template<typename T>
inline read_delegate make_delegate(T &&func, const char *name, const char *tag)
{
	return read_delegate(func, name, tag);
}

template<typename T>
inline write_delegate make_delegate(T &&func, const char *name, const char *tag)
{
	return write_delegate(func, name, tag);
}
