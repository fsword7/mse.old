/*
 * devicei.h - device inline templates
 *
 *  Created on: Feb 21, 2019
 *      Author: Tim Stark
 */

#pragma once

template <class DeviceClass> template <typename... Params>
inline DeviceClass *device_type<DeviceClass>::operator()(system_config &config, tag_t *tag, Params &&... args) const
{
	return dynamic_cast<DeviceClass *>(config.addDeviceType(tag, *this, std::forward<Params>(args)...));
}


template <typename T, typename Ret, typename... Params>
inline std::enable_if_t<di_memory::is_related_class<device_t, T>::value>
	di_memory::setAddressMap(int space, Ret (T::*func)(Params... args))
{
//	device_t *dev = getDevice()->config().getCurrentDevice();
	device_t *dev = getDevice()->config().getSystemDevice();
//	std::cout << "Device " << dev->getType().sname() << ": tag " << dev->tag() << std::endl;
	setAddressMap(space, mapConstructor(func, dev->tag().c_str(), &dynamic_cast<T &>(*dev)));
}

template <typename T, typename Ret, typename... Params>
inline std::enable_if_t<!di_memory::is_related_class<device_t, T>::value>
	di_memory::setAddressMap(int space, Ret (T::*func)(Params... args))
{
//	device_t *dev = getDevice()->config().getCurrentDevice();
	device_t *dev = getDevice()->config().getSystemDevice();
//	std::cout << "Device " << dev->getType().sname() << ": tag " << dev->tag() << std::endl;
	setAddressMap(space, mapConstructor(func, dev->tag().c_str(), &dynamic_cast<T &>(*dev)));
}
