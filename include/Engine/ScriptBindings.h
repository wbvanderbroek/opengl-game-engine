// ScriptBindings.h
#pragma once
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/object.h>

MonoString* GetName_Internal() {
	std::string nativeName = "NativeGameObjectName";
	return mono_string_new(mono_domain_get(), nativeName.c_str());
}


inline void SetName_Internal(MonoString* monoStr)
{
	char* utf8 = mono_string_to_utf8(monoStr);
	std::cout << "[C++] SetName_Internal called with: " << utf8 << std::endl;
	mono_free(utf8);
}

inline void RegisterScriptBindings()
{
	mono_add_internal_call("Engine.GameObject::GetName_Internal", (const void*)GetName_Internal);
	mono_add_internal_call("Engine.GameObject::SetName_Internal", (const void*)SetName_Internal);
}
