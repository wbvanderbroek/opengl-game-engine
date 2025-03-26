#include <Engine.h>

Engine::Engine(unsigned int width, unsigned int height, GLFWwindow* window)
	: m_width(width),
	m_height(height),
	m_window(window),
	m_shaderProgram("Assets/Shaders/default.vert", "Assets/Shaders/default.frag"),
	m_storage(this)
{
	m_camera = m_storage.Instantiate(Camera(width, height));

	//activate shaders but if lighting is not added to scene everything will still be black
	m_shaderProgram.Activate();

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
	static auto lastTime = std::chrono::high_resolution_clock::now();
	static int frameCount = 0;

	float deltaTime = CalculateDeltaTime();

	std::vector<Light*> lightsList;

	for (auto& obj : m_storage.m_objects)
	{
		obj->Update(deltaTime);

		if (Light* lightObj = dynamic_cast<Light*>(obj.get()))
		{
			lightsList.push_back(lightObj);
		}
	}

	UpdateLighting(lightsList);



	glClearColor(0.1f, 0.5f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto& obj : m_storage.m_objects)
		obj->LateUpdate(deltaTime);

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


	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Engine::UpdateCameraSize(unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;
	m_camera->SetDimensions(width, height);
}

void Engine::UpdateLighting(std::vector<Light*> lights)
{
	glUniform1i(glGetUniformLocation(m_shaderProgram.m_id, "numLights"), static_cast<int>(lights.size()));

	for (size_t i = 0; i < lights.size(); i++) {
		char uniformName[128];

		snprintf(uniformName, sizeof(uniformName), "lights[%zu].color", i);
		glUniform4f(
			glGetUniformLocation(m_shaderProgram.m_id, uniformName),
			lights[i]->m_lightColor.x, lights[i]->m_lightColor.y,
			lights[i]->m_lightColor.z, lights[i]->m_lightColor.w
		);

		snprintf(uniformName, sizeof(uniformName), "lights[%zu].pos", i);
		glUniform3f(
			glGetUniformLocation(m_shaderProgram.m_id, uniformName),
			lights[i]->translation.x, lights[i]->translation.y, lights[i]->translation.z
		);

	}
}
