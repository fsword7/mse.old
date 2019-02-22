/*
 * device.h - Base Device Class facility
 *
 *  Created on: Jan 6, 2019
 *      Author: Tim Stark
 */

#pragma once

#include <typeinfo>

class system_config;
class device_list;
class device_t;
class devauto_base;
class machine;
class di_memory;
class di_execute;
class validity_checker;


template <class DeviceClass>
struct device_tag_struct { typedef DeviceClass type; };
template <class DeviceClass>
auto device_tag_func() { return device_tag_struct<DeviceClass>{}; };

template <class SystemClass>
struct system_tag_struct { typedef SystemClass type; };
template <class SystemClass>
auto system_tag_func() { return system_tag_struct<SystemClass>{}; };

class device_type_base
{
	typedef device_t *(*create_func)(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock);

	template <typename DeviceClass>
	static device_t *createDevice(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock)
	{
		return new DeviceClass(config, tag, owner, clock);
	}

	template <typename SystemClass>
	static device_t *createSystem(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock)
	{
		return new SystemClass(config, tag, clock);
	}

public:
	device_type_base()
	: idType(typeid(std::nullptr_t)),
	  shortName(nullptr),
	  fullName(nullptr),
	  srcName(nullptr),
	  creator(nullptr)
	{}

	template <class SystemClass>
	device_type_base(system_tag_struct<SystemClass>(*)())
	: idType(typeid(SystemClass)),
	  shortName(nullptr),
	  fullName(nullptr),
	  srcName(nullptr),
	  creator(&createSystem<SystemClass>)
	{}

	template <class DeviceClass>
	device_type_base(device_tag_struct<DeviceClass>(*)())
	: idType(typeid(DeviceClass)),
	  shortName(nullptr),
	  fullName(nullptr),
	  srcName(nullptr),
	  creator(&createDevice<DeviceClass>)
	{}

	device_t *create(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock) const
	{
		assert(creator != nullptr);
		return creator(config, tag, owner, clock);
	}

	const std::type_info &type()	{ return idType; }
	const char *sname()				{ return shortName; }
	const char *fname()				{ return fullName; }
	const char *source()			{ return srcName; }

private:
	const std::type_info	&idType;		// device type identification
	const char				*shortName;		// short name
	const char				*fullName;		// full name/description
	const char				*srcName;		// Name of source file
	const create_func		creator;
};

template <class DeviceClass>
class device_type : public device_type_base
{
public:
	using device_type_base::device_type_base;
	using device_type_base::create;

//	template <typename... Params>
//	DeviceClass *create(const system_config &config, tag_t *tag, device_t *owner, Params &&... args) const
//	{
//		return new DeviceClass(config, tag, owner, std::forward<Params>(args)...);
//	}

	template <typename... Params>
	DeviceClass *operator() (system_config &config, tag_t *tag, Params&&... args) const;

};

template <class SystemClass>
constexpr auto systemCreator = &system_tag_func<SystemClass>;

template <class DeviceClass>
constexpr auto deviceCreator = &device_tag_func<DeviceClass>;

#define DECLARE_DEVICE_TYPE(Type, Class) \
	extern device_type<Class> const &Type;

#define DEFINE_DEVICE_TYPE(Type, Class, ShortName, FullName) \
	device_type<Class> const &Type = deviceCreator<Class>;

// *************************************************************************

class device_interface
{
	friend class interface_list;

protected:
	device_interface(device_t *dev, tag_t *name);
	virtual ~device_interface();

public:
	tag_t *type() { return typeName; }
	device_t *getDevice() { return device; }

	virtual void validate(validity_checker &valid) const;

protected:
	device_interface	*next;
	device_t			*device;
	tag_t				*typeName;
};

// *************************************************************************

class device_t : public delegate_bind
{
	friend class devauto_base;

public:
	class device_list
	{
	public:
		device_list() { list.clear(); }

		device_t *find(const std::string &name) const
		{
			for(auto cdev : list) {
				if (cdev->tag() == name)
					return cdev;
			}
			return nullptr;
		}

		void add(device_t *dev) {
			list.push_back(dev);
		}

		device_t *first() { return list[0]; }
		device_t *last()  { return list[size()]; }

		int size()   { return list.size(); }
		bool empty() { return list.empty(); }

		std::vector<device_t *>::iterator begin() { return list.begin(); }
		std::vector<device_t *>::iterator end() { return list.end(); }
		std::vector<device_t *>::const_iterator begin() const { return list.cbegin(); }
		std::vector<device_t *>::const_iterator end() const { return list.cend(); }

	private:
		std::vector<device_t *> list;
	};

	class interface_list
	{
		friend class device_t;
		friend class device_interface;
		friend class di_memory;
		friend class di_execute;

	public:
		interface_list()
		: execute(nullptr), memory(nullptr)
		{ list.clear(); }

		device_interface *first() { return list.front(); }
		device_interface *last()  { return list.back(); }

		int size()   { return list.size(); }
		bool empty() { return list.empty(); }

		void add(device_interface *intf) {
			list.push_back(intf);
		}

		std::vector<device_interface *>::iterator begin() { return list.begin(); }
		std::vector<device_interface *>::iterator end() { return list.end(); }
		std::vector<device_interface *>::const_iterator begin() const { return list.cbegin(); }
		std::vector<device_interface *>::const_iterator end() const { return list.cend(); }

	private:
		std::vector<device_interface *> list;

