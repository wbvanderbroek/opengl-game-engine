#pragma once

#include <Engine/Component.h>

enum LightType
{
	PointLight,
	DirectionalLight,
	SpotLight
};

class Light : public Component
{
public:
	Light() { m_runInEditor = true; }

	glm::vec4 m_lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // This is RGBA, always leave the alpha as 1.0f

	LightType m_lightType = LightType::PointLight;

	void Update(float deltaTime) override;
};