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

	ImVec2 viewportSize = ImGui::GetIO().DisplaySize;

	float panelWidth = viewportSize.x * 0.2f;
	float inspectorWidth = viewportSize.x * 0.25f;

	// Hierarchy Window (left)
	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::SetNextWindowSize(ImVec2(panelWidth, viewportSize.y - 20));
	if (m_showHierarchy) RenderHierarchyWindow();

	//// Game View (middle)
	//ImGui::SetNextWindowPos(ImVec2(panelWidth, 20));
	//ImGui::SetNextWindowSize(ImVec2(viewportSize.x - panelWidth - inspectorWidth, viewportSize.y - 20));

	// Inspector (right)
	ImGui::SetNextWindowPos(ImVec2(viewportSize.x - inspectorWidth, 20));
	ImGui::SetNextWindowSize(ImVec2(inspectorWidth, viewportSize.y - 20));
	if (m_showInspector) RenderInspectorWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorUI::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void EditorUI::RenderMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene", "Ctrl+N"))
			{
				m_engine->m_storage.ClearScene();
				m_engine->m_storage.m_currentScenePath = "";
			}

			if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
			{
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
				m_engine->m_storage.SaveScene("Assets/Scenes/scene.json");
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit", "Alt+F4"))
			{
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
					plane->SetLocalRotation(glm::vec3(270, 0, 0));
					m_selectedObject = plane;
				}

				if (ImGui::MenuItem("Building"))
				{
					auto building = m_engine->m_storage.Instantiate(GameObject());
					building->AddComponent(Model("Assets/models/building/MetalMineBuilding.fbx"));
					building->SetLocalRotation(glm::vec3(270, 0, 180));
					m_selectedObject = building;
				}

				if (ImGui::MenuItem("Character"))
				{
					auto character = m_engine->m_storage.Instantiate(GameObject());
					character->AddComponent(Model("Assets/models/character/char.fbx"));
					character->SetLocalRotation(glm::vec3(270, 0, 0));
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

void EditorUI::RenderHierarchyWindow()
{
	ImGui::Begin("Hierarchy", &m_showHierarchy);

	if (ButtonCenteredOnLine("Create Game Object"))
	{
		auto newObject = m_engine->m_storage.Instantiate(GameObject());
		m_selectedObject = newObject;
	}

	ImGui::Separator();

	for (auto& gameObject : m_engine->m_storage.m_objects)
	{
		DisplayGameObject(gameObject);
	}

	ImGui::End();
}

void EditorUI::RenderInspectorWindow()
{
	ImGui::Begin("Inspector", &m_showInspector);

	if (m_selectedObject)
	{
		char buffer[256];
		strncpy_s(buffer, m_selectedObject->m_name.c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0'; // Ensure null-termination

		if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
			m_selectedObject->m_name = buffer;
		}

		DisplayTransformComponent(m_selectedObject);

		ImGui::Separator();

		// Display all components
		for (auto& component : m_selectedObject->m_components)
		{
			DisplayComponent(component);
			ImGui::Separator();
		}

		if (ImGui::Button("Add Component", ImVec2(-1, 0)))
		{
			ImGui::OpenPopup("AddComponentPopup");
		}

		if (ImGui::BeginPopup("AddComponentPopup"))
		{
			if (ImGui::Selectable("Model"))
			{
				if (m_selectedObject)
					m_selectedObject->AddComponent(Model("Assets/models/plane/plane.fbx"));
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Selectable("Light"))
			{
				if (m_selectedObject)
					m_selectedObject->AddComponent(Light());
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Selectable("Camera"))
			{
				if (m_selectedObject)
					m_selectedObject->AddComponent(Camera());
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::Spacing();

		if (ImGui::Button("Delete GameObject", ImVec2(-1, 0)))
		{
			m_selectedObject->Destroy();
		}
	}
	else
	{
		ImGui::Text("No GameObject selected");
	}

	ImGui::End();
}

void EditorUI::DisplayGameObject(std::shared_ptr<GameObject> gameObject)
{
	if (!gameObject) return;

	// Create a unique ID for ImGui
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (m_selectedObject == gameObject)
		flags |= ImGuiTreeNodeFlags_Selected;

	if (gameObject->m_children.empty())
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	bool nodeOpen = ImGui::TreeNodeEx((void*)(uintptr_t)gameObject.get(), flags, "%s", gameObject->m_name.c_str());

	// Handle selection
	if (ImGui::IsItemClicked())
	{
		m_selectedObject = gameObject;
	}

	// Right-click context menu
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Create Child"))
		{
			auto child = m_engine->m_storage.Instantiate(GameObject());
			gameObject->AddChild(child);
			m_selectedObject = child;
		}

		if (ImGui::MenuItem("Delete"))
		{
			if (m_selectedObject == gameObject)
				m_selectedObject = nullptr;

			gameObject->Destroy();

			ImGui::EndPopup();

			if (gameObject->m_children.empty())
			{
				return;
			}
			else
			{
				if (nodeOpen)
					ImGui::TreePop();
				return;
			}
		}

		ImGui::EndPopup();
	}

	if (nodeOpen && !(flags & ImGuiTreeNodeFlags_Leaf))
	{
		for (auto& child : gameObject->m_children)
		{
			DisplayGameObject(child);
		}
		ImGui::TreePop();
	}

}

void EditorUI::DisplayTransformComponent(std::shared_ptr<GameObject> gameObject)
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		glm::vec3 position = gameObject->localPosition;
		if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f))
		{
			gameObject->localPosition = position;
		}

		glm::vec3 localRotation = gameObject->GetLocalRotation();
		if (ImGui::DragFloat3("Rotation", glm::value_ptr(localRotation), 1.0f))
		{
			gameObject->SetLocalRotation(localRotation);
		}

		glm::vec3 localScale = gameObject->localScale;
		if (ImGui::DragFloat3("Scale", glm::value_ptr(localScale), 0.1f, 0.01f))
		{
			gameObject->localScale = localScale;
		}
	}
}

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

	if (ImGui::Button("Remove Component"))
	{
		m_selectedObject->RemoveComponent(component);
		return;
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