		di_execute		*execute;
		di_memory 		*memory;
	};

protected:
	device_t(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock);

public:
	virtual ~device_t();

	template <class DeviceClass>
	static device_t *create(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock)
	{
//		DeviceClass *device = new DeviceClass(tag, config, clock);
//		return config.addDevice(device, owner);
		return new DeviceClass(config, tag, clock);
	}

//	const char *tag() { return tagName.c_str(); }
	std::string &tag() { return tagName; }
//	const char *name() { return drvName.c_str(); }
	std::string &name() { return devName; }
	device_t *owner() { return devOwner; }
	device_t *next()  { return devNext; }

	const system_config &config() const { return sysConfig; }

//	void setSystemConfig(const system_config &config) { sysConfig = config; }

	void addSystemConfig(const system_config &config);

	device_list &devices() { return deviceList; }
	const device_list &devices() const { return deviceList; }

	interface_list &interfaces() { return interfaceList; }
	const interface_list &interfaces() const { return interfaceList; }

	bool hasInterface(di_memory *&intf)  { return ((intf = interfaceList.memory) != nullptr); }
	bool hasInterface(di_execute *&intf) { return ((intf = interfaceList.execute) != nullptr); }

	bool hasInterface(di_memory *&intf) const  { return ((intf = interfaceList.memory) != nullptr); }
	bool hasInterface(di_execute *&intf) const { return ((intf = interfaceList.execute) != nullptr); }

	void validate(validity_checker &valid) const;

	virtual void validateDevice(validity_checker &valid) const;

protected:
	devauto_base *register_device(devauto_base *autodev);

	void setMachine(machine *sys) { system = sys; }
	void resolvePreMap();
	void resolvePostMap();

protected:
	device_t 		*devOwner;			// Parent device owner
	device_t 		*devNext;			// Next device of the same owner
	device_list 	deviceList;			// List of child devices - container
	interface_list	interfaceList;		// List of interfaces - container


	const system_config &sysConfig;

	machine  *system;

private:
	std::string		tagName;			// Tag name for linking named devices
	std::string		devName;			// Device name for command line access

	std::vector<devauto_base *> acList;	// List of auto device configurations
};

// *************************************************************************

class device_iterator
{
public:
	device_iterator(device_t &dev, int depth = 255)
	: devRoot(dev), maxDepth(depth) {}

	class iterator
	{
	public:
		iterator(device_t *dev, int depth, int mdepth)
		: cdev(dev), curDepth(depth), maxDepth(mdepth) {}

		device_t *current() { return cdev; }
		int depth() { return curDepth; }

		// Required operator function calls
		bool operator == (iterator const &iter) { return cdev == iter.cdev; }
		bool operator != (iterator const &iter) { return cdev != iter.cdev; }
		iterator &operator ++ () { advance(); return *this; }
		iterator operator ++ (int) { iterator const result(*this); ++*this; return result; }
		device_t &operator * ()  { assert(cdev); return *cdev; }
		device_t *operator -> () { return cdev; }

	protected:
		void advance()
		{
			if (cdev != nullptr) {
				device_t *start = cdev;

				if (curDepth < maxDepth)
				{
					cdev = start->devices().first();
					if (cdev != nullptr) {
						curDepth++;
						return;
					}
				}

				while(curDepth > 0 && start)
				{
					cdev = start->next();
					if (cdev != nullptr)
						return;

					start = start->owner();
					curDepth--;
				}

				cdev = nullptr;
			}
		}

		device_t	*cdev;
		int			curDepth;
		const int	maxDepth;
	};

	iterator begin() const { return iterator(&devRoot, 0, maxDepth); }
	iterator end() const   { return iterator(nullptr, 0, maxDepth); }

	device_t *first() const { return begin().current(); }

	int count()
	{
		int result = 0;
		for (device_t &dev : *this)
			result++;
		return result;
	}

private:
	device_t	&devRoot;
	int			maxDepth;
};

template <class InterfaceClass>
class device_interface_iterator
{
public:
	device_interface_iterator(device_t &dev, int depth = 255)
	: devRoot(dev), maxDepth(depth) {}

	class iterator : public device_iterator::iterator
	{
	public:
		iterator(device_t *dev, int depth, int mdepth)
		: device_iterator::iterator(dev, depth, mdepth)
		{
			findInterface();
		}

		InterfaceClass *current() const { return interface; }

		// Required operator function calls
		bool operator == (iterator const &iter) { return interface == iter.interface; }
		bool operator != (iterator const &iter) { return interface != iter.interface; }
		iterator &operator ++ () { advance(); findInterface(); return *this; }
		iterator operator ++ (int) { iterator const result(*this); ++*this; return result; }
		InterfaceClass &operator * ()  { assert(interface != nullptr); return *interface; }
		InterfaceClass *operator -> () { return interface; }

	private:
		void findInterface()
		{
			for (; cdev != nullptr; advance())
				if (cdev->hasInterface(interface))
					return;
		}

		InterfaceClass *interface;
	};

	iterator begin() const { return iterator(&devRoot, 0, maxDepth); }
	iterator end() const   { return iterator(nullptr, 0, maxDepth); }

	InterfaceClass *first() const { return begin().current(); }

	int count()
	{
		int result = 0;
		for (InterfaceClass &intf : *this)
			result++;
		return result;
	}

private:
	device_t	&devRoot;
	int			maxDepth;
};
