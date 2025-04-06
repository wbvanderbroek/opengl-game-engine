#include <Engine/GameObject.h>
#include <Engine/Scripting/ScriptComponent.h>
#include <glm/glm.hpp>
#include <iostream>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

struct CS_Vector3 { float x, y, z; };

extern "C" {

	void GameEngine_Transform_get_position_Injected(void* nativePtr, CS_Vector3* out)
	{
		auto* go = static_cast<GameObject*>(nativePtr);
		if (!go)
		{
			std::cout << "[C++] GameObject is null in get_position_Injected" << std::endl;
			return;
		}

		glm::vec3 pos = go->GetLocalPosition();

		out->x = pos.x;
		out->y = pos.y;
		out->z = pos.z;
	}

	void GameEngine_Transform_set_position_Injected(void* nativePtr, CS_Vector3 value)
	{
		auto* go = static_cast<GameObject*>(nativePtr);
		if (!go)
		{
			std::cout << "[C++] GameObject is null in set_position_Injected!" << std::endl;
			return;
		}

		go->SetLocalPosition(glm::vec3(value.x, value.y, value.z));
	}
}



void RegisterTransformBindings()
{
	mono_add_internal_call("GameEngine.Transform::GameEngine_Transform_get_position_Injected", (const void*)GameEngine_Transform_get_position_Injected);
	mono_add_internal_call("GameEngine.Transform::GameEngine_Transform_set_position_Injected", (const void*)GameEngine_Transform_set_position_Injected);
}


void RegisterScriptBindings()
{
	RegisterTransformBindings();
	// ... later you'll add Light, Camera, etc.
}
