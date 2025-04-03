#pragma once

#include <cerrno>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/glad.h>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	Shader(const char* vertexFile, const char* fragmentFile);

	GLuint m_id;

	void Activate();
	void Delete();

private:
	void compileErrors(unsigned int shader, const char* type);
};