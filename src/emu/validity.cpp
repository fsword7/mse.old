/*
 * validity.cpp
 *
 *  Created on: Feb 7, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/exception.h"
#include "emu/driver.h"
#include "emu/validity.h"

validity_checker::validity_checker()
: errors(0), warnings(0), validate_all(false)
{
}

validity_checker::~validity_checker()
{
	end();
}

void validity_checker::begin()
{
}

void validity_checker::end()
{
}

void validity_checker::checkDriver()
{

}

void validity_checker::validate_system()
{

}

void validity_checker::validate_devices()
{

}

void validity_checker::check(device_t *system)
{
	begin();



	end();
}

void validity_checker::validate()
{
//	try {
//		validate_system();
//	}
//	catch (mseFatalError &err) {
//
//	}
}

