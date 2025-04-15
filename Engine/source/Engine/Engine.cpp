#include <Engine/Config.h>
#include <Engine/EditorUI.h>
#include <Engine/Engine.h>
#include <Engine/Scripting/ScriptEngine.h>

Engine::Engine(GLFWwindow* window)
	: m_window(window),
	m_shaderProgram("Engine/CoreAssets/Shaders/default.vert", "Engine/CoreAssets/Shaders/default.frag"),
	m_storage(this),
	m_config(Config::Instance())
{
	ScriptEngine::Initialize();
	// Activate shaders
	m_shaderProgram.Activate();

	// Editor UI
	if (m_config.m_mode == Mode::Editor)
	{
		m_config.m_editorUI = std::make_unique<EditorUI>(this);
		m_config.m_editorUI->Initialize(window);
	}

	glEnable(GL_DEPTH_TEST);

	m_storage.LoadFirstScene();
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
	if (m_config.m_mode == Mode::Editor)
	{
		// --- Resize the framebuffer if the Scene View size changed ---
		ImVec2 sceneSize = m_config.m_editorUI->GetSceneViewSize();

		static int lastW = 0;
		static int lastH = 0;

		int newW = static_cast<int>(sceneSize.x);
		int newH = static_cast<int>(sceneSize.y);

		if (newW > 0 && newH > 0 && (newW != lastW || newH != lastH))
		{
			lastW = newW;
			lastH = newH;

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glBindTexture(GL_TEXTURE_2D, m_config.m_editorUI->GetGameTexture());
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newW, newH, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

			glBindRenderbuffer(GL_RENDERBUFFER, m_config.m_editorUI->GetGameDepthBuffer());
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, newW, newH);

			m_camera->SetDimensions(newW, newH);
		}

		// --- Bind and render to framebuffer ---
		glBindFramebuffer(GL_FRAMEBUFFER, m_config.m_editorUI->GetGameFramebuffer());
		glViewport(0, 0, newW, newH);
	}

	// --- Timing ---
	m_activeLights.clear();
	static auto lastTime = std::chrono::high_resolution_clock::now();
	static int frameCount = 0;
	float deltaTime = CalculateDeltaTime();

	// --- Update Logic ---
	for (auto& obj : m_storage.m_objects)
		obj->Update(deltaTime);

	UpdateLighting();

	// --- Scene rendering ---
	glClearColor(0.1f, 0.5f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto& obj : m_storage.m_objects)
		obj->LateUpdate(deltaTime);

	// --- Unbind framebuffer ---
	if (m_config.m_mode == Mode::Editor)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		int windowWidth, windowHeight;
		glfwGetFramebufferSize(m_window, &windowWidth, &windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);
	}

	// --- Draw UI ---
	if (m_config.m_mode == Mode::Editor)
		m_config.m_editorUI->Render();

	glfwSwapBuffers(m_window);
	glfwPollEvents();

	// --- FPS counter ---
	frameCount++;
	auto currentTime = std::chrono::high_resolution_clock::now();
	float elapsed = std::chrono::duration<float>(currentTime - lastTime).count();

	if (elapsed >= 1.0f)
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

	if (m_config.m_mode == Mode::Editor)
		m_config.m_editorUI->Shutdown();

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
			m_activeLights[i]->m_gameObject->GetGlobalPosition().x, m_activeLights[i]->m_gameObject->GetGlobalPosition().y, m_activeLights[i]->m_gameObject->GetGlobalPosition().z
		);

		snprintf(uniformName, sizeof(uniformName), "lights[%zu].type", i);
		glUniform1i(glGetUniformLocation(m_shaderProgram.m_id, uniformName), static_cast<int>(m_activeLights[i]->m_lightType));
	}
}
