#pragma once

class Light : public GameObject
{
public:
	glm::vec4 m_lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // This is RGBA, always leave the alpha as 1.0f
};