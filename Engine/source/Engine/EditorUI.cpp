#include <fstream>
#include <iostream>

#include <enum/magic_enum.hpp> 

#include <Engine/Components/Camera.h>
#include <Engine/Components/Component.h>
#include <Engine/Components/ComponentRegistry.h>
#include <Engine/Components/Light.h>
#include <Engine/Components/Model.h>
#include <Engine/Config.h>
#include <Engine/EditorUI.h>
#include <Engine/Engine.h>
#include <Engine/GameObject.h>
#include <Engine/ObjectStorage.h>

void EditorUI::Initialize(GLFWwindow* window)
{
	m_window = window;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");


	glGenFramebuffers(1, &m_gameFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gameFramebuffer);

	glGenTextures(1, &m_gameTexture);
	glBindTexture(GL_TEXTURE_2D, m_gameTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_sceneViewSize.x, m_sceneViewSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gameTexture, 0);

	glGenRenderbuffers(1, &m_gameDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_gameDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_sceneViewSize.x, m_sceneViewSize.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_gameDepthBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Game framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void EditorUI::PreUpdate()
{
	ImVec2 sceneSize = m_sceneViewSize;

	static int lastW = 0;
	static int lastH = 0;

	int newW = static_cast<int>(sceneSize.x);
	int newH = static_cast<int>(sceneSize.y);

	if (newW > 0 && newH > 0 && (newW != lastW || newH != lastH))
	{
		lastW = newW;
		lastH = newH;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindTexture(GL_TEXTURE_2D, m_gameTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newW, newH, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

		glBindRenderbuffer(GL_RENDERBUFFER, m_gameDepthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, newW, newH);

		m_engine->UpdateCameraSize(newW, newH);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_gameFramebuffer);
	glViewport(0, 0, newW, newH);
}

void EditorUI::Render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	RenderMainMenuBar();

	ImVec2 viewport = ImGui::GetMainViewport()->Size;
	float topOffset = 20.0f;
	float contentHeight = viewport.y - topOffset;

	ImGui::SetNextWindowPos(ImVec2(0, topOffset));
	ImGui::SetNextWindowSize(ImVec2(viewport.x, contentHeight));
	ImGui::Begin("##EditorRoot", nullptr,
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoBringToFrontOnFocus);

	RenderHierarchyWindow(contentHeight);

	float minPanelWidth = 100.0f;
	float maxLeftPanelWidth = viewport.x - m_inspectorWidth - m_splitterWidth - minPanelWidth;
	RenderSplitter("Left", m_leftPanelWidth, minPanelWidth, maxLeftPanelWidth, contentHeight, false);

	RenderSceneView(contentHeight, m_inspectorWidth, viewport.x);

	float maxInspectorWidth = viewport.x - m_leftPanelWidth - m_splitterWidth - minPanelWidth;
	RenderSplitter("Right", m_inspectorWidth, minPanelWidth, maxInspectorWidth, contentHeight, true);

	RenderInspectorWindow(contentHeight);

	ImGui::End(); // ##EditorRoot

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
				m_engine->m_storage.LoadScene("../Game/Assets/Scenes/scene.json");
			}

			if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
			{
				if (m_engine->m_storage.m_currentScenePath.empty())
					m_engine->m_storage.SaveScene("../Game/Assets/Scenes/scene.json");
				else
					m_engine->m_storage.SaveScene(m_engine->m_storage.m_currentScenePath);
			}

			if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
			{
				m_engine->m_storage.SaveScene("../Game/Assets/Scenes/scene.json");
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit", "Alt+F4"))
			{
				glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Create Empty", "Ctrl+Shift+N"))
			{
				auto newObject = m_engine->m_storage.Instantiate(GameObject());
				m_selectedObject = newObject;
			}

			if (ImGui::BeginMenu("3D Object"))
			{
				if (ImGui::MenuItem("Plane"))
				{
					auto plane = m_engine->m_storage.Instantiate(GameObject());
					plane->AddComponent(std::make_shared<Model>("Engine/CoreAssets/Models/plane/plane.fbx"));
					plane->SetLocalRotation(glm::vec3(270, 0, 0));
					m_selectedObject = plane;
				}

				if (ImGui::MenuItem("Building"))
				{
					auto building = m_engine->m_storage.Instantiate(GameObject());
					building->AddComponent(std::make_shared<Model>("Engine/CoreAssets/Models/building/MetalMineBuilding.fbx"));
					building->SetLocalRotation(glm::vec3(270, 0, 180));
					m_selectedObject = building;
				}

				if (ImGui::MenuItem("Character"))
				{
					auto character = m_engine->m_storage.Instantiate(GameObject());
					character->AddComponent(std::make_shared<Model>("Engine/CoreAssets/Models/character/char.fbx"));
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
					light->AddComponent(std::make_shared<Light>());
					m_selectedObject = light;
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Camera"))
			{
				auto camera = m_engine->m_storage.Instantiate(GameObject());
				camera->AddComponent(std::make_shared<Camera>());
				m_selectedObject = camera;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void EditorUI::RenderHierarchyWindow(float contentHeight)
{
	ImGui::BeginChild("Hierarchy", ImVec2(m_leftPanelWidth, contentHeight), true);

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

	ImGui::EndChild();
}

void EditorUI::RenderInspectorWindow(float contentHeight)
{
	ImGui::SameLine();
	ImGui::BeginChild("Inspector", ImVec2(0, contentHeight), true);

	if (m_selectedObject)
	{
		char buffer[256];
		//TODO: Apple support
#ifdef _WIN32
		strncpy_s(buffer, m_selectedObject->m_name.c_str(), sizeof(buffer));
#endif
#ifdef __linux__
		strncpy(buffer, m_selectedObject->m_name.c_str(), sizeof(buffer));
#endif
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
			for (const auto& [name, factory] : ComponentRegistry::Instance().GetRegistry())
			{
				if (ImGui::Selectable(name.c_str()))
				{
					if (m_selectedObject)
					{
						m_selectedObject->AddComponent(factory());
					}
					ImGui::CloseCurrentPopup();
				}
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

	ImGui::EndChild();
}

void EditorUI::RenderSplitter(const char* id, float& targetWidth, float minWidth, float maxWidth, float height, bool invertDelta)
{
	ImGui::SameLine();
	ImGui::PushID(id);

	ImGui::InvisibleButton("splitter", ImVec2(m_splitterWidth, height));

	if (ImGui::IsItemActive())
	{
		float delta = ImGui::GetIO().MouseDelta.x;
		if (invertDelta)
			delta = -delta;

		targetWidth = std::clamp(targetWidth + delta, minWidth, maxWidth);
	}

	if (ImGui::IsItemHovered() || ImGui::IsItemActive())
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);

	ImGui::GetWindowDrawList()->AddRectFilled(
		ImGui::GetItemRectMin(),
		ImGui::GetItemRectMax(),
		IM_COL32(150, 150, 150, 255)
	);

	ImGui::PopID();
}

void EditorUI::RenderSceneView(float contentHeight, float inspectorWidth, float viewportX)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	int windowWidth, windowHeight;
	glfwGetFramebufferSize(m_window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);

	ImGui::SameLine();

	ImVec2 sceneSize = ImVec2(
		viewportX - m_leftPanelWidth - m_splitterWidth * 2 - inspectorWidth,
		contentHeight
	);

	ImGui::BeginChild("Scene", sceneSize, true);

	m_sceneViewSize = ImGui::GetContentRegionAvail();
	m_sceneViewPos = ImGui::GetCursorScreenPos();

	ImGui::Image((ImTextureID)(uintptr_t)m_gameTexture,
		m_sceneViewSize, ImVec2(0, 1), ImVec2(1, 0));

	ImGui::EndChild();
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
		if (ImGui::CollapsingHeader(WithID("Model", modelComponent.get()).c_str(), flags))
		{
			ImGui::Text("Model Path: %s", modelComponent->m_modelPath.c_str());

			if (ImGui::Button(WithID("Change Model", modelComponent.get()).c_str()))
			{
				static int modelIndex = 0;
				const char* models[] = {
					"Engine/CoreAssets/Models/plane/plane.fbx",
					"Engine/CoreAssets/Models/building/MetalMineBuilding.fbx",
					"Engine/CoreAssets/Models/character/char.fbx"
				};

				modelIndex = (modelIndex + 1) % 3;
				modelComponent->m_modelPath = models[modelIndex];
				modelComponent->Reload();
			}
		}
	}
	else if (auto lightComponent = std::dynamic_pointer_cast<Light>(component))
	{
		if (ImGui::CollapsingHeader(WithID("Light", lightComponent.get()).c_str(), flags))
		{
			ImGui::ColorEdit4(WithID("Light Color", lightComponent.get()).c_str(), glm::value_ptr(lightComponent->m_lightColor));

			float intensity = lightComponent->m_lightColor.w;
			if (ImGui::SliderFloat(WithID("Intensity", lightComponent.get()).c_str(), &intensity, 0.0f, 1.0f))
				lightComponent->m_lightColor.w = intensity;

			using namespace magic_enum;

			constexpr auto names = enum_names<LightType>();
			int currentType = static_cast<int>(lightComponent->m_lightType);

			if (ImGui::Combo(WithID("Light Type", lightComponent.get()).c_str(), &currentType,
				[](void* data, int idx, const char** out_text)
				{
					auto& enumNames = *reinterpret_cast<const decltype(names)*>(data);
					if (idx < 0 || static_cast<size_t>(idx) >= enumNames.size()) return false;
					*out_text = enumNames[idx].data();
					return true;
				}, (void*)&names, static_cast<int>(names.size())))
			{
				lightComponent->m_lightType = static_cast<LightType>(currentType);
			}

		}
	}
	else if (auto cameraComponent = std::dynamic_pointer_cast<Camera>(component))
	{
		if (ImGui::CollapsingHeader(WithID("Camera", cameraComponent.get()).c_str(), flags))
		{
			float fov = cameraComponent->GetFOV();
			if (ImGui::SliderFloat(WithID("Field of View", cameraComponent.get()).c_str(), &fov, 10.0f, 120.0f))
				cameraComponent->SetFOV(fov);

			float nearClip = cameraComponent->GetNearClip();
			if (ImGui::SliderFloat(WithID("Near Clip", cameraComponent.get()).c_str(), &nearClip, 0.1f, 10.0f))
				cameraComponent->SetNearClip(nearClip);

			float farClip = cameraComponent->GetFarClip();
			if (ImGui::SliderFloat(WithID("Far Clip", cameraComponent.get()).c_str(), &farClip, 10.0f, 1000.0f))
				cameraComponent->SetFarClip(farClip);
		}
	}

	if (ImGui::Button(WithID("Remove Component", component.get()).c_str()))
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