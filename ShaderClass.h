#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <cerrno>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();
};

#endif

