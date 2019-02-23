/*
 * badcast.h - Report bad cast error message
 *
 *  Created on: Feb 22, 2019
 *      Author: Tim Stark
 */

#pragma once

void report_bad_cast(const std::type_info &srcType, const std::type_info &dstType);
void report_bad_device_cast(const device_t *dev, const std::type_info &srcType, const std::type_info &dstType);

template <typename Dest, typename Source>
inline std::enable_if_t<std::is_base_of<device_t, Source>::value>
	report_bad_cast(Source *const src)
{
	if (src != nullptr)
		report_bad_device_cast(src, typeid(Source), typeid(Dest));
	else
		report_bad_cast(typeid(Source), typeid(Dest));
}

template <typename Dest, typename Source>
inline std::enable_if_t<!std::is_base_of<device_t, Source>::value>
	report_bad_cast(Source *const src)
{
	device_t const *dev(dynamic_cast<device_t const *>(src));

	if (dev != nullptr)
		report_bad_device_cast(dev, typeid(Source), typeid(Dest));
	else
		report_bad_cast(typeid(Source), typeid(Dest));
}


template <typename Dest, typename Source>
inline Dest downcast(Source *src)
{
	Dest const chk(dynamic_cast<Dest>(src));
	if (chk != src)
		report_bad_cast<std::remove_pointer_t<Dest>, Source>(src);

	return static_cast<Dest>(src);
}

template <typename Dest, typename Source>
inline Dest downcast(Source &src)
{
	std::remove_reference_t<Dest> *const chk(dynamic_cast<std::remove_reference_t<Dest> *>(src));
	if (chk != &src)
		report_bad_cast<std::remove_reference_t<Dest>, Source>(&src);

	return static_cast<Dest>(src);
}
