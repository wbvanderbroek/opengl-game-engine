#include <Engine/Scripting/ScriptBindings.h>
#include <Engine/Scripting/ScriptEngine.h>
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>

void ScriptEngine::Initialize()
{
	mono_set_dirs("Mono/lib", "Mono/etc");
	mono_set_assemblies_path("CoreAssets/Scripting");
	mono_config_parse(nullptr);

	MonoDomain* domain = mono_jit_init("GameScriptDomain");
	if (!domain)
	{
		std::cerr << "[Mono] Failed to initialize Mono JIT!" << std::endl;
		return;
	}

	MonoAssembly* engineAssembly = mono_domain_assembly_open(domain, "CoreAssets/Scripting/GameEngine.dll");
	if (!engineAssembly)
	{
		std::cerr << "[Mono] Failed to load GameEngine.dll!" << std::endl;
		return;
	}

	MonoAssembly* scriptsAssembly = mono_domain_assembly_open(domain, "CoreAssets/Scripting/Game.dll");
	if (!scriptsAssembly)
	{
		std::cerr << "[Mono] Failed to load Scripts.dll!" << std::endl;
		return;
	}

	RegisterScriptBindings();

	ScriptEngine::SetMonoDomain(domain);
	ScriptEngine::SetAssembly(scriptsAssembly); // Scripts will be the "main" assembly
	ScriptEngine::SetScriptsImage(mono_assembly_get_image(scriptsAssembly));
	ScriptEngine::SetEngineImage(mono_assembly_get_image(engineAssembly));
}

void ScriptEngine::SetMonoDomain(MonoDomain* domain)
{
	s_domain = domain;
}

void ScriptEngine::SetAssembly(MonoAssembly* assembly)
{
	s_assembly = assembly;
}

void ScriptEngine::SetScriptsImage(MonoImage* image)
{
	s_scriptsImage = image;
}

MonoDomain* ScriptEngine::GetDomain()
{
	return s_domain;
}

MonoAssembly* ScriptEngine::GetAssembly()
{
	return s_assembly;
}

MonoImage* ScriptEngine::GetScriptsImage()
{
	return s_scriptsImage;
}

void ScriptEngine::SetEngineImage(MonoImage* image)
{
	s_engineImage = image;
}

MonoImage* ScriptEngine::GetEngineImage()
{
	return s_engineImage;
}
