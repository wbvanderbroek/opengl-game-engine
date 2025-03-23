#pragma once

#include <vector>

#include <glad/glad.h>

class EBO
{
public:
	EBO(std::vector<GLuint>& indices);

	GLuint m_id;

	void Bind();
	void Unbind();
	void Delete();
};