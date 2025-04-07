#include <Engine/Scripting/ScriptBindings.h>
#include <Engine/Scripting/ScriptEngine.h>
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>

void ScriptEngine::Initialize()
{
	// Init Mono and scripting system
	mono_set_dirs("Mono/lib", "Mono/etc");
	mono_config_parse(nullptr);

	MonoDomain* domain = mono_jit_init("GameScriptDomain");
	if (!domain)
	{
		std::cerr << "[Mono] Failed to initialize Mono JIT!" << std::endl;
		return;
	}

	// Load C# assembly
	MonoAssembly* assembly = mono_domain_assembly_open(domain, "../Game/Assets/Scripts/Scripts.dll");
	if (!assembly)
	{
		std::cerr << "[Mono] Failed to load Scripts.dll!" << std::endl;
		return;
	}

	// Register C++ internal calls for C# (e.g., transform.position)
	RegisterScriptBindings();

	// Store domain & image if needed globally
	ScriptEngine::SetMonoDomain(domain);
	ScriptEngine::SetAssembly(assembly);
	ScriptEngine::SetImage(mono_assembly_get_image(assembly));
}

void ScriptEngine::SetMonoDomain(MonoDomain* domain)
{
	s_domain = domain;
}

void ScriptEngine::SetAssembly(MonoAssembly* assembly)
{
	s_assembly = assembly;
}

void ScriptEngine::SetImage(MonoImage* image)
{
	s_image = image;
}

MonoDomain* ScriptEngine::GetDomain()
{
	return s_domain;
}

MonoAssembly* ScriptEngine::GetAssembly()
{
	return s_assembly;
}

MonoImage* ScriptEngine::GetImage()
{
	return s_image;
}
