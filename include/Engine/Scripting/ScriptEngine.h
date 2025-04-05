#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

class ScriptEngine
{
public:
	static void Initialize();
	static void SetMonoDomain(MonoDomain* domain);
	static void SetAssembly(MonoAssembly* assembly);
	static void SetImage(MonoImage* image);

	static MonoDomain* GetDomain();
	static MonoAssembly* GetAssembly();
	static MonoImage* GetImage();

private:
	static inline MonoDomain* s_domain = nullptr;
	static inline MonoAssembly* s_assembly = nullptr;
	static inline MonoImage* s_image = nullptr;
};
