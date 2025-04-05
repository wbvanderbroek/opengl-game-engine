#include <Engine/Scripting/ScriptEngine.h>

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
