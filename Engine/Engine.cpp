#include "Engine.h"
#include <chrono>
#include <Game/ObjectStorage.h>

Engine::Engine(unsigned int width, unsigned int height, GLFWwindow* window)
	: m_width(width),
	m_height(height),
	m_window(window),
	m_shaderProgram("Assets/Shaders/default.vert", "Assets/Shaders/default.frag"),
	m_camera(&m_storage , width, height, glm::vec3(0.0f, 0.0f, 2.0f), m_window),
	m_model("Assets/models/door/door.gltf")
{
	m_lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightPos = glm::vec3(0.5f, -1.5f, 0.5f);
	m_lightModel = glm::mat4(1.0f);
	m_lightModel = glm::translate(m_lightModel, m_lightPos);

	m_shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(m_shaderProgram.ID, "lightColor"), m_lightColor.x, m_lightColor.y, m_lightColor.z, m_lightColor.w);
	glUniform3f(glGetUniformLocation(m_shaderProgram.ID, "lightPos"), m_lightPos.x, m_lightPos.y, m_lightPos.z);

	glEnable(GL_DEPTH_TEST);

	std::shared_ptr<Camera> camera = std::make_shared<Camera>(m_camera);
	m_storage.AddGameObject(camera);
}

void Engine::StartInternal()
{
	m_shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(m_shaderProgram.ID, "lightColor"),
		m_lightColor.x, m_lightColor.y, m_lightColor.z, m_lightColor.w);
	glUniform3f(glGetUniformLocation(m_shaderProgram.ID, "lightPos"),
		m_lightPos.x, m_lightPos.y, m_lightPos.z);

	glEnable(GL_DEPTH_TEST);

	m_storage = ObjectStorage();

	for (auto& obj : m_storage.m_objects)
	{
		obj->Start();
	}
}

void Engine::UpdateInternal()
{
	// delta time
	auto currentTime = std::chrono::system_clock::now();
	auto elapsedSeconds = std::chrono::duration<double>();

	if (m_previousTime.time_since_epoch().count())
		elapsedSeconds = currentTime - m_previousTime;

	m_previousTime = currentTime;

	auto deltaTime = (float)elapsedSeconds.count();

	for (auto& obj : m_storage.m_objects)
		obj->Update(deltaTime);


	// Complete the rendering after the game logic is run
	glClearColor(0.1f, 0.5f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_model.Draw(m_shaderProgram, m_camera);

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void Engine::QuitInternal()
{
	for (auto& obj : m_storage.m_objects)
		obj->Quit();

	m_shaderProgram.Delete();


	glfwDestroyWindow(m_window);
	glfwTerminate();
}

