#include <chrono>
#include <Engine.h>
#include <ObjectStorage.h>

Engine::Engine(unsigned int width, unsigned int height, GLFWwindow* window)
	: m_width(width),
	m_height(height),
	m_window(window),
	m_shaderProgram("Assets/Shaders/default.vert", "Assets/Shaders/default.frag"),
	m_storage(this)
{
	m_camera = std::make_shared<Camera>(&m_storage, width, height, glm::vec3(0.0f, 0.0f, 2.0f), m_window);
	m_lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightPos = glm::vec3(0.5f, -1.5f, 0.5f);
	m_lightModel = glm::mat4(1.0f);
	m_lightModel = glm::translate(m_lightModel, m_lightPos);

	m_shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(m_shaderProgram.ID, "lightColor"), m_lightColor.x, m_lightColor.y, m_lightColor.z, m_lightColor.w);
	glUniform3f(glGetUniformLocation(m_shaderProgram.ID, "lightPos"), m_lightPos.x, m_lightPos.y, m_lightPos.z);

	glEnable(GL_DEPTH_TEST);
}

void Engine::StartInternal()
{
	m_shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(m_shaderProgram.ID, "lightColor"),
		m_lightColor.x, m_lightColor.y, m_lightColor.z, m_lightColor.w);
	glUniform3f(glGetUniformLocation(m_shaderProgram.ID, "lightPos"),
		m_lightPos.x, m_lightPos.y, m_lightPos.z);

	glEnable(GL_DEPTH_TEST);

	m_storage.AddGameObject(m_camera);

	for (auto& obj : m_storage.m_objects)
	{
		obj->Start();
	}
}

void Engine::UpdateInternal()
{
	for (auto& obj : m_storage.m_objects)
		obj->Update(CalculateDeltaTime());

	glClearColor(0.1f, 0.5f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto& obj : m_storage.m_objects)
		obj->LateUpdate(CalculateDeltaTime());

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void Engine::QuitInternal()
{
	for (auto& obj : m_storage.m_objects)
		obj->OnQuit();

	m_shaderProgram.Delete();


	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Engine::UpdateCameraSize(unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;
	m_camera->SetDimensions(width, height);
}
