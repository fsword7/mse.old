/*
 * delegate.h
 *
 *  Created on: Jan 8, 2019
 *      Author: Tim Stark
 */

#pragma once

#include <functional>

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

template<typename ReturnType, typename... Params>
class delegate_base
{
public:
	template <class FunctionClass>
		using traits = delegate_traits<FunctionClass, ReturnType, Params...>;
	using functype = std::function<ReturnType (Params...)>;

	delegate_base();

	delegate_base(functype func)
		: stdfunc(func)
	{
	}

protected:
	functype stdfunc;	// std::funcion pointer
};

//template<typename Signature>
//class delegate;

template<typename Return, typename... Params>
class delegate : public delegate_base<Return, Params...>
{
private:
	using base = delegate_base<Return, Params...>;

protected:
//	template <class FunctionClass> using traits = typename base::template traits<FunctionClass>;
//	template <class FunciionClass> using member_func = typename traits<FunctionClass>::member_func;
//	template <class FunctionClass> using const_member_func = typename traits<FunctionClass>::const_member_func;
//	templete <class FunctionClass> using static_ref_func = typename traits<FunctionClass>::static_ref_func;

public:
	delegate() : base() {};

	delegate(const base &src) : base(src) {}
	delegate(const base &src, delegate_bind &obj) : base(src, obj) {}

	explicit delegate(std::function<Return (Params...)> func) : base(func) {}

//	template<class FunctionCall>
//	delegate(member_func<FunctionClass> func, FunctionClass *obj)
//		: base(func, obj) {}
//
//	template<class FunctionCall>
//	delegate(const_member_func<FunctionClass> func, FunctionClass *obj)
//		: base(func, obj) {}
//
//	template<class FunctionCall>
//	delegate(static_ref_func<FunctionClass> func, FunctionClass *obj)
//		: base(func, obj) {}


};
