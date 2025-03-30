#pragma once

#include <filesystem>
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
	EditorUI(Engine* engine);
	~EditorUI();

	std::shared_ptr<GameObject> m_selectedObject;

	void Initialize(GLFWwindow* window);
	void Render();
	void Shutdown();

private:
	Engine* m_engine;
	bool m_showHierarchy = true;
	bool m_showInspector = true;
	bool m_showComponentMenu = false;
	std::string m_newGameObjectName = "New GameObject";

	void RenderMainMenuBar();
	void RenderHierarchyWindow();
	void RenderInspectorWindow();
	void RenderComponentMenu();

	void DisplayGameObject(std::shared_ptr<GameObject> gameObject);
	void DisplayTransformComponent(std::shared_ptr<GameObject> gameObject);
	void DisplayComponent(std::shared_ptr<Component> component);

	bool ButtonCenteredOnLine(const char* label);
};