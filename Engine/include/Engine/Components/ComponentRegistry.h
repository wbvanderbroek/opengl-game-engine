#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

class Component;

using ComponentFactory = std::function<std::shared_ptr<Component>()>;

class ComponentRegistry
{
public:
	static ComponentRegistry& Instance()
	{
		static ComponentRegistry instance;
		return instance;
	}

	void Register(const std::string& name, ComponentFactory factory)
	{
		m_registry[name] = factory;
	}

	const std::unordered_map<std::string, ComponentFactory>& GetRegistry() const
	{
		return m_registry;
	}

private:
	std::unordered_map<std::string, ComponentFactory> m_registry;
};

#define REGISTER_COMPONENT(TYPE) \
	static bool _##TYPE##_registered = [] { \
		ComponentRegistry::Instance().Register(#TYPE, [] { return std::make_shared<TYPE>(); }); \
		return true; \
	}();