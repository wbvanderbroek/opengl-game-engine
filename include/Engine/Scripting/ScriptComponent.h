#pragma once

#include <Engine/Components/Component.h>
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <string>

class ScriptComponent : public Component
{
public:
	std::string m_className;
	MonoObject* m_instance = nullptr;
	MonoMethod* m_updateMethod = nullptr;

	ScriptComponent(const std::string& className = "PlayerController")
		: m_className(className)
	{
		m_runInEditor = true; // Optional: allows script to run in editor
	}

	void Awake() override;
	void Start() override;
	void Update(float deltaTime) override;
};

REGISTER_COMPONENT(ScriptComponent)
