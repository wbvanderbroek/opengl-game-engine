#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Engine/Components/Camera.h>
#include <Engine/Components/Light.h>
#include <Engine/ObjectStorage.h>
#include <Engine/Shader.h>

class Engine
{
public:
	Engine(GLFWwindow* window);

	Shader m_shaderProgram;
	std::shared_ptr<Camera> m_camera;
	ObjectStorage m_storage;

	// the lights add themselves to this list each frame
	std::vector<Light*> m_activeLights;

	void StartInternal();
	void UpdateInternal();
	void QuitInternal();
	void UpdateCameraSize(unsigned int width, unsigned int height);
	void UpdateLighting();

private:
	GLFWwindow* m_window;
	std::chrono::system_clock::time_point m_previousTime;
	Config& m_config;

	float CalculateDeltaTime();
};