/*
 * emucore.cpp
 *
 *  Created on: Feb 22, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/exception.h"

void report_bad_cast(const std::type_info &srcType, const std::type_info &dstType)
{
	throw mseFatalError("Error: bad downcast<> - Tried to convert %s to %s - Incompatible",
			srcType.name(), dstType.name());
}


void report_bad_device_cast(const device_t *dev, const std::type_info &srcType, const std::type_info &dstType)
{
	throw mseFatalError("Error: bad downcast<> - Tried to convert device %s (%s) of type %s to %s - Incompatible",
			dev->tag().c_str(), dev->fullName(), srcType.name(), dstType.name());
}

void msePrintf(const char *format, ...)
{
	char     out[2048];
	va_list  args;

	va_start(args, format);

	vsprintf(out, format, args);

	std::cout << out << std::flush;

	// All done, release arguments.
	va_end(args);
}
