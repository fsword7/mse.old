/*
 * exception.cpp
 *
 *  Created on: Feb 7, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/exception.h"

mseFatalError::mseFatalError(const char *format, ...)
: code(0)
{
	va_list args;

	if (format == nullptr)
		message[0] = '\0';

	va_start(args, format);
	vsnprintf(message, sizeof(message), format, args);
	va_end(args);
}

mseFatalError::mseFatalError(int _code, const char *format, ...)
: code(_code)
{
	va_list args;

	if (format == nullptr)
		message[0] = '\0';

	va_start(args, format);
	vsnprintf(message, sizeof(message), format, args);
	va_end(args);
}
