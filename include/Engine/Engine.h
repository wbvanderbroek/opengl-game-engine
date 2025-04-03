#pragma once

#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Engine/Camera.h>
#include <Engine/EditorUI.h>
#include <Engine/Light.h>
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

	float CalculateDeltaTime()
	{
		auto currentTime = std::chrono::system_clock::now();
		auto elapsedSeconds = std::chrono::duration<double>();

		if (m_previousTime.time_since_epoch().count())
			elapsedSeconds = currentTime - m_previousTime;

		m_previousTime = currentTime;

		return (float)elapsedSeconds.count();
	}
};