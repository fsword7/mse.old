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

#define COMP(Name, Parent, Type, Class, Create, Init, Fullname)	\
{													\
	#Name,											\
	#Parent,										\
	device_t::create<Class>,						\
	[] (device_t &owner) { static_cast<Class &>(owner).Create(); }, \
	[] (device_t &owner) { static_cast<Class &>(owner).Init(); },	\
}


//struct system_driver
//{
//	typedef device_t *(*creator_t)(const char *, const system_config &, device_t *, uint64_t);
//	typedef void (*system_creator)(device_t &);
//	typedef void (*driver_init)(device_t &);
//
//	template <class DeviceClass>
//	using devtype_t = device_type<DeviceClass>(const char *, const char *, const char *);
//
//	const char		*name;		// machine name
//	const char 		*parent;	// parent of machine name
//	template <class DeviceClass>
//	devtype_t<DeviceClass>	&type;		// static type info for system device
//	creator_t		creator;	// system device creator
//	system_creator	create;		// system create callback
//	driver_init		init;		// system initialize callback
//};

//class system_driver
//{
//	typedef device_t *(*creator_t)(const char *, const system_config &, device_t *, uint64_t);
//	typedef void (*system_creator)(device_t &);
//	typedef void (*driver_init)(device_t &);
//
//	const char		*name;		// machine name
//	const char 		*parent;	// parent of machine name
//	device_type		&type;		// static type info for system device
//	creator_t		creator;	// system device creator
//	system_creator	create;		// system create callback
//	driver_init		init;		// system initialize callback
//};

#define SYSTEM_NAME(Name) Name##_driver

#define SYSTEM_DEVICE_TYPE(Class, Name, FullName) \
	device_type<Class>(#Name, FullName, __FILE__)

//#define COMP(Name, Parent, Type, Class, Create, Init, Fullname)	\
//{													\
//	#Name,											\
//	#Parent,										\
//	Type,											\
//	device_t::create<Class>,						\
//	[] (device_t &owner) { static_cast<Class &>(owner).Create(); }, \
//	[] (device_t &owner) { static_cast<Class &>(owner).Init(); },	\
//}

//#define COMPX(Name, Parent, Type, Class, Create, Init, Fullname)	\
//extern system_driver SYSTEM_NAME(Name)				\
//{													\
//	#Name,											\
//	#Parent,										\
//	Type,											\
//	device_t::create<Class>,						\
//	[] (device_t &owner) { static_cast<Class &>(owner).Create(); }, \
//	[] (device_t &owner) { static_cast<Class &>(owner).Init(); },	\
//};
