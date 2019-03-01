/*
 * dimem.h
 *
 *  Created on: Feb 4, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "emu/device.h"
#include "emu/addrmap.h"
#include "emu/emumap.h"

#define cfgSetAddressMap(_cpu, _space, _map) \
	dynamic_cast<di_memory *>(_cpu)->setAddressMap(_space, \
		mapConstructor(&std::remove_pointer_t<decltype(this)>::_map, tag().c_str(), this))

typedef device_interface_iterator<di_memory> dimem_iterator;

class di_memory : public device_interface
{
private:
	template <typename T, typename U>
	struct is_related_class {
		static constexpr bool value = std::is_convertible<std::add_pointer<T>, std::add_pointer<U>>::value;
	};

	template <typename T, typename U>
	struct is_related_device {
		static constexpr bool value = is_device_implementation<T>::value && is_related_class<T, U>::value;
	};

	template <typename T, typename U>
	struct is_related_interface {
		static constexpr bool value = is_device_interface<T>::value && is_related_class<T, U>::value;
	};

	template <typename T, typename U>
	struct is_unrelated_device {
		static constexpr bool value = is_device_implementation<T>::value && !is_related_class<T, U>::value;
	};

	template <typename T, typename U>
	struct is_unrelated_interface {
		static constexpr bool value = is_device_interface<T>::value && !is_related_class<T, U>::value;
	};

public:
	di_memory(device_t *dev);
	~di_memory();

	mapConstructor getAddressMap(int space = 0);
	const mapAddressConfig *getAddressSpaceConfig(int space) const;
	mapAddressConfig *getAddressSpaceConfig(int space);

	int mapConfigCount() { return mapConfig.size(); }

//	template <typename T, typename U, typename Ret, typename... Params>
//	std::enable_if_t<is_related_device<device_t, T>::value>
//	setAddressMap(int space, T &obj, Ret (U::*func)(Params...))
//	{
//		setAddressMap(space, mapConstructor(func, obj.tag(), &downcast<U &>(obj)));
//	}
//
//	template <typename T, typename U, typename Ret, typename... Params>
//	std::enable_if_t<is_related_interface<device_t, T>::value>
//	setAddressMap(int space, T &obj, Ret (U::*func)(Params...))
//	{
//		setAddressMap(space, mapConstructor(func, obj.device().tag(), &downcast<U &>(obj)));
//	}

	template <typename T, typename U, typename Ret, typename... Params>
	std::enable_if_t<is_unrelated_device<device_t, T>::value>
	setAddressMap(int space, T &obj, Ret (U::*func)(Params...))
	{
		setAddressMap(space, mapConstructor(func, obj.tag().c_str(), &dynamic_cast<U &>(obj)));
	}

	template <typename T, typename U, typename Ret, typename... Params>
	std::enable_if_t<is_unrelated_interface<device_t, T>::value>
	setAddressMap(int space, T &obj, Ret (U::*func)(Params...))
	{
		setAddressMap(space, mapConstructor(func, obj.device().tag().c_str(), &dynamic_cast<U &>(obj)));
	}

	template <typename T, typename Ret, typename... Params>
	std::enable_if_t<is_related_class<device_t, T>::value>
	setAddressMap(int space, Ret (T::*func)(Params...));

	template <typename T, typename Ret, typename... Params>
	std::enable_if_t<!is_related_class<device_t, T>::value>
	setAddressMap(int space, Ret (T::*func)(Params...));

	void setAddressMap(int space, mapConstructor map);

	void validate() const;

	template <typename Space> void allocate(mapMemoryManager &manager, int space)
	{
		assert((space >= 0) && (space < mapConfig.size()));
		mapSpace.resize(std::max(int(mapSpace.size()), space+1));

		assert(mapSpace[space] != nullptr);
		mapSpace[space] = new Space(manager, *this, space, mapConfig[space]->address_width());

		std::cout << "Allocated address space." << std::endl;
	}

	// Address space initialization calls
	void prepare() { for (auto const &space : mapSpace) if (space != nullptr) { space->prepare(); } }
	void populate() { for (auto const &space : mapSpace) if (space != nullptr) { space->populate(); }}
	void allocate() { for (auto const &space : mapSpace) if (space != nullptr) { space->allocate(); }}
	void locate() { for (auto const &space : mapSpace) if (space != nullptr) { space->locate(); }}
	void set_log_unmap() { }

protected:
	std::vector<mapConstructor>		mapAddress;

private:
	std::vector<mapAddressConfig *>	mapConfig;
	std::vector<mapAddressSpace *>	mapSpace;
};
