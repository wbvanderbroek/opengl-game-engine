#include <Engine/Components/Light.h>
#include <Engine/Engine.h>

void Light::Update(float deltaTime)
{
	m_gameObject->m_storage->m_engine->m_activeLights.push_back(this);
}