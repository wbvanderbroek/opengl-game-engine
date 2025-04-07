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

	MonoMethod* m_awakeMethod = nullptr;
	MonoMethod* m_startMethod = nullptr;
	MonoMethod* m_updateMethod = nullptr;
	MonoMethod* m_lateUpdateMethod = nullptr;
	MonoMethod* m_onDestroyMethod = nullptr;
	MonoMethod* m_onQuitMethod = nullptr;

	ScriptComponent(const std::string& className = "PlayerController")
		: m_className(className)
	{
		m_runInEditor = true;
	}

	void Awake() override;
	void Start() override;
	void Update(float deltaTime) override;
	void LateUpdate(float deltaTime) override;
	void OnDestroy() override;
	void OnQuit() override;
};

REGISTER_COMPONENT(ScriptComponent)