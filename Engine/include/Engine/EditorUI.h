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
	void PreUpdate();
	void Render();
	void Shutdown();

private:
	Engine* m_engine;
	GLFWwindow* m_window;
	std::string m_newGameObjectName = "New GameObject";

	GLuint m_gameFramebuffer = 0;
	GLuint m_gameTexture = 0;
	GLuint m_gameDepthBuffer = 0;

	ImVec2 m_sceneViewSize = { 1280, 720 };
	ImVec2 m_sceneViewPos = { 0, 0 };

	float m_leftPanelWidth = 300.0f;
	const float m_splitterWidth = 6.0f;
	float m_inspectorWidth = 300.0f;

	void RenderMainMenuBar();
	void RenderHierarchyWindow(float contentHeight);
	void RenderInspectorWindow(float contentHeight);
	void RenderSplitter(const char* id, float& targetWidth, float minWidth, float maxWidth, float height, bool invertDelta = false);
	void RenderSceneView(float contentHeight, float inspectorWidth, float viewportX);
	void DisplayGameObject(std::shared_ptr<GameObject> gameObject);
	void DisplayTransformComponent(std::shared_ptr<GameObject> gameObject);
	void DisplayComponent(std::shared_ptr<Component> component);

	bool ButtonCenteredOnLine(const char* label);

	inline std::string WithID(const std::string& label, const void* ptr)
	{
		return label + "##" + std::to_string(reinterpret_cast<uintptr_t>(ptr));
	}
};