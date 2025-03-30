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

	void Initialize(GLFWwindow* window);
	void Render();
	void Shutdown();

	void SaveScene(const std::string& filename);
	void LoadScene(const std::string& filename);

private:
	Engine* m_engine;
	std::shared_ptr<GameObject> m_selectedObject;
	bool m_showHierarchy = true;
	bool m_showInspector = true;
	bool m_showComponentMenu = false;
	std::string m_newGameObjectName = "New GameObject";
	std::string m_currentScenePath = "";

	void RenderMainMenuBar();
	void RenderHierarchyWindow();
	void RenderInspectorWindow();
	void RenderComponentMenu();

	void DisplayGameObject(std::shared_ptr<GameObject> gameObject);
	void DisplayTransformComponent(std::shared_ptr<GameObject> gameObject);
	void DisplayComponent(std::shared_ptr<Component> component);

	bool ButtonCenteredOnLine(const char* label);

	// Serialization helpers
	nlohmann::json SerializeScene();
	nlohmann::json SerializeGameObject(std::shared_ptr<GameObject> gameObject);
	nlohmann::json SerializeComponent(std::shared_ptr<Component> component);

	void DeserializeScene(const nlohmann::json& data);
	std::shared_ptr<GameObject> DeserializeGameObject(const nlohmann::json& data);
	void DeserializeComponent(std::shared_ptr<GameObject> gameObject, const nlohmann::json& data);

	// Component factory
	std::shared_ptr<Component> CreateComponentByType(const std::string& type);
};