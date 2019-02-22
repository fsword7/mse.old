/*
 * didebug.h
 *
 *  Created on: Feb 21, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "emu/device.h"

//typedef device_interface_iterator<di_memory> dimem_iterator;

class di_debug : public device_interface
{
	template <typename T, typename U>
	struct is_related_class {
		static constexpr bool value = std::is_convertible<std::add_pointer<T>, std::add_pointer<U>>::value;
	};

public:
	di_debug(device_t *dev);
	~di_debug();

};
