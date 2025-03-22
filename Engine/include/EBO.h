#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>
#include <vector>

class EBO
{
public:
	EBO(std::vector<GLuint>& indices);

	GLuint m_id;

	void Bind();
	void Unbind();
	void Delete();
};

#endif