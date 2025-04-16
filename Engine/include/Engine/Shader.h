#pragma once

#include <fstream>
#include <glad/glad.h>
#include <iostream>

class Shader
{
public:
	Shader(const char* vertexFile, const char* fragmentFile);

	GLuint m_id;

	void Activate();
	void Delete();

private:
	std::string GetFileContents(const char* filename);
	void compileErrors(unsigned int shader, const char* type);
};