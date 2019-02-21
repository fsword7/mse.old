/*
 * driver.h
 *
 *  Created on: Jan 9, 2019
 *      Author: Tim Stark
 */

#pragma once

class romEntry;

struct system_driver
{
	typedef device_t *(*creator_t)(const system_config &, tag_t *, device_t *, uint64_t);
	typedef void (*system_creator)(system_config &, device_t &);
	typedef void (*driver_init)(device_t &);

	const char		*name;			// machine name
	const char 		*parent;		// parent of machine name
	creator_t		creator;		// system device creator
	system_creator	create;			// system create callback
	driver_init		init;			// system initialize callback
	romEntry		*rom;			// ROM entries for firmware
	const char		*description;	//Description/full name
	const char		*source;		// source file name
};

#define COMP(Name, Parent, Type, Class, Create, Init, Company, Description)	\
extern const system_driver SYSTEM_NAME(Name) =		\
{													\
	#Name,											\
	#Parent,										\
	device_t::create<Class>,						\
	[] (system_config &config, device_t &owner) { static_cast<Class &>(owner).Create(config); }, \
	[] (device_t &owner) { static_cast<Class &>(owner).Init(); },	\
	ROM_NAME(Name),		\
	Description,		\
	__FILE__			\
};

//struct system_driver
//{
//	typedef device_t *(*creator_t)(const system_config &, tag_t *, device_t *, uint64_t);
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
//	typedef device_t *(*creator_t)(const system_config &, tag_t *, device_t *, uint64_t);
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
#define SYSTEM_EXTERN(Name) extern const system_driver SYSTEM_NAME(Name)

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
