/*
 * devauto.h - auto device configuration
 *
 *  Created on: Jan 27, 2019
 *      Author: Tim Stark
 */

#pragma once

class devauto_base
{
public:
	devauto_base(device_t &base, tag_t *tag);
	virtual ~devauto_base();

private:
	devauto_base	*next;
	device_t		&base;
	tag_t			*tag;
	bool			resolved;
};
