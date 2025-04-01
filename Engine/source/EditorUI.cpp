#include <Camera.h>
#include <Component.h>
#include <EditorUI.h>
#include <Engine.h>
#include <fstream>
#include <GameObject.h>
#include <iostream>
#include <Light.h>
#include <Model.h>
#include <ObjectStorage.h>

// Constructor
EditorUI::EditorUI(Engine* engine) : m_engine(engine)
{
}

// Destructor
EditorUI::~EditorUI()
{
	Shutdown();
}

// Initialize ImGui
void EditorUI::Initialize(GLFWwindow* window)
{
	// Setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// Setup style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
}

void EditorUI::Render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	RenderMainMenuBar();

	// Get viewport size for manual layout
	ImVec2 viewportSize = ImGui::GetIO().DisplaySize;

	float panelWidth = viewportSize.x * 0.2f;
	float inspectorWidth = viewportSize.x * 0.25f;

	// Hierarchy Window (left)
	ImGui::SetNextWindowPos(ImVec2(0, 20)); // below menu bar
	ImGui::SetNextWindowSize(ImVec2(panelWidth, viewportSize.y - 20));
	if (m_showHierarchy) RenderHierarchyWindow();

	// Game View (middle)
	ImGui::SetNextWindowPos(ImVec2(panelWidth, 20));
	ImGui::SetNextWindowSize(ImVec2(viewportSize.x - panelWidth - inspectorWidth, viewportSize.y - 20));

	// Inspector (right)
	ImGui::SetNextWindowPos(ImVec2(viewportSize.x - inspectorWidth, 20));
	ImGui::SetNextWindowSize(ImVec2(inspectorWidth, viewportSize.y - 20));
	if (m_showInspector) RenderInspectorWindow();

	// Optional floating menu
	if (m_showComponentMenu)
		RenderComponentMenu();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}



// Shutdown ImGui
void EditorUI::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

