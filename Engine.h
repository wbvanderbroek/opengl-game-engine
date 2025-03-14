#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Model.h"
#include "ShaderClass.h"
#include <chrono>
#include "ObjectStorage.h"

class GameObject;
class Engine
{
public:
	Engine(unsigned int width, unsigned int height, GLFWwindow* window);
	void StartInternal();

	void UpdateInternal();

	void QuitInternal();
protected:
	virtual void Start() {}
	virtual void Update(float deltaTime) {}
	virtual void Quit() {}

private:
	unsigned int m_width;
	unsigned int m_height;
	GLFWwindow* m_window;

	Shader m_shaderProgram;
	Camera m_camera;
	Model m_model;

	std::chrono::system_clock::time_point m_previousTime;

	glm::vec4 m_lightColor;
	glm::vec3 m_lightPos;
	glm::mat4 m_lightModel;

	ObjectStorage m_storage;
};

#endif
