/*
 * driver.h
 *
 *  Created on: Jan 9, 2019
 *      Author: Tim Stark
 */

#pragma once

struct system_driver
{
	typedef void (*driver_init)(device_t &);

	const char		*name;		// machine name
	const char 		*parent;	// parent of machine name
	driver_init		init;		// system initialize callback
};


#define SYSTEM_NAME(name)			driver_##name

//#define COMP(Name, Parent, Class, Init, Fullname)	\
//extern system_driver const SYSTEM_NAME(Name)		\
//{													\
//	#Name,											\
//	#Parent,										\
//	[] (device_t &owner) { static_cast<Class &>(owner).Init(); },	\
//};

#define COMP(Name, Parent, Class, Init, Fullname)	\
{													\
	#Name,											\
	#Parent,										\
	[] (device_t &owner) { static_cast<Class &>(owner).Init(); },	\
}
