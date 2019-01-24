/*
 * driver.h
 *
 *  Created on: Jan 9, 2019
 *      Author: Tim Stark
 */

#pragma once

struct system_driver
{
	typedef device_t *(*creator_t)(const char *, const system_config &, device_t *, uint64_t);
	typedef void (*system_creator)(device_t &);
	typedef void (*driver_init)(device_t &);

	const char		*name;		// machine name
	const char 		*parent;	// parent of machine name
	creator_t		creator;	// system device creator
	system_creator	create;		// system create callback
	driver_init		init;		// system initialize callback
};

//class system_driver
//{
//public:
//	typedef device_t *(*creator_t)(const char *, const system_config &, device_t *);
//	typedef void (*system_creator)(device_t &);
//	typedef void (*driver_init)(device_t &);
//
//	const char		*name;		// machine name
//	const char 		*parent;	// parent of machine name
////	device_type		type;		// Static type info for system device
//	creator_t		creator;	// system device creator
//	system_creator	create;		// system create callback
//	driver_init		init;		// system initialize callback
//};

#define SYSTEM_NAME(name)			driver_##name
//#define SYSTEM_EXTERN(name)			extern system_driver const SYSTEM_NAME(name)
//
//#define SYSTEM_TYPE(Name, Class) system_creator<Class>

//#define COMPX(Name, Parent, Class, Create, Init, Fullname)	\
//extern system_driver const SYSTEM_NAME(Name)		\
//{													\
//	#Name,											\
//	#Parent,										\
//	SYSTEM_TYPE(Name, Class),						\
//	device_t<Class>::create,						\
//	[] (device_t &owner) { static_cast<Class &>(owner).Create(); }, \
//	[] (device_t &owner) { static_cast<Class &>(owner).Init(); }	\
//};

//#define COMPX(Name, Parent, Class, Create, Init, Fullname)	\
//extern system_driver const SYSTEM_NAME(Name)		\
//{													\
//	#Name,											\
//	#Parent,										\
//	device_t::create<Class>,						\
//	[] (device_t &owner) { static_cast<Class &>(owner).Create(); }, \
//	[] (device_t &owner) { static_cast<Class &>(owner).Init(); }	\
//};

#define COMP(Name, Parent, Class, Create, Init, Fullname)	\
{													\
	#Name,											\
	#Parent,										\
	device_t::create<Class>,						\
	[] (device_t &owner) { static_cast<Class &>(owner).Create(); }, \
	[] (device_t &owner) { static_cast<Class &>(owner).Init(); },	\
}
