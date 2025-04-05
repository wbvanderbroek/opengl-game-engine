#include <Engine/Config.h>
#include <Engine/Engine.h>
#include <Engine/Scripting/ScriptBindings.h>
#include <Engine/Scripting/ScriptEngine.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>

Engine::Engine(GLFWwindow* window)
	: m_window(window),
	m_shaderProgram("Assets/Shaders/default.vert", "Assets/Shaders/default.frag"),
	m_storage(this),
	m_config(Config::Instance())
{
	// Init Mono and scripting system
	mono_set_dirs("C:/Program Files/Mono/lib", "C:/Program Files/Mono/etc");  // Adjust path if needed
	mono_config_parse(nullptr);

	MonoDomain* domain = mono_jit_init("GameScriptDomain");
	if (!domain)
	{
		std::cerr << "[Mono] Failed to initialize Mono JIT!" << std::endl;
		return;
	}

	// Load C# assembly
	MonoAssembly* assembly = mono_domain_assembly_open(domain, "Assets/Scripts/Scripts.dll");
	if (!assembly)
	{
		std::cerr << "[Mono] Failed to load Scripts.dll!" << std::endl;
		return;
	}

	// Register C++ internal calls for C# (e.g., transform.position)
	RegisterScriptBindings();

	// Store domain & image if needed globally
	ScriptEngine::SetMonoDomain(domain);
	ScriptEngine::SetAssembly(assembly);
	ScriptEngine::SetImage(mono_assembly_get_image(assembly));

	// Activate shaders
	m_shaderProgram.Activate();

	// Editor UI
	if (m_config.m_mode == Mode::Editor)
	{
		m_config.m_editorUI = std::make_unique<EditorUI>(this);
		m_config.m_editorUI->Initialize(window);
	}

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

	if (m_config.m_mode == Mode::Editor)
		m_config.m_editorUI->Render();

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
