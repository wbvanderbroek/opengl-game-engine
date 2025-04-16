#pragma once

#include <filesystem>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <json/json.h>
#include <memory>
#include <string>
#include <vector>

class Engine;
class GameObject;
class Component;

class EditorUI
{
public:
	EditorUI(Engine* engine) : m_engine(engine) {};

	~EditorUI() { Shutdown(); };

	std::shared_ptr<GameObject> m_selectedObject;

	void Initialize(GLFWwindow* window);
	void Render();
	void Shutdown();

	// Framebuffer for Game View
	GLuint m_gameFramebuffer = 0;
	GLuint m_gameTexture = 0;
	GLuint m_gameDepthBuffer = 0;

	ImVec2 m_sceneViewSize = { 1280, 720 };
	ImVec2 m_sceneViewPos = { 0, 0 };

private:
	Engine* m_engine;
	bool m_showHierarchy = true;
	bool m_showInspector = true;
	std::string m_newGameObjectName = "New GameObject";

	void RenderMainMenuBar();
	void RenderHierarchyWindow();
	void RenderInspectorWindow();

	void DisplayGameObject(std::shared_ptr<GameObject> gameObject);
	void DisplayTransformComponent(std::shared_ptr<GameObject> gameObject);
	void DisplayComponent(std::shared_ptr<Component> component);

	bool ButtonCenteredOnLine(const char* label);

	inline std::string WithID(const std::string& label, const void* ptr)
	{
		return label + "##" + std::to_string(reinterpret_cast<uintptr_t>(ptr));
	}
};