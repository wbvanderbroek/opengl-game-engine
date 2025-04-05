#include <Engine/Scripting/ScriptComponent.h>
#include <Engine/Scripting/ScriptEngine.h>
#include <iostream>

void ScriptComponent::Awake()
{
	if (!m_gameObject)
		std::cout << "[ScriptComponent] m_gameObject is NULL in Awake()" << std::endl;
	else
		std::cout << "[ScriptComponent] GameObject assigned in Awake()" << std::endl;

	MonoImage* image = ScriptEngine::GetImage();
	if (!image) return;

	// Get the script class (e.g., Game.PlayerController)
	std::string ns = "";
	std::string className = m_className;

	auto dot = m_className.find_last_of('.');
	if (dot != std::string::npos) {
		ns = m_className.substr(0, dot);
		className = m_className.substr(dot + 1);
	}

	MonoClass* klass = mono_class_from_name(image, ns.c_str(), className.c_str());
	if (!klass) {
		std::cerr << "[ScriptComponent] Could not find class: " << m_className << std::endl;
		return;
	}

	m_instance = mono_object_new(ScriptEngine::GetDomain(), klass);

	mono_runtime_object_init(m_instance);

	// Inject C++ native ptr into Transform instance
	MonoClass* transformClass = mono_class_from_name(image, "GameEngine", "Transform");
	MonoObject* transformInstance = mono_object_new(ScriptEngine::GetDomain(), transformClass);
	mono_runtime_object_init(transformInstance);

	MonoClassField* nativeField = mono_class_get_field_from_name(transformClass, "m_nativePtr");
	mono_field_set_value(transformInstance, nativeField, &m_gameObject);

	// Assign Transform instance to C# script's 'transform' property
	MonoProperty* transformProp = mono_class_get_property_from_name(klass, "transform");
	if (transformProp)
	{
		void* args[1] = { transformInstance };
		MonoMethod* setter = mono_property_get_set_method(transformProp);
		if (setter)
		{
			mono_runtime_invoke(setter, m_instance, args, nullptr);
			std::cout << "[ScriptComponent] Transform injected into script" << std::endl;
		}
	}

	// Cache Update method if it exists
	m_updateMethod = mono_class_get_method_from_name(klass, "Update", 0);

	std::cout << "[ScriptComponent] Script instance created: " << m_instance << std::endl;
	if (!m_instance)
	{
		std::cerr << "[ScriptComponent] mono_object_new failed — m_instance is NULL" << std::endl;
		return;
	}
}

void ScriptComponent::Start()
{
	if (!m_instance)
	{
		std::cerr << "[ScriptComponent] m_instance is null in Start()\n";
		return;
	}

	MonoMethod* startMethod = mono_class_get_method_from_name(
		mono_object_get_class(m_instance), "Start", 0);

	if (startMethod)
		mono_runtime_invoke(startMethod, m_instance, nullptr, nullptr);
}

void ScriptComponent::Update(float deltaTime)
{
	if (m_updateMethod && m_instance)
		mono_runtime_invoke(m_updateMethod, m_instance, nullptr, nullptr);
}
