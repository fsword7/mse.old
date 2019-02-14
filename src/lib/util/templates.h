/*
 * templates.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Tim Stark
 */

#pragma once

template <typename T>
constexpr T makeBitMask(unsigned int N)
{
	return T((N < (8 * sizeof(T))
		? (std::make_unsigned_t<T>(1) << N)
		: std::make_unsigned_t<T>(0)) - 1);
}

template <typename T>
constexpr std::enable_if_t<std::is_signed<T>::value, T> labs(T v) noexcept
{
	return (v < T(0)) ? -v : v;
}
