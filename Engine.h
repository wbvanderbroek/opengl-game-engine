#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Camera.h"
#include "Model.h"
#include "ShaderClass.h"

class Engine
{
public:
	Engine(unsigned int width, unsigned int height, GLFWwindow* window);

	void Start();

	void Update();

	void Quit();

private:
	unsigned int m_width;
	unsigned int m_height;
	GLFWwindow* m_window;

	Shader m_shaderProgram;
	Camera m_camera;
	Model  m_model;

	glm::vec4 m_lightColor;
	glm::vec3 m_lightPos;
	glm::mat4 m_lightModel;
};

#endif