// Render the main menu bar
void EditorUI::RenderMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene", "Ctrl+N"))
			{
				// Clear current scene
				m_engine->m_storage.m_objects.clear();
				m_selectedObject = nullptr;
				m_engine->m_storage.m_currentScenePath = "";
			}

			if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
			{
				// This would typically open a file dialog
				// For now, let's use a fixed filename
				m_engine->m_storage.LoadScene("Assets/Scenes/scene.json");
			}

			if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
			{
				if (m_engine->m_storage.m_currentScenePath.empty())
					m_engine->m_storage.SaveScene("Assets/Scenes/scene.json");
				else
					m_engine->m_storage.SaveScene(m_engine->m_storage.m_currentScenePath);
			}

			if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
			{
				// This would typically open a file dialog
				// For now, let's use a fixed filename
				m_engine->m_storage.SaveScene("Assets/Scenes/scene.json");
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit", "Alt+F4"))
			{
				// Request app to close
				glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Hierarchy", NULL, &m_showHierarchy);
			ImGui::MenuItem("Inspector", NULL, &m_showInspector);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Create Empty", "Ctrl+Shift+N"))
			{
				auto newObject = m_engine->m_storage.Instantiate(GameObject());
				newObject->OnCreate();
				m_selectedObject = newObject;
			}

			if (ImGui::BeginMenu("3D Object"))
			{
				if (ImGui::MenuItem("Plane"))
				{
					auto plane = m_engine->m_storage.Instantiate(GameObject());
					plane->AddComponent(Model("Assets/models/plane/plane.fbx"));
					plane->SetRotation(glm::vec3(270, 0, 0));
					m_selectedObject = plane;
				}

				if (ImGui::MenuItem("Building"))
				{
					auto building = m_engine->m_storage.Instantiate(GameObject());
					building->AddComponent(Model("Assets/models/building/MetalMineBuilding.fbx"));
					building->SetRotation(glm::vec3(270, 0, 180));
					m_selectedObject = building;
				}

				if (ImGui::MenuItem("Character"))
				{
					auto character = m_engine->m_storage.Instantiate(GameObject());
					character->AddComponent(Model("Assets/models/character/char.fbx"));
					character->SetRotation(glm::vec3(270, 0, 0));
					m_selectedObject = character;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Light"))
			{
				if (ImGui::MenuItem("Point Light"))
				{
					auto light = m_engine->m_storage.Instantiate(GameObject());
					light->AddComponent(Light());
					m_selectedObject = light;
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Camera"))
			{
				auto camera = m_engine->m_storage.Instantiate(GameObject());
				camera->AddComponent(Camera());
				m_selectedObject = camera;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

// Render the hierarchy window
void EditorUI::RenderHierarchyWindow()
{
	ImGui::Begin("Hierarchy", &m_showHierarchy);

	if (ButtonCenteredOnLine("Create Game Object"))
	{
		auto newObject = m_engine->m_storage.Instantiate(GameObject());
		newObject->OnCreate();
		m_selectedObject = newObject;
	}

	ImGui::Separator();

	// Display all game objects in the scene
	for (auto& gameObject : m_engine->m_storage.m_objects)
	{
		DisplayGameObject(gameObject);
	}

	ImGui::End();
}

// Render the inspector window
void EditorUI::RenderInspectorWindow()
{
	ImGui::Begin("Inspector", &m_showInspector);

	if (m_selectedObject)
	{
		// Object name input (would require adding name to GameObject class)
		// ImGui::InputText("Name", &m_selectedObject->name);

		// Transform component always exists
		DisplayTransformComponent(m_selectedObject);

		ImGui::Separator();

		// Display all components
		for (auto& component : m_selectedObject->m_components)
		{
			DisplayComponent(component);
			ImGui::Separator();
		}

		// Add Component button
		if (ImGui::Button("Add Component", ImVec2(-1, 0)))
		{
			m_showComponentMenu = true;
		}

		// Delete GameObject button
		ImGui::Spacing();
		if (ImGui::Button("Delete GameObject", ImVec2(-1, 0)))
		{
			m_selectedObject->Destroy();
			m_selectedObject = nullptr;
		}
	}
	else
	{
		ImGui::Text("No GameObject selected");
	}

	ImGui::End();
}

// Render the component selection menu
void EditorUI::RenderComponentMenu()
{
	ImGui::SetNextWindowSize(ImVec2(300, 0)); // set width, height auto
	ImGui::Begin("Add Component", &m_showComponentMenu, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::Button("Model", ImVec2(-1, 0)))
	{
		if (m_selectedObject)
		{
			m_selectedObject->AddComponent(Model("Assets/models/plane/plane.fbx"));
			m_showComponentMenu = false;
		}
	}

	if (ImGui::Button("Light", ImVec2(-1, 0)))
	{
		if (m_selectedObject)
		{
			m_selectedObject->AddComponent(Light());
			m_showComponentMenu = false;
		}
	}

	if (ImGui::Button("Camera", ImVec2(-1, 0)))
	{
		if (m_selectedObject)
		{
			m_selectedObject->AddComponent(Camera());
			m_showComponentMenu = false;
		}
	}

	if (ImGui::Button("Cancel", ImVec2(-1, 0)))
	{
		m_showComponentMenu = false;
	}

	ImGui::End();
}

// Display a game object in the hierarchy
void EditorUI::DisplayGameObject(std::shared_ptr<GameObject> gameObject)
{
	// Create a unique ID for ImGui
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (m_selectedObject == gameObject)
		flags |= ImGuiTreeNodeFlags_Selected;

	// You would ideally have a name property in GameObject
	std::string name = "GameObject";

	// Check for components to make the name more descriptive
	for (auto& component : gameObject->m_components)
	{
		if (dynamic_cast<Camera*>(component.get()))
		{
			name = "Camera";
			break;
		}
		else if (dynamic_cast<Light*>(component.get()))
		{
			name = "Light";
			break;
		}
		else if (dynamic_cast<Model*>(component.get()))
		{
			name = "Model";
			break;
		}
	}

	bool nodeOpen = ImGui::TreeNodeEx((void*)(uintptr_t)gameObject.get(), flags, "%s", name.c_str());

	// Handle selection
	if (ImGui::IsItemClicked())
	{
		m_selectedObject = gameObject;
	}

	// Right-click context menu
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete"))
		{
			if (m_selectedObject == gameObject)
				m_selectedObject = nullptr;

			gameObject->Destroy();
			ImGui::EndPopup();

			if (nodeOpen)
				ImGui::TreePop();

			return;
		}
		ImGui::EndPopup();
	}

	if (nodeOpen)
	{
		// You could render child objects here if you implement parent-child relationships
		ImGui::TreePop();
	}
}

// Display the transform component
void EditorUI::DisplayTransformComponent(std::shared_ptr<GameObject> gameObject)
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// Position
		glm::vec3 position = gameObject->translation;
		if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f))
		{
			gameObject->translation = position;
		}

		// Rotation
		glm::vec3 rotation = gameObject->GetRotation();
		if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 1.0f))
		{
			gameObject->SetRotation(rotation);
		}

		// Scale
		glm::vec3 scale = gameObject->scale;
		if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f, 0.01f))
		{
			gameObject->scale = scale;
		}
	}
}

