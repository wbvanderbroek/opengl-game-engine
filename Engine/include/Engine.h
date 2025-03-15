#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Camera.h>
#include <chrono>
#include <ObjectStorage.h>
#include <Shader.h>

class Model;
class GameObject;
class Engine
{
public:
	Engine(unsigned int width, unsigned int height, GLFWwindow* window);

	void StartInternal();
	void UpdateInternal();
	void QuitInternal();

	Shader m_shaderProgram;
	std::shared_ptr<Camera> m_camera;
protected:
	virtual void Start() {}
	virtual void Update(float deltaTime) {}
	virtual void Quit() {}
private:
	unsigned int m_width;
	unsigned int m_height;
	GLFWwindow* m_window;


	std::chrono::system_clock::time_point m_previousTime;

	glm::vec4 m_lightColor;
	glm::vec3 m_lightPos;
	glm::mat4 m_lightModel;

	ObjectStorage m_storage;

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

#endif
