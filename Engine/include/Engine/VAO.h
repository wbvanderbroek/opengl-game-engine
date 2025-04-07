#pragma once

#include <glad/glad.h>

#include <Engine/VBO.h>

class VAO
{
public:
	VAO();

	GLuint m_id;

	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();
};