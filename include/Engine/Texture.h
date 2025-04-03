#pragma once

#include <glad/glad.h>
#include <stb/stb_image.h>

#include <Engine/Shader.h>

class Texture
{
public:
	Texture(const char* image, const char* texType, GLuint slot);

	GLuint m_id;
	GLuint m_unit;

	const char* m_type;

	void TexUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
};