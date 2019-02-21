/*
 * devicei.h - device inline templates
 *
 *  Created on: Feb 21, 2019
 *      Author: Tim Stark
 */

#pragma once

template <class DeviceClass> template <typename... Params>
inline DeviceClass &device_type<DeviceClass>::operator()(system_config &config, tag_t *tag, Params &&... args) const
{
	return dynamic_cast<DeviceClass &>(*config.addDevice(tag, *this, std::forward<Params>(args)...));
}
