/*
 * devsys.h
 *
 *  Created on: Mar 2, 2017
 *      Author: Timothy Stark
 */

#pragma once

class Device
{
public:
	Device();
	~Device();

};

class sysDevice : public Device
{
public:
	sysDevice();
	~sysDevice();


private:
};



//struct sysDriver
//{
//	const char *srcFile;
//	const char *name;
//	const char *parent;
//	const char *desc;
//	const char *manfacturer;
//	const char *year;
//};

struct sysDriver
{
	const char *devName;  // Device name
	const char *devDesc;  // Device description
	const char *srcFile;  // Source file
	const char *pdevName; // Parent device name

};

//#define SYS_NAME(name) sysDriver_##name
//
//#define SYSTEM(name, parent, fname) \
//extern const sysDriver SYS_NAME(name) = \
//{              \
//	__FILE__,  \
//	#name,     \
//	#parent,   \
//	fname,     \
//	nullptr,   \
//	nullptr    \
//};
//
//#define GROUP(name) \
//extern const sysDriver SYS_NAME(name) = \
//{              \
//	__FILE__,  \
//	#name,     \
//	nullptr,   \
//	nullptr,   \
//	nullptr,   \
//	nullptr    \
//};
