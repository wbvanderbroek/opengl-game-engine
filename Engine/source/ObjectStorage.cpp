#include <algorithm> 

#include <Config.h>
#include <Engine.h>
#include <GameObject.h>
#include <Light.h>
#include <Model.h>
#include <ObjectStorage.h>

ObjectStorage::ObjectStorage(Engine* engine) : m_engine(engine)
{
	if (Config::Instance().m_editorMode)
	{
		CreateDefaultScene();
	}
	else
	{
		LoadScene("Assets/Scenes/scene.json");
	}
}

void ObjectStorage::RemoveGameObject(std::shared_ptr<GameObject> object)
{
	auto it = std::find(m_objects.begin(), m_objects.end(), object);
	if (it != m_objects.end())
	{
		(*it)->OnDestroy();
		m_objects.erase(it);
	}
}

void ObjectStorage::CreateDefaultScene()
{
	auto camera = Instantiate(GameObject());
	camera->AddComponent(Camera());

	auto light = Instantiate(GameObject());
	light->AddComponent(Light());
	light->localPosition = glm::vec3(0, 10, 0);

	auto plane = Instantiate(GameObject());
	plane->AddComponent(Model("Assets/models/plane/plane.fbx"));
	plane->SetLocalRotation(glm::vec3(270, 0, 0));
	plane->localScale = glm::vec3(100, 100, 100);
	plane->localPosition = glm::vec3(0, -10, 0);
}

void ObjectStorage::SaveScene(const std::string& filename)
{
	nlohmann::json sceneData = SerializeScene();

	std::ofstream file(filename);
	if (file.is_open())
	{
		file << sceneData.dump(4);
		file.close();
		m_currentScenePath = filename;
		std::cout << "Scene saved to " << filename << std::endl;
	}
	else
	{
		std::cerr << "Failed to save scene to " << filename << std::endl;
	}
}

void ObjectStorage::LoadScene(const std::string& filename)
{
	std::ifstream file(filename);
	if (file.is_open())
	{
		try
		{
			nlohmann::json sceneData = nlohmann::json::parse(file);
			file.close();

			// Clear existing scene
			m_objects.clear();
			if (Config::Instance().m_editorMode)
				m_engine->m_editorUI->m_selectedObject = nullptr;

			// Load scene data from json
			DeserializeScene(sceneData);

			m_currentScenePath = filename;
			std::cout << "Scene loaded from " << filename << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error parsing scene file: " << e.what() << std::endl;
		}
	}
	else
	{
		std::cerr << "Failed to open scene file: " << filename << std::endl;
	}
}

nlohmann::json ObjectStorage::SerializeScene()
{
	nlohmann::json sceneData;
	sceneData["version"] = 1;

	nlohmann::json gameObjectsArray = nlohmann::json::array();

	for (auto& gameObject : m_objects)
	{
		gameObjectsArray.push_back(SerializeGameObject(gameObject));
	}

	sceneData["gameObjects"] = gameObjectsArray;
	return sceneData;
}

nlohmann::json ObjectStorage::SerializeGameObject(std::shared_ptr<GameObject> gameObject)
{
	nlohmann::json objectData;

	objectData["name"] = gameObject->m_name;

	objectData["localPosition"] = { gameObject->localPosition.x, gameObject->localPosition.y, gameObject->localPosition.z };
	objectData["localRotation"] = { gameObject->GetLocalRotation().x, gameObject->GetLocalRotation().y, gameObject->GetLocalRotation().z };
	objectData["localScale"] = { gameObject->localScale.x, gameObject->localScale.y, gameObject->localScale.z };

	nlohmann::json componentsArray = nlohmann::json::array();
	for (auto& component : gameObject->m_components)
		componentsArray.push_back(SerializeComponent(component));

	objectData["components"] = componentsArray;

	nlohmann::json childrenArray = nlohmann::json::array();
	for (auto& child : gameObject->m_children)
		childrenArray.push_back(SerializeGameObject(child));
	objectData["children"] = childrenArray;

	return objectData;
}


