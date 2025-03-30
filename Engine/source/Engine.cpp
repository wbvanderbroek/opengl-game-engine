#include <Engine.h>

Engine::Engine(GLFWwindow* window)
	: m_window(window),
	m_shaderProgram("Assets/Shaders/default.vert", "Assets/Shaders/default.frag"),
	m_storage(this),
	m_editorUI(this)
{
	// activate shaders but if lighting is not added to scene everything will still be black
	m_shaderProgram.Activate();

	// Initialize the editor UI
	m_editorUI.Initialize(window);

	glEnable(GL_DEPTH_TEST);
}

void Engine::StartInternal()
{
	for (auto& obj : m_storage.m_objects)
	{
		obj->Start();
	}
}

void Engine::UpdateInternal()
{
	m_activeLights.clear();
	static auto lastTime = std::chrono::high_resolution_clock::now();
	static int frameCount = 0;

	float deltaTime = CalculateDeltaTime();

	for (auto& obj : m_storage.m_objects)
		obj->Update(deltaTime);

	UpdateLighting();

	glClearColor(0.1f, 0.5f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto& obj : m_storage.m_objects)
		obj->LateUpdate(deltaTime);

	// Render ImGui UI
	m_editorUI.Render();

	glfwSwapBuffers(m_window);
	glfwPollEvents();

	// Fps calculator
	frameCount++;
	auto currentTime = std::chrono::high_resolution_clock::now();
	float elapsedTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();

	if (elapsedTime >= 1.0f)
	{
		std::cout << "FPS: " << frameCount << std::endl;
		frameCount = 0;
		lastTime = currentTime;
	}
}

void Engine::QuitInternal()
{
	for (auto& obj : m_storage.m_objects)
		obj->OnQuit();

	m_shaderProgram.Delete();
	m_editorUI.Shutdown();

	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Engine::UpdateCameraSize(unsigned int width, unsigned int height)
{
	if (m_camera != nullptr)
		m_camera->SetDimensions(width, height);
}

void Engine::UpdateLighting()
{
	glUniform1i(glGetUniformLocation(m_shaderProgram.m_id, "numLights"), static_cast<int>(m_activeLights.size()));

	for (size_t i = 0; i < m_activeLights.size(); i++) {
		char uniformName[128];

		snprintf(uniformName, sizeof(uniformName), "lights[%zu].color", i);
		glUniform4f(
			glGetUniformLocation(m_shaderProgram.m_id, uniformName),
			m_activeLights[i]->m_lightColor.x, m_activeLights[i]->m_lightColor.y,
			m_activeLights[i]->m_lightColor.z, m_activeLights[i]->m_lightColor.w
		);

		snprintf(uniformName, sizeof(uniformName), "lights[%zu].pos", i);
		glUniform3f(
			glGetUniformLocation(m_shaderProgram.m_id, uniformName),
			m_activeLights[i]->m_gameObject->translation.x, m_activeLights[i]->m_gameObject->translation.y, m_activeLights[i]->m_gameObject->translation.z
		);

	}
}
