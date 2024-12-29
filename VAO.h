#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include "VBO.h"
#include <glad/glad.h>

class VAO
{
public:
	GLuint ID;
	VAO();

	void LinkVBO(VBO& VBO, GLuint layout);
	void Bind();
	void Unbind();
	void Delete();
};

#endif
