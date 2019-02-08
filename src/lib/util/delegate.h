/*
 * delegate.h
 *
 *  Created on: Jan 8, 2019
 *      Author: Tim Stark
 */

#pragma once

#include <functional>
#include <typeinfo>
#include <exception>
#include <cstdint>

using delegate_generic_func = void(*)();

class delegate_generic_class;

class delegate_bind
{
public:
	delegate_bind() {}
	virtual ~delegate_bind() {}
};

class binding_type_exception : public std::exception
{
public:
	binding_type_exception(const std::type_info &target, const std::type_info &actual)
		: target_type(target), actual_type(actual) {}

	const std::type_info &target_type;
	const std::type_info &actual_type;
};

template<typename Class, typename Return, typename... Params>
struct delegate_traits
{
	using static_ptr_func = Return (*)(Class *, Params...);
	using static_ref_func = Return (&)(Class *, Params...);
	using member_ptr_func = Return (Class::*)(Params...);
	using const_member_ptr_func = Return (Class::*)(Params...) const;
};

class delegate_mfp
{
public:
	delegate_mfp() : func(0), thisDelta(0) {}

	template<typename MemberFunctionType, class MemberFunctionClass, typename ReturnType, typename StaticFunctionType>
	delegate_mfp(MemberFunctionType mfp, MemberFunctionClass *, ReturnType *, StaticFunctionType)
	{
		*reinterpret_cast<MemberFunctionType *>(this) = mfp;
	}

	bool isnull() const { return (func == 0 && thisDelta == 0); }

	template <typename FunctionType>
	void update_after_bind(FunctionType &func, delegate_generic_class *&obj)
	{
		func = reinterpret_cast<FunctionType>(convert_to_generic(obj));
	}

private:
	delegate_generic_func convert_to_generic(delegate_generic_class *&obj) const;

	uintptr_t 	func;
	int			thisDelta;
};

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
		  stdfunc(nullptr),
		  binder(nullptr)
	{
	}

	delegate_base(functype func)
		: function(nullptr),
		  object(nullptr),
		  stdfunc(func),
		  binder(nullptr)
	{
	}

	template <class FunctionClass>
	delegate_base(typename traits<FunctionClass>::member_ptr_func func, FunctionClass *obj)
		: function(nullptr),
		  object(nullptr),
		  stdfunc(nullptr),
		  binder(&bind_helper<FunctionClass>),
		  mfp(func, obj, static_cast<Return *>(nullptr), static_cast<generic_static_func>(nullptr))
	{
		bind(reinterpret_cast<delegate_generic_class *>(obj));
	}

	template <class FunctionClass>
	delegate_base(typename traits<FunctionClass>::const_member_ptr_func func, FunctionClass *obj)
		: function(nullptr),
		  object(nullptr),
		  stdfunc(nullptr),
		  binder(&bind_helper<FunctionClass>),
		  mfp(func, obj, static_cast<Return *>(nullptr), static_cast<generic_static_func>(nullptr))
	{
		bind(reinterpret_cast<delegate_generic_class *>(obj));
	}

	template <class FunctionClass>
	delegate_base(typename traits<FunctionClass>::static_ref_func func, FunctionClass *obj)
		: function(reinterpret_cast<generic_static_func>(func)),
		  object(nullptr),
		  stdfunc(nullptr),
		  binder(&bind_helper<FunctionClass>)
	{
		bind(reinterpret_cast<delegate_generic_class *>(obj));
	}


	bool is_mfp() const { return !mfp.isnull(); }
	bool isnull() const { return (function != nullptr && stdfunc != nullptr && mfp.isnull()); }

	void bind(delegate_bind &obj) { if (binder != nullptr) bind((*binder)(obj)); }

	Return operator()(Params... args) const
	{
//		if (is_mfp())
//			return (*reinterpret_cast<generic_member_func>(function))(object, std::forward<Params>(args)...);
		if (stdfunc != nullptr)
			return stdfunc(std::forward<Params>(args)...);
		else
			return (*function)(object, std::forward<Params>(args)...);
	}

protected:
	using bind_func = delegate_generic_class*(*)(delegate_bind &obj);

	template <class FunctionClass>
	static delegate_generic_class *bind_helper(delegate_bind &obj)
	{
		FunctionClass *result = dynamic_cast<FunctionClass *>(&obj);
		if (result == nullptr)
			throw binding_type_exception(typeid(FunctionClass), typeid(obj));

		return reinterpret_cast<delegate_generic_class *>(result);
	}

	void bind(delegate_generic_class *obj)
	{
		object = obj;

		if (object != nullptr && is_mfp())
			mfp.update_after_bind(function, object);
	}

	delegate_generic_class	*object;
	generic_static_func		function;
	functype 				stdfunc;	// std::funcion pointer
	bind_func				binder;
	delegate_mfp			mfp;
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

	template<class FunctionClass>
	delegate(member_ptr_func<FunctionClass> func, FunctionClass *obj)
		: base(func, obj) {}

	template<class FunctionClass>
	delegate(const_member_ptr_func<FunctionClass> func, FunctionClass *obj)
		: base(func, obj) {}

	template<class FunctionClass>
	delegate(static_ref_func<FunctionClass> func, FunctionClass *obj)
		: base(func, obj) {}


};
