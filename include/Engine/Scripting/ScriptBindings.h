#include <Engine/Components/Component.h>
#include <Engine/GameObject.h>
#include <glm/glm.hpp>
#include <iostream>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

struct CS_Vector3 { float x, y, z; };

extern "C" {

	// INTERNAL CALL: Get Transform.position
	void GameEngine_Transform_get_position_Injected(void* nativePtr, CS_Vector3* out)
	{
		auto* script = static_cast<Component*>(nativePtr);
		if (!script || !script->m_gameObject) return;

		glm::vec3 pos = script->m_gameObject->GetLocalPosition();
		out->x = pos.x;
		out->y = pos.y;
		out->z = pos.z;

		std::cout << "[C++] get_position_Injected: " << out->x << ", " << out->y << ", " << out->z << std::endl;
	}

	// INTERNAL CALL: Set Transform.position
	void GameEngine_Transform_set_position_Injected(void* nativePtr, CS_Vector3* value)
	{
		auto* script = static_cast<Component*>(nativePtr);
		if (!script || !script->m_gameObject) return;

		glm::vec3 pos(value->x, value->y, value->z);
		script->m_gameObject->SetLocalPosition(pos);

		std::cout << "[C++] set_position_Injected: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
	}
}

void RegisterTransformBindings()
{
	mono_add_internal_call("GameEngine.Transform::get_position_Injected", (const void*)GameEngine_Transform_get_position_Injected);
	mono_add_internal_call("GameEngine.Transform::set_position_Injected", (const void*)GameEngine_Transform_set_position_Injected);
}

void RegisterScriptBindings()
{
	RegisterTransformBindings();
	// ... later you'll add Light, Camera, etc.
}