// Display a component in the inspector
void EditorUI::DisplayComponent(std::shared_ptr<Component> component)
{
	std::string componentName = "Component";
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;

	if (auto modelComponent = std::dynamic_pointer_cast<Model>(component))
	{
		if (ImGui::CollapsingHeader("Model", flags))
		{
			ImGui::Text("Model Path: %s", modelComponent->m_modelPath.c_str());

			if (ImGui::Button("Change Model"))
			{
				// This would typically open a file dialog
				// For now, just cycle through some models
				static int modelIndex = 0;
				const char* models[] = {
					"Assets/models/plane/plane.fbx",
					"Assets/models/building/MetalMineBuilding.fbx",
					"Assets/models/character/char.fbx"
				};

				modelIndex = (modelIndex + 1) % 3;
				modelComponent->m_modelPath = models[modelIndex];
				modelComponent->Reload();
			}
		}
	}
	else if (auto lightComponent = std::dynamic_pointer_cast<Light>(component))
	{
		if (ImGui::CollapsingHeader("Light", flags))
		{
			ImGui::ColorEdit4("Light Color", glm::value_ptr(lightComponent->m_lightColor));

			// Other light properties could go here
			float intensity = lightComponent->m_lightColor.w;
			if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 1.0f))
			{
				lightComponent->m_lightColor.w = intensity;
			}
		}
	}
	else if (auto cameraComponent = std::dynamic_pointer_cast<Camera>(component))
	{
		if (ImGui::CollapsingHeader("Camera", flags))
		{
			float fov = cameraComponent->GetFOV();
			if (ImGui::SliderFloat("Field of View", &fov, 10.0f, 120.0f))
			{
				cameraComponent->SetFOV(fov);
			}

			float nearClip = cameraComponent->GetNearClip();
			if (ImGui::SliderFloat("Near Clip", &nearClip, 0.1f, 10.0f))
			{
				cameraComponent->SetNearClip(nearClip);
			}

			float farClip = cameraComponent->GetFarClip();
			if (ImGui::SliderFloat("Far Clip", &farClip, 10.0f, 1000.0f))
			{
				cameraComponent->SetFarClip(farClip);
			}
		}
	}

	// Right-click context menu for component
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Remove Component"))
		{
			// Remove component (would need to implement this functionality)
			// m_selectedObject->RemoveComponent(component);
			auto& components = m_selectedObject->m_components;
			auto it = std::find(components.begin(), components.end(), component);
			if (it != components.end())
			{
				components.erase(it);
			}
		}
		ImGui::EndPopup();
	}
}

// Helper to center a button
bool EditorUI::ButtonCenteredOnLine(const char* label)
{
	float width = ImGui::CalcTextSize(label).x + ImGui::GetStyle().FramePadding.x * 2.0f;
	float availWidth = ImGui::GetContentRegionAvail().x;

	float offset = (availWidth - width) * 0.5f;
	if (offset > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);

	return ImGui::Button(label);
}

