#pragma once

#include <Component.h>

enum LightType
{
	pointLight,
	directionalLight,
	spotLight
};

class Light : public Component
{
public:
	Light() { m_runInEditor = true; }

	glm::vec4 m_lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // This is RGBA, always leave the alpha as 1.0f

	LightType m_lightType = LightType::spotLight;

	void Update(float deltaTime) override;
};