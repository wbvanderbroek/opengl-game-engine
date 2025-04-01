#pragma once

#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Camera.h>
#include <EditorUI.h>
#include <Light.h>
#include <ObjectStorage.h>
#include <Shader.h>

class Engine
{
public:
	Engine(GLFWwindow* window, int argc, char* argv[]);

	Shader m_shaderProgram;
	std::shared_ptr<Camera> m_camera;
	ObjectStorage m_storage;
	std::unique_ptr<EditorUI> m_editorUI;

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
	bool m_isEditorMode = true;

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