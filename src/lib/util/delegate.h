/*
 * delegate.h
 *
 *  Created on: Jan 8, 2019
 *      Author: Tim Stark
 */

#pragma once

using delegate_generic_func = void(*)();


class delegate_bind
{
public:
	delegate_bind() {}
	virtual ~delegate_bind() {}
};

template<typename Class, typename Return, typename... Params>
struct delegate_traits
{
	using static_ptr_func = Return (*)(Class *, Params...);
	using static_ref_func = Return (&)(Class *, Params...);
	using member_ptr_func = Return (Class::*)(Params...);
	using const_member_ptr_func = Return (Class::*)(Params...) const;
};

//class delegate_mfp
//{
//public:
//	delegate_mfp() : func(0), thisDelta(0) {}
//
//	template<typename MemberReturn, class MemberClass, typename Return, typename StaticFunc>
//	delegate_mfp(MemberFunc mfp, MemberClass *, Return *, StaticFunc)
//	{
//		*reinterpret_cast<MemberFunc *>(this) = mfp;
//	}
//
//	bool isnull() const { return (func == nullptr && thisDelta = 0); }
//
//private:
//	uintptr_t 	func;
//	int			thisDelta;
//};

template<typename Return, typename... Params>
class delegate_base
{
public:
	delegate_base();

protected:

};

template<typename Return, typename... Params>
class delegate : public delegate_base<Return, Params...>
{
private:
	using base = delegate_base<Return, Params...>;

public:
	delegate() : base() {};
};
