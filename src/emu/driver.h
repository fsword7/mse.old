/*
 * driver.h
 *
 *  Created on: Jan 9, 2019
 *      Author: Tim Stark
 */

#pragma once

class romEntry;


class system_driver
{
public:
//	typedef device_t *(*creator_t)(const system_config &, tag_t *, device_t *, uint64_t);
	typedef void (*system_creator)(system_config &, device_t &);
	typedef void (*driver_init)(device_t &);

	const char		*name;			// machine name
	const char 		*parent;		// parent of machine name
	const device_type_base	&type;
//	creator_t		creator;		// system device creator
	system_creator	create;			// system create callback
	driver_init		init;			// system initialize callback
	romEntry		*rom;			// ROM entries for firmware
	const char		*description;	//Description/full name
	const char		*source;		// source file name
};

#define SYSTEM_NAME(Name) Name##_driver
#define SYSTEM_EXTERN(Name) extern const system_driver SYSTEM_NAME(Name)
#define SYSTEM_TYPE(Name, Class) systemCreator<Class>

#define COMP(Name, Parent, Type, Class, Create, Init, Company, Description)	\
extern const system_driver SYSTEM_NAME(Name) =		\
{													\
	#Name,											\
	#Parent,										\
	SYSTEM_TYPE(Name, Class),						\
	[] (system_config &config, device_t &owner) { static_cast<Class &>(owner).Create(config); }, \
	[] (device_t &owner) { static_cast<Class &>(owner).Init(); },	\
	ROM_NAME(Name),		\
	Description,		\
	__FILE__			\
};
