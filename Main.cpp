// main.cpp
#include <Engine/ScriptBindings.h>
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

int main()
{
	// Step 1: Set Mono system paths
	mono_set_dirs("C:/Program Files/Mono/lib", "C:/Program Files/Mono/etc");

	// Step 2: Initialize domain
	MonoDomain* domain = mono_jit_init("GameScriptDomain");

	// Step 3: Register internal call bindings
	RegisterScriptBindings();


	// Step 4: Load C# Assembly
	MonoAssembly* assembly = mono_domain_assembly_open(domain, "Assets/Scripts/Scripts.dll");
	if (!assembly) {
		std::cerr << "[C++] Failed to load Scripts.dll" << std::endl;
		return 1;
	}
	MonoImage* image = mono_assembly_get_image(assembly);


	// Step 5: Create MyScript instance and call Update
	MonoClass* klass = mono_class_from_name(image, "Engine", "MyScript");
	MonoObject* obj = mono_object_new(domain, klass);
	mono_runtime_object_init(obj);


	MonoMethod* updateMethod = mono_class_get_method_from_name(klass, "Update", 0);
	std::cout << "[C++] Script bindings registered." << std::endl;

	MonoObject* exc = nullptr;
	mono_runtime_invoke(updateMethod, obj, nullptr, &exc);

	if (exc) {
		MonoString* msg = mono_object_to_string(exc, nullptr);
		char* utf8 = mono_string_to_utf8(msg);
		std::cout << "[C++] Exception in C#: " << utf8 << std::endl;
		mono_free(utf8);
	}

	std::cout << "[C++] Done executing script." << std::endl;

	mono_jit_cleanup(domain);
	return 0;
}
