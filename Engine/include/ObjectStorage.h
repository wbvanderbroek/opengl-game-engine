#pragma once
#include <GameObject.h>
#include <json/json.h>
#include <memory>
#include <vector>

class Engine;
class GameObject;
class ObjectStorage
{
private:

	// Serialization helpers
	nlohmann::json SerializeScene();
	nlohmann::json SerializeGameObject(std::shared_ptr<GameObject> gameObject);
	nlohmann::json SerializeComponent(std::shared_ptr<Component> component);

	void DeserializeScene(const nlohmann::json& data);
	std::shared_ptr<GameObject> DeserializeGameObject(const nlohmann::json& data);
	void DeserializeComponent(std::shared_ptr<GameObject> gameObject, const nlohmann::json& data);

	// Component factory
	std::shared_ptr<Component> CreateComponentByType(const std::string& type);

public:
	ObjectStorage(Engine* engine);

	std::string m_currentScenePath = "";

	void RemoveGameObject(std::shared_ptr<GameObject> object);
	void CreateDefaultScene();

	template<typename T>
	std::shared_ptr<T> Instantiate(T&& obj)
	{
		auto object = std::make_shared<std::decay_t<T>>(std::forward<T>(obj));
		object->SetStorage(this);
		m_objects.push_back(object);
		object->OnCreate();
		return object;
	}
	void SaveScene(const std::string& filename);
	void LoadScene(const std::string& filename);


	Engine* m_engine;
	std::vector<std::shared_ptr<GameObject>> m_objects;
};