nlohmann::json ObjectStorage::SerializeComponent(std::shared_ptr<Component> component)
{
	nlohmann::json componentData;

	if (auto modelComponent = std::dynamic_pointer_cast<Model>(component))
	{
		componentData["type"] = "Model";
		componentData["modelPath"] = modelComponent->m_modelPath;
	}
	else if (auto lightComponent = std::dynamic_pointer_cast<Light>(component))
	{
		componentData["type"] = "Light";
		componentData["color"] = {
			lightComponent->m_lightColor.r,
			lightComponent->m_lightColor.g,
			lightComponent->m_lightColor.b,
			lightComponent->m_lightColor.a
		};
	}
	else if (auto cameraComponent = std::dynamic_pointer_cast<Camera>(component))
	{
		componentData["type"] = "Camera";
		componentData["fov"] = cameraComponent->GetFOV();
		componentData["nearClip"] = cameraComponent->GetNearClip();
		componentData["farClip"] = cameraComponent->GetFarClip();
	}
	else
	{
		componentData["type"] = "Unknown";
	}

	return componentData;
}

void ObjectStorage::DeserializeScene(const nlohmann::json& data)
{
	if (data.contains("gameObjects") && data["gameObjects"].is_array())
	{
		for (const auto& objData : data["gameObjects"])
		{
			auto gameObject = DeserializeGameObject(objData);
		}
	}
}

std::shared_ptr<GameObject> ObjectStorage::DeserializeGameObject(const nlohmann::json& data)
{
	auto gameObject = Instantiate(GameObject());

	if (data.contains("name") && data["name"].is_string())
	{
		gameObject->m_name = data["name"];
	}

	if (data.contains("localPosition") && data["localPosition"].is_array() && data["localPosition"].size() == 3)
	{
		gameObject->localPosition = glm::vec3(
			data["localPosition"][0],
			data["localPosition"][1],
			data["localPosition"][2]
		);
	}

	if (data.contains("localRotation") && data["localRotation"].is_array() && data["localRotation"].size() == 3)
	{
		gameObject->SetLocalRotation(glm::vec3(
			data["localRotation"][0],
			data["localRotation"][1],
			data["localRotation"][2]
		));
	}

	if (data.contains("localScale") && data["localScale"].is_array() && data["localScale"].size() == 3)
	{
		gameObject->localScale = glm::vec3(
			data["localScale"][0],
			data["localScale"][1],
			data["localScale"][2]
		);
	}

	if (data.contains("components") && data["components"].is_array())
	{
		for (const auto& componentData : data["components"])
		{
			DeserializeComponent(gameObject, componentData);
		}
	}

	if (data.contains("children") && data["children"].is_array())
	{
		for (const auto& childData : data["children"])
		{
			auto child = DeserializeGameObject(childData);
			gameObject->AddChild(child);
		}
	}

	return gameObject;
}

void ObjectStorage::DeserializeComponent(std::shared_ptr<GameObject> gameObject, const nlohmann::json& data)
{
	if (data.contains("type") && data["type"].is_string())
	{
		std::string type = data["type"];

		if (type == "Model" && data.contains("modelPath") && data["modelPath"].is_string())
		{
			std::string model = data["modelPath"];
			gameObject->AddComponent(Model(model));
		}
		else if (type == "Light")
		{
			auto light = Light();

			if (data.contains("color") && data["color"].is_array() && data["color"].size() == 4)
			{
				light.m_lightColor = glm::vec4(
					data["color"][0],
					data["color"][1],
					data["color"][2],
					data["color"][3]
				);
			}

			gameObject->AddComponent(light);
		}
		else if (type == "Camera")
		{
			auto camera = Camera();

			if (data.contains("fov") && data["fov"].is_number())
			{
				camera.SetFOV(data["fov"]);
			}

			if (data.contains("nearClip") && data["nearClip"].is_number())
			{
				camera.SetNearClip(data["nearClip"]);
			}

			if (data.contains("farClip") && data["farClip"].is_number())
			{
				camera.SetFarClip(data["farClip"]);
			}

			gameObject->AddComponent(camera);
		}
	}
}

std::shared_ptr<Component> ObjectStorage::CreateComponentByType(const std::string& type)
{
	if (type == "Model")
	{
		return std::make_shared<Model>("Assets/models/plane/plane.fbx");
	}
	else if (type == "Light")
	{
		return std::make_shared<Light>();
	}
	else if (type == "Camera")
	{
		return std::make_shared<Camera>();
	}

	return nullptr;
}