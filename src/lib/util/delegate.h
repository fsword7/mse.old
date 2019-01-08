/*
 * delegate.h
 *
 *  Created on: Jan 8, 2019
 *      Author: Tim Stark
 */

#pragma once

#include <functional>

using delegate_generic_func = void(*)();

class delegate_generic_class;

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
	template <class FunctionClass>
		using traits = delegate_traits<FunctionClass, Return, Params...>;
	using generic_static_func = typename traits<delegate_generic_class>::static_ptr_func;
	using generic_member_func = generic_static_func;

	using functype = std::function<Return (Params...)>;

	delegate_base()
		: function(nullptr),
		  object(nullptr),
		  stdfunc(nullptr)
	{
	}

	delegate_base(functype func)
		: function(nullptr),
		  object(nullptr),
		  stdfunc(func)
	{
	}

//	template<class FunctionClass>
//	delegate_base(typename traits<FunctionClass>::member_ptr_func func, FunctionClass *obj)
//		:

//	template <class FunctionClass>
//	delegate_base(typename traits<FunctionClass>::static_ref_func func, FunctionClass *obj)
//		: function(reinterpret_cast<generic_static_func>(func)),
//		  object(nullptr),
//		  stdfunc(nullptr)
//	{
//	}

protected:
	delegate_generic_func	*object;
	generic_static_func		function;
	functype 				stdfunc;	// std::funcion pointer
};

template<typename Signature>
class delegate;

template<typename Return, typename... Params>
class delegate<Return (Params...)> : public delegate_base<Return, Params...>
{
private:
	using base = delegate_base<Return, Params...>;

protected:
	template <class FunctionClass> using traits = typename base::template traits<FunctionClass>;
	template <class FunctionClass> using member_ptr_func = typename traits<FunctionClass>::member_ptr_func;
	template <class FunctionClass> using const_member_ptr_func = typename traits<FunctionClass>::const_member_ptr_func;
	template <class FunctionClass> using static_ref_func = typename traits<FunctionClass>::static_ref_func;

public:
	delegate() : base() {};

	delegate(const base &src) : base(src) {}
	delegate(const base &src, delegate_bind &obj) : base(src, obj) {}

	explicit delegate(std::function<Return (Params...)> func) : base(func) {}

//	template<class FunctionClass>
//	delegate(member_ptr_func<FunctionClass> func, FunctionClass *obj)
//		: base(func, obj) {}
//
//	template<class FunctionClass>
//	delegate(const_member_ptr_func<FunctionClass> func, FunctionClass *obj)
//		: base(func, obj) {}
//
//	template<class FunctionClass>
//	delegate(static_ref_func<FunctionClass> func, FunctionClass *obj)
//		: base(func, obj) {}


};
