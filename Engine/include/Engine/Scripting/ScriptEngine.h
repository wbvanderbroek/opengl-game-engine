#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

class ScriptEngine
{
public:
	static void Initialize();
	static void SetMonoDomain(MonoDomain* domain);
	static void SetAssembly(MonoAssembly* assembly);
	static void SetScriptsImage(MonoImage* image);

	static MonoDomain* GetDomain();
	static MonoAssembly* GetAssembly();
	static MonoImage* GetScriptsImage();
	static void SetEngineImage(MonoImage* image);
	static MonoImage* GetEngineImage();

private:
	static inline MonoDomain* s_domain = nullptr;
	static inline MonoAssembly* s_assembly = nullptr;
	static inline MonoImage* s_scriptsImage = nullptr;
	static inline MonoImage* s_engineImage = nullptr;
